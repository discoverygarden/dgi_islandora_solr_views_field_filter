<?php

/**
 * Implements hook_views_data().
 *
 * Adapted from the implementation inside of islandora_solr_views, to be a
 * replacement.
 *
 * @see islandora_solr_views_views_data()
 */
function dgi_islandora_solr_views_field_filter_views_data() {
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
