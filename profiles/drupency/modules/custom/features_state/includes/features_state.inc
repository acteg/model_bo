<?php
/**
 * @file
 * Usefull functions for features_state.module.
 */

/**
 * Get a list of all feature modules.
 * (almost the same function as drush fl command)
 */
function features_state_list() {
  module_load_include('inc', 'features', 'features.export');
  if (variable_get('features_state_translate', 0) == 1) {
    $translation = TRUE;
    $headers = array(t('Feature'), t('Name'), t('Status'), t('Version'), t('State'));
  }
  else {
    $translation = FALSE;
    $headers = array('Feature', 'Name', 'Status', 'Version', 'State');
  }


  // Sort the Features list before compiling the output.
  $features = features_get_features();
  ksort($features);

  foreach ($features as $k => $m) {
    switch (features_get_storage($m->name)) {
      case FEATURES_DEFAULT:
      case FEATURES_REBUILDABLE:
        $storage = '';
        break;
      case FEATURES_OVERRIDDEN:
        $storage = $translation ? t('Overridden') : 'Overridden';
        break;
      case FEATURES_NEEDS_REVIEW:
        $storage = $translation ? t('Needs review') : 'Need review';
        break;
    }
    $rows[] = $translation ? array(
      $m->name,
      $m->info['name'],
      $m->status ? t('Enabled') : t('Disabled'),
      $m->info['version'],
      $storage
    ):
      array(
        $m->name,
        $m->info['name'],
        $m->status ? 'Enabled' : 'Disabled',
        $m->info['version'],
        $storage
      );
  }
  return theme('table', array('header' => $headers, 'rows' => $rows));
}