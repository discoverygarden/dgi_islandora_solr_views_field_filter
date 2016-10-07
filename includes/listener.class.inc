<?php
/**
 * @file
 * Incremental JSON parser listener implementation.
 */

class DgiIslandoraSolrViewsFieldFilterListener extends \JsonStreamingParser\Listener\IdleListener {
  const FIELDS_DEPTH = 1;

  protected $startedDoc = FALSE;
  protected $inFields = FALSE;
  protected $depth = 0;

  protected $fields;

  protected $fieldName;
  protected $fieldInfo;
  protected $fieldItem;

  public function __construct(&$fields) {
    $this->fields =& $fields;
  }

  public function startDocument() {
    if ($this->startedDoc) {
      throw new ListenerException(t('Cannot reuse Listener.'));
    }
    $this->startedDoc = TRUE;
  }

  public function startObject() {
    if ($this->inFields && $this->depth == static::FIELDS_DEPTH) {
      $this->fieldName = NULL;
      $this->fieldInfo = array();
    }
    $this->depth++;
  }

  public function endObject() {
    if ($this->depth === static::FIELDS_DEPTH + 1 && $this->inFields) {
      $this->inFields = FALSE;
    }
    elseif ($this->depth === static::FIELDS_DEPTH + 2 && $this->inFields) {
      $this->addFieldToFields($this->fieldName, $this->fieldInfo);
    }
    $this->depth--;
  }

  public function key($key) {
    if ($this->depth == static::FIELDS_DEPTH && $key == 'fields') {
      // About to start in on the "fields" object.
      $this->inFields = TRUE;
    }
    elseif ($this->inFields) {
      // Processing something in the "fields" object.
      if ($this->depth == static::FIELDS_DEPTH + 1 && $this->checkFieldname($key)) {
        // Keys directly inside represent field names.
        $this->fieldName = $key;
      }
      elseif ($this->fieldName !== NULL) {
        // Other keys will correspond to info about particular fields.
        $this->fieldItem = $key;
      }
    }
  }

  public function value($value) {
    if ($this->inFields && $this->fieldItem !== NULL) {
      // Only interested when grabbing values for particular field info...
      $this->fieldInfo[$this->fieldItem] = $value;
      $this->fieldItem = NULL;
    }
  }

  protected function checkFieldname($fieldname) {
    static $regex = NULL;
    if ($regex === NULL) {
      $regex = variable_get('dgi_islandora_solr_views_field_filter_regex_exclude', DGI_ISLANDORA_SOLR_VIEWS_FIELD_FILTER_REGEX_EXCLUDE_DEFAULT);
    }
    
    // We do not allow to display 'sort_*' fields.
    if (strpos($fieldname, 'sort_') === 0) {
      return FALSE;
    }
    elseif ($regex && preg_match($regex, $fieldname)) {
      return FALSE;
    }

    return TRUE;
  }

  protected function addFieldToFields($solr_field_name, $solr_field) {
    // Set luke field variables.
    $field_type = $solr_field['type'];
    $field_schema = $solr_field['schema'];
    $field_dynamicbase = isset($solr_field['dynamicBase']) ? $solr_field['dynamicBase'] : NULL;

    // Set field handlers.
    $field = array();
    $field['title'] = $solr_field_name;
    $field['help'] = t('Type') . ': ' . $field_type;
    // Field handler.
    if ($field_type == 'date') {
      $field['field']['handler'] = 'islandora_solr_views_handler_field_date';
    }
    else {
      $field['field']['handler'] = 'islandora_solr_views_handler_field';
    }
    // Check if sortable.
    if (strstr($field_schema, "I") != FALSE AND strstr($field_schema, "M") == FALSE) {
      $field['field']['click sortable'] = TRUE;
    }
    // Argument handler.
    $field['argument'] = array(
      'handler' => 'islandora_solr_views_handler_argument',
    );
    // Filter handler.
    $field['filter'] = array(
      'handler' => 'islandora_solr_views_handler_filter',
    );
    // Sortable handler.
    // Check if sortable: must be indexed and can't be multivalued.
    // http://wiki.apache.org/solr/CommonQueryParameters#sort
    if (strstr($field_schema, "I") != FALSE AND strstr($field_schema, "M") == FALSE) {
      $field['sort'] = array(
        'handler' => 'islandora_solr_views_handler_sort',
      );
    }
    // Add array.
    $this->fields[$solr_field_name] = $field;
  }
}

class ListenerException extends \Exception {}
