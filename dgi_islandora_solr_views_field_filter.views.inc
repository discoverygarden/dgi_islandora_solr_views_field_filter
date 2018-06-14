<?php
/**
 * @file
 * Views hook implementations.
 */

/**
 * Heavy-lifting for hook_views_data().
 *
 * Adapted from the implementation inside of islandora_solr_views, to be a
 * replacement.
 *
 * @see islandora_solr_views_views_data()
 */
function _dgi_islandora_solr_views_field_filter_views_data() {
  // Set base variables.
  $base_table = 'islandora_solr';
  $data[$base_table]['table']['group'] = t('Islandora Solr');
  $data[$base_table]['table']['base'] = array(
    'query class' => 'islandora_solr_views_query',
    'title' => t('Islandora Solr'),
    'help' => t('Searches the Islandora Solr index.'),
    'field' => 'PID',
  );
  // Always add score handlers.
  $data[$base_table]['score'] = array(
    'title' => t('Score'),
    'help' => t('Relevancy score'),
    'field' => array(
      'handler' => 'islandora_solr_views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'islandora_solr_views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'islandora_solr_views_handler_filter',
    ),
    'sort' => array(
      'handler' => 'islandora_solr_views_handler_sort',
    ),
  );
  // Include object manage link.
  $data[$base_table]['islandora_manage'] = array(
    'title' => t('Manage link'),
    'help' => t('Provide a simple link to manage the content.'),
    'field' => array(
      'handler' => 'islandora_solr_views_handler_field_manage',
    ),
  );
  // Include a lucene query parser filter (q).
  $data[$base_table]['query_lucene'] = array(
    'title' => t('Query Parser (Lucene)'),
    'help' => t('Query Parser field to perform Lucene keyword searches.'),
    'filter' => array(
      'handler' => 'islandora_solr_views_handler_filter_query_lucene',
    ),
  );
  // Include a dismax query parser filter (q).
  $data[$base_table]['query_dismax'] = array(
    'title' => t('Query Parser (DisMax)'),
    'help' => t('Query Parser field to perform DisMax keyword searches.'),
    'filter' => array(
      'handler' => 'islandora_solr_views_handler_filter_query_dismax',
    ),
  );

  module_load_include('inc', 'dgi_islandora_solr_views_field_filter', 'includes/luke');
  dgi_islandora_solr_views_field_filter_add_fields($data[$base_table]);

  // Add our collection counting goodness.
  $data[$base_table]['solr_collection_count'] = array(
    'title' => 'Collection count',
    'help' => t('This shows the number of immediate child objects in the collection.'),
    'field' => array(
      'handler' => 'IslandoraSolrViewsHandlerCollectionCount',
    ),
  );
  return $data;
}

/**
 * Implements hook_views_data().
 */
function dgi_islandora_solr_views_field_filter_views_data() {
  $file = _dgi_islandora_solr_views_field_filter_get_data_file();

  try {
    $listener = new \JsonStreamingParser\Listener\InMemoryListener();
    $stream = fopen($file, 'rb');
    $parser = new \JsonStreamingParser\Parser($stream, $listener);
    $parser->parse();
    return $listener->getJSON();
  }
  catch (Exception $e) {
    fclose($stream);
    watchdog_exception('dgi_islandora_solr_views_field_filter', $e);
    throw $e;
  }
}

/**
 * Helper; build out the file URI.
 *
 * @return string
 *   The URI of the cache file.
 */
function _dgi_islandora_solr_views_field_filter_get_filename() {
  $directory = 'temporary://dgi_islandora_solr_views_field_filter';
  if (!file_prepare_directory($directory, FILE_MODIFY_PERMISSIONS, FILE_CREATE_DIRECTORY)) {
    throw new Exception(t('Unable to create our temporary directory: @dir', array(
      '@dir' => $directory,
    )));
  }
  return "$directory/fields.json";
}

/**
 * Helper; identify the file object in the DB for the file URI.
 *
 * @return object|bool
 *   The loaded file object if one could be identifier; otherwise, boolean
 *   FALSE.
 */
function _dgi_islandora_solr_views_field_filter_get_file() {
  $q = (new EntityFieldQuery())
    ->entityCondition('entity_type', 'file')
    ->propertyCondition('filename', _dgi_islandora_solr_views_field_filter_get_filename());

  $results = $q->execute();

  $result = reset($results);

  return $result ?
    file_load($result->fid) :
    FALSE;
}

/**
 * Manage the building of the file.
 *
 * Locking protocol very much adapted from that in Islandora's
 * islandora_view_datastream_retrieve_file_uri().
 *
 * @see islandora_view_datastream_retrieve_file_uri()
 *
 * @return string
 *   The URI of the managed file.
 *
 * @throws Exception
 *   If we fail to acquire locks, Exception can be thrown.
 */
function _dgi_islandora_solr_views_field_filter_get_data_file() {
  $file_uri = _dgi_islandora_solr_views_field_filter_get_filename();

  touch(drupal_realpath($file_uri));
  $fp = fopen($file_uri, 'r+b');
  if (flock($fp, LOCK_SH)) {
    try {
      fseek($fp, 0, SEEK_END);
      if (ftell($fp) === 0) {
        // Just opened at beginning of file, if beginning == EOF, need to grab
        // it.
        if (!flock($fp, LOCK_EX | LOCK_NB)) {
          // Hypothetically, two threads could have a "shared" lock with an
          // unpopulated file, so to avoid deadlock on the "exclusive" lock,
          // drop the "shared" lock before blocking to obtain the "exclusive"
          // lock.
          flock($fp, LOCK_UN);
        }
        if (flock($fp, LOCK_EX)) {
          // Get exclusive lock, write file.
          $file = islandora_temp_file_entry($file_uri, 'application/json');
          if ($file->filesize > 0) {
            // Populated in another thread while we were waiting for the
            // "exclusive" lock; drop lock and return.
            flock($fp, LOCK_UN);
            fclose($fp);
            return $file_uri;
          }

          // Populate the file...
          file_unmanaged_save_data(json_encode(_dgi_islandora_solr_views_field_filter_views_data()), $file->uri, FILE_EXISTS_REPLACE);

          clearstatcache($file->uri);
          $file = file_save($file);
        }
        else {
          throw new Exception(t('Failed to acquire write lock when acquiring @func response.', array(
            '@func' => 'dgi_islandora_solr_views_field_filter_views_data()',
          )));
        }
      }
      flock($fp, LOCK_UN);
      fclose($fp);
      return $file_uri;
    }
    catch (Exception $e) {
      flock($fp, LOCK_UN);
      fclose($fp);
      throw $e;
    }
  }
  throw new Exception(t('Failed to acquire shared lock when acquiring @func response.', array(
    '@func' => 'dgi_islandora_solr_views_field_filter_views_data()',
  )));
}
