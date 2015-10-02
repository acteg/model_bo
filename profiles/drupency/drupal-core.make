api = 2
core = 7.x

projects[drupal][type] = core
projects[drupal][version] = 7.38

projects[drupal][patch][985814] = http://drupal.org/files/drupal-actions-985814-11-D7.patch
projects[drupal][patch][972536] = "http://drupal.org/files/drupal-menu-int-972536-86.patch"
projects[drupal][patch][1559696] = "http://drupal.org/files/default_image_index-1559696-27.patch"
projects[drupal][patch][1203112] = "http://drupal.org/files/add-x-ua-compatible-1203112-26.patch"

; does not pass since 7.38
; projects[drupal][patch][1934498] = "http://drupal.org/files/issues/image-alter-image-url_1934498_99_0.patch"

; taxonomy_allowed_values should use entity_label - http://drupal.org/node/1944246
projects[drupal][patch][1944246] = "http://drupal.org/files/drupal-taxonomy_allowed_values.patch"

; Make profiles inherit from each other, see https://drupal.org/node/2067229
; Allows to use drupency as a base profile including it's modules
projects[drupal][patch][] = "http://drupal.org/files/2067229-2-inheritable-profiles.patch"