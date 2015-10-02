; =============================================================
;                   Drupency makefile
; =============================================================

core = 7.x
api = 2

; For the drupal core version, check out drupal-core.make
; To build a Drupency site from scratch, use build-drupency.make

; ======================= Core / Site-building

projects[admin_menu][version] = 3.0-rc5
projects[admin_menu][subdir] = "contrib"

projects[backup_migrate][version] = 3.1
projects[backup_migrate][subdir] = "contrib"

projects[context][version] = 3.6
projects[context][subdir] = "contrib"

projects[ctools][version] = 1.7
projects[ctools][subdir] = "contrib"

projects[date][version] = 2.8
projects[date][subdir] = "contrib"
projects[date][version] = 2.8
projects[date][subdir] = "contrib"
projects[date][version] = 2.8
projects[date][subdir] = "contrib"

projects[devel][version] = 1.5
projects[devel][subdir] = "contrib"

projects[diff][version] = 3.2
projects[diff][subdir] = "contrib"

projects[entity][version] = 1.6
projects[entity][subdir] = "contrib"
projects[entitycache][version] = 1.2
projects[entitycache][subdir] = "contrib"
projects[entityreference][version] = 1.1
projects[entityreference][subdir] = "contrib"

projects[features][version] = 2.6
projects[features][subdir] = "contrib"
projects[features_extra][version] = 1.0-beta1
projects[features_extra][subdir] = "contrib"

projects[imagecache_external][version] = 2.1
projects[imagecache_external][subdir] = "contrib"

projects[jquery_update][version] = 3.0-alpha2
projects[jquery_update][subdir] = "contrib"

projects[libraries][version] = 2.2
projects[libraries][subdir] = "contrib"

projects[link][version] = 1.3
projects[link][subdir] = "contrib"

projects[md5check][version] = 1.0
projects[md5check][subdir] = "contrib"

projects[module_filter][version] = 2.0
projects[module_filter][subdir] = "contrib"

projects[nodeblock][version] = 1.6
projects[nodeblock][subdir] = "contrib"

projects[pathauto][version] = 1.2
projects[pathauto][subdir] = "contrib"

projects[rules][version] = 2.9
projects[rules][subdir] = "contrib"

projects[services][version] = 3.12
projects[services][subdir] = "contrib"

projects[strongarm][version] = 2.0
projects[strongarm][subdir] = "contrib"

projects[token][version] = 1.6
projects[token][subdir] = "contrib"

projects[uuid][version] = 1.0-alpha6
projects[uuid][subdir] = "contrib"

; latest version of uuid_feature crashs the drush make command
projects[uuid_features][version]= 1.0-alpha4
projects[uuid_features][subdir] = "contrib"

projects[variable][version] = 2.5
projects[variable][subdir] = "contrib"

projects[views][version] = 3.11
projects[views][subdir] = "contrib"
projects[views_bulk_operations][version] = 3.3
projects[views_bulk_operations][subdir] = "contrib"

projects[wysiwyg][version] = 2.2
projects[wysiwyg][subdir] = "contrib"
projects[wysiwyg][patch][] = "https://www.drupal.org/files/wysiwyg-ckeditor-4.1853550.136.patch"

projects[xmlsitemap][version] = 2.2
projects[xmlsitemap][subdir] = "contrib"

; ======================= Themes

projects[zen][version] = 5.5
projects[bootstrap][version] = 3.0

; Download actency bootstrap theme "bactency"
projects[bactency][type] = "theme"
projects[bactency][download][type] = "git"
projects[bactency][download][url] = "https://github.com/Actency/bactency.git"
projects[bactency][download][branch] = "develop"

; ======================= Libraries
; CKEditor 4.4.6
libraries[ckeditor][download][type] = "file"
libraries[ckeditor][download][url] = "http://download.cksource.com/CKEditor/CKEditor/CKEditor%204.4.6/ckeditor_4.4.6_standard.zip"

libraries[jquery.cycle][download][type] = "git"
libraries[jquery.cycle][download][url] = "https://github.com/malsup/cycle.git"
libraries[jquery.cycle][download][branch] = "master"

; ======================= Customs
projects[features_state][type] = "module"
projects[features_state][download][type] = "git"
projects[features_state][download][url] = "https://github.com/Actency/features-state.git"
projects[features_state][download][branch] = "develop"
projects[features_state][subdir] = "custom"

projects[dms_client][type] = "module"
projects[dms_client][download][type] = "git"
projects[dms_client][download][url] = "https://github.com/Actency/dms_client.git"
projects[dms_client][download][branch] = "feature/drupal_7"
projects[dms_client][subdir] = "custom"