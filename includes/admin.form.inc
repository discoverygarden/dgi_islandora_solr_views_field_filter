<?php

/**
 * @file
 * Admin form.
 */

/**
 * Form building; build the admin form.
 */
function dgi_islandora_solr_views_field_filter_admin_form($form, &$form_state) {
  $form['dgi_islandora_solr_views_field_filter_regex_exclude'] = array(
    '#type' => 'textfield',
    '#title' => t('Exclusion regex'),
    '#description' => t('A regular expression. Solr fields matching will be <strong>excluded</strong> from Solr Views. Leave empty to avoid filtering.'),
    '#maxlength' => 4096,
    '#default_value' => variable_get('dgi_islandora_solr_views_field_filter_regex_exclude', DGI_ISLANDORA_SOLR_VIEWS_FIELD_FILTER_REGEX_EXCLUDE_DEFAULT),
    '#element_validate' => array('dgi_islandora_solr_views_field_filter_regex_element_validate'),
  );
  $form['dgi_islandora_solr_views_field_filter_fopen_timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Timeout'),
    '#description' => t('A integer in seconds to define the timeout value for the fopen request.'),
    '#element_validate' => array('element_validate_integer'),
    '#default_value' => variable_get('dgi_islandora_solr_views_field_filter_fopen_timeout', 120),
  );
  return system_settings_form($form);
}

/**
 * Element validation; ensure the entered pattern appears to be valid.
 */
function dgi_islandora_solr_views_field_filter_regex_element_validate($element, $form_state) {
  if ($element['#value'] && preg_match($element['#value'], 'test subject') === FALSE) {
    form_error($element, t('The regular expression in %field does not appear to be valid.', array(
      '%field' => $element['#title'],
    )));
  }
}
