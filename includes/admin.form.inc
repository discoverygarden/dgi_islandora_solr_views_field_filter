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
    '#max_length' => 4096,
    '#default_value' => variable_get('dgi_islandora_solr_views_field_filter_regex_exclude', DGI_ISLANDORA_SOLR_VIEWS_FIELD_FILTER_REGEX_EXCLUDE_DEFAULT),
    '#element_validate' => array('dgi_islandora_solr_views_field_filter_regex_element_validate'),
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