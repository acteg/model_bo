<?php

/**
 * @file
 * Administration functions for features_state.module.
 */

/**
 * Form builder; Configure Features State settings for this site.
 */
function features_state_admin_settings() {
  $form['features_state'] = array(
    '#type' => 'fieldset',
    '#title' => t('Features State settings'),
  );
  $form['features_state']['features_state_translate'] = array(
    '#type' => 'checkbox',
    '#title' => t('Translate display'),
    '#default_value' => variable_get('features_state_translate', 0),
    '#description' => "You have to keep this setting OFF if you don't want feature states translations.",
  );
  return system_settings_form($form);
}
