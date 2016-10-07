<?php
/**
 * @file
 * Helper for talking to Luke.
 */

/**
 * Add filtered fields to array.
 *
 * @param array $fields
 *   Reference to the array in which to add views field definitions.
 */
function dgi_islandora_solr_views_field_filter_add_fields(&$fields) {
  if (!class_exists("\JsonStreamingParser\Parser")) {
    $library_path = module_exists('libraries') ? libraries_get_path('jsonstreamingparser') : FALSE;
    if ($library_path) {
      $files = array(
        'Listener.php',
        'Parser.php',
        'ParsingError.php',
        'Listener/IdleListener.php',
      );
      foreach ($files as $file) {
        require_once "$library_path/src/$file";
      }
    }
    watchdog('dgi_islandora_solr_views_field_filter', 'The JSON Streaming Parser does not appear to be installed.');
    return;
  }
  $url = dgi_islandora_solr_views_field_filter_get_luke_url();
  if (!$url) {
    return;
  }

  $time_limit = variable_get('dgi_islandora_solr_views_field_filter_time_limit');
  if ($time_limit) {
    set_time_limit($time_limit);
  }

  $stream = fopen($url, 'r');
  $listener = new DgiIslandoraSolrViewsFieldFilterListener($fields);
  try {
    $parser = new \JsonStreamingParser\Parser($stream, $listener);
    $parser->parse();
  }
  catch (Exception $e) {
    fclose($stream);
    watchdog_exception('dgi_islandora_solr_views_field_filter', $e);
  }
}

/**
 * Get URL to Luke endpoint.
 *
 * Pared down from islandora_solr_get_luke().
 *
 * @see islandora_solr_get_luke()
 *
 * @return string|bool
 *   A string containing the URL to the Luke endpoint if we could hit Solr;
 *   otherwise, boolean FALSE.
 */
function dgi_islandora_solr_views_field_filter_get_luke_url() {
  module_load_include('inc', 'islandora_solr', 'includes/luke');
  $solr_url = variable_get('islandora_solr_url', 'localhost:8080/solr');

  // If Solr is available get Luke.
  if (islandora_solr_ping($solr_url)) {
    // Create URL.
    $luke_url = $solr_url . '/admin/luke';
    // Check for scheme specifier.
    $luke_url = islandora_solr_check_http($luke_url);
    $luke_query = array(
      'wt' => 'json',
      'numTerms' => 0,
    );
    // Generate nice URL.
    return url($luke_url, array('absolute' => TRUE, 'query' => $luke_query));
  }
  return FALSE;
}
