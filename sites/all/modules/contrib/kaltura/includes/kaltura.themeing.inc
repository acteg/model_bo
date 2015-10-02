<?php
/**
 * @file
 * defines all themeing functions for kaltura core module.
 */

/*
 * theme the kaltura maintenance page
 *
 * this page is displayed whenever a try to create the $kaltura_client object fails
 */
function theme_kaltura_maintenance_page($variables) {
  return $variables['text'] . '<br /><a href="#" onclick="window.top.kalturaCloseModalBox()">' . t('Close') . '</a>';
}


/*
 * theme for kaltura/contribution_wizard_field
 * in case we add content as a CCK field
 */
function theme_kaltura_contribution_wizard_field($vars) {
  ctools_include('modal');
  ctools_include('ajax');

  $js_settings['kaltura']['gotoURL'] = kaltura_cw_destination();
  $js_settings['kaltura']['thumbnailBaseURL'] = _kaltura_thumbnail_base_url();
  $js_settings['kaltura']['kcwField'] = $vars['field_id'];
  $js_settings['kaltura']['insertSWF']['divKalturaCw'] = array(
    'url' => $vars['theme_params']['swfurl'],
    'swfID' => 'kalturaCw',
    'width' => $vars['theme_params']['width'],
    'height' => $vars['theme_params']['height'],
    'flashVars' => $vars['theme_params']['flashVarsStr'],
    'wMode' => NULL,
  );
  drupal_add_js($js_settings, 'setting');
  return '<div class="flash"><div id="divKalturaCw"></div></div>';
}
