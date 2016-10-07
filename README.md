# DGI Islandora Solr Views Field Filter

## Introduction

Too many fields being made available to Solr Views causes memory issues, so we allow the fields to be filtered out.

## Requirements

This module requires the following modules/libraries:

* [Islandora Solr Views](https://github.com/islandora/islandora_solr_views)
* [salsify/json-streaming-parser](https://github.com/salsify/jsonstreamingparser) (see note about "Composer Manager" below)

## Installation

Prior to the installation of this module, it may be desirable to install the [Composer Manager](https://www.drupal.org/project/composer_manager) module, to facilitate the installation of dependencies via Composer ("Composer Manager" is not marked as a strict dependecy, as installing at the _same time_ as this module would lead to things being broken due to the order of operations).

For basic install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

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
