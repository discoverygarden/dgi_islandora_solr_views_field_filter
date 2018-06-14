<?php

/**
 * @file
 * Misc utilities/helper functions.
 */

/**
 * Helper to make sure the JSON streaming parser is loaded.
 */
function _dgi_islandora_solr_views_field_filter_load_parser() {
  if (!class_exists("\JsonStreamingParser\Parser")) {
    $library_path = module_exists('libraries') ? libraries_get_path('jsonstreamingparser') : FALSE;
    if ($library_path) {
      $files = array(
        'Listener.php',
        'Parser.php',
        'ParsingError.php',
        'Listener/IdleListener.php',
        'Listener/InMemoryListener.php',
      );
      foreach ($files as $file) {
        require_once "$library_path/src/$file";
      }
    }
    if (!class_exists("\JsonStreamingParser\Parser")) {
      watchdog('dgi_islandora_solr_views_field_filter', 'The JSON Streaming Parser does not appear to be installed.');
    }
  }
}
