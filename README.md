# DGI Islandora Solr Views Field Filter

## Introduction

Too many fields being made available to Solr Views causes memory issues, so we allow the fields to be filtered out.

## Requirements

This module requires the following modules/libraries:

* [Islandora Solr Views](https://github.com/islandora/islandora_solr_views)
* [salsify/json-streaming-parser](https://github.com/salsify/jsonstreamingparser) (see [below](#json-streaming-parser))

## Installation

For base, install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

### json-streaming-parser

This can be installed two ways:

1. We include a `composer.json` file, which can get picked up by something like [Composer Manager](https://www.drupal.org/project/composer_manager), to install the dependency automatically.
2. If the class is not already available, and if the `libraries` module is available, we will use it to look up `jsonstreamingparser`, from which we will attempt to load the handful of files we require. As such, cloning the [repository](https://github.com/salsify/jsonstreamingparser) to `sites/all/libraries` such that `sites/all/libraries/jsonstreamingparser/src/Parser.php` exists should suffice.

## Configuration

Some configuration is available at `admin/islandora/tools/dgi_solr_views_field_filter`. He, we allow the configuration of:

1. The regex to use to filter out fields. The only filters out the compound sequencing relationships.
2. A time limit to (attempt) to set, to allow additional time for parsing.

## Troubleshooting/Issues

Having problems or solved a problem? Contact [discoverygarden](http://support.discoverygarden.ca).

## Maintainers/Sponsors

Current maintainers:

* [discoverygarden](http://www.discoverygarden.ca)

## Development

If you would like to contribute to this module, please check out our helpful
[Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers)
info, [Developers](http://islandora.ca/developers) section on Islandora.ca and
contact [discoverygarden](http://support.discoverygarden.ca).

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
