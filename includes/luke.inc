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
  module_load_include('inc', 'dgi_islandora_solr_views_field_filter', 'includes/utilities');
  _dgi_islandora_solr_views_field_filter_load_parser();

  $url = dgi_islandora_solr_views_field_filter_get_luke_url();
  if (!$url) {
    return;
  }
  $timeout = variable_get('dgi_islandora_solr_views_field_filter_fopen_timeout', 120);
  $opts = array(
    'http' => array(
      'timeout' => $timeout,
    ),
  );
  $context = stream_context_create($opts);
  $stream = fopen($url, 'r', FALSE, $context);
  $listener = new DgiIslandoraSolrViewsFieldFilterListener($fields);
  try {
    set_time_limit(0);
    $parser = new \JsonStreamingParser\Parser($stream, $listener);
    $parser->parse();
  }
  catch (Exception $e) {
    fclose($stream);
    watchdog_exception('dgi_islandora_solr_views_field_filter', $e);
  }
  finally {
    set_time_limit(ini_get('max_execution_time'));
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
  watchdog('dgi_islandora_solr_views_field_filter', 'Solr Luke handler unavailable', NULL, WATCHDOG_CRITICAL);
  return FALSE;
}
