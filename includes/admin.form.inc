<?php
/**
 * @file
 * Admin form.
 */

function dgi_islandora_solr_views_field_filter_admin_form($form, &$form_state) {
  $form['dgi_islandora_solr_views_field_filter_regex_exclude'] = array(
    '#type' => 'textfield',
    '#title' => t('Exclusion regex'),
    '#description' => t('A regular expression. Solr fields matching will be <strong>excluded</strong> from Solr Views.'),
    '#max_length' => 4096,
    '#default_value' => variable_get('dgi_islandora_solr_views_field_filter_regex_exclude', DGI_ISLANDORA_SOLR_VIEWS_FIELD_FILTER_REGEX_EXCLUDE_DEFAULT),
    '#element_validate' => array('dgi_islandora_solr_views_field_filter_regex_element_validate'),
  );
  $form['dgi_islandora_solr_views_field_filter_time_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Time limit'),
    '#description' => t('An additional amount of time (in seconds) to allow for acquisition and processing of the Luke response, for larger Solr schemas. Leave empty to run in the default amount of time.'),
    '#default_value' => variable_get('dgi_islandora_solr_views_field_filter_time_limit'),
    '#element_validate' => array('element_validate_integer_positive'),
  );

  return system_settings_form($form);
}

function dgi_islandora_solr_views_field_filter_regex_element_validate($element, $form_state) {
  if ($element['#value'] && preg_match($element['#value'], 'test subject') === FALSE) {
    form_error($element, t('The regular expression in %field does not appear to be valid.', array(
      '%field' => $element['#title'],
    )));
  }
}
