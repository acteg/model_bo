<?php
/**
 * @file
 * Contains functions for administration use of the kaltura core module.
 */

/**
 * Helper function that processes the partner registration with kaltura.
 *
 * If registration successful, the partner details are saved as system variables (drupal DB) and
 * the user is being redirected to the modules settings page where he should see the details inserted
 * and a configuration test which says that the configuraion works.
 *
 * TODO set up for new API.
 * @changes:
 *  removed references to categories
 * @questions:
 *  this method created problems within getServiceConfiguration wrt the Logger
 */
function kaltura_register_partner($values) {
  // Get module version.
  $info = system_get_info('module', 'kaltura');
  $module_version = !empty($info['version']) ? $info['version'] : 'N/A';

  $partner_name = ($values['server_url'] == 'http://www.kaltura.com') ? NULL : $values['first_name'] . '_' . $values['last_name'];
  $cms_pass = kaltura_generate_cms_password();

  // Maybe mark fields them in the form.
  $addit_val_array = array('title', 'vertical', 'would_you_like_to_be_contacted');
  $additional = array();
  foreach ($addit_val_array as $val) {
    $parm = new KalturaKeyValue();
    $parm->key = $val;
    $parm->value = $values[$val];
    $additional[] = $parm;
  }

  $partner = new KalturaPartner();
  // What is this field?
  $partner->name = $values['company'];
  $partner->phone = $values['phone'];
  $partner->website = $values['website'];
  $partner->notificationUrl = $values['kaltura_partner_url2'] . '/?q=kaltura/notification_handler';

  if (!empty($partner_name)) {
    $partner->adminName = $partner_name;
    $partner->firstName = NULL;
    $partner->lastName = NULL;
  }
  else {
    $partner->firstName = $values['first_name'];
    $partner->lastName = $values['last_name'];
  }

  $partner->adminEmail = $values['email'];
  $partner->country = $values['country'];
  $partner->state = $values['state'];
  $partner->description = $values['k_description'] . '|DRUPAL' . VERSION . '|module ver ' . $module_version;
  $partner->commercialUse = "non-commercial_use";
  $partner->type = 102;
  $partner->appearInSearch = '1';
  $partner->additionalParams = $additional;
  // TODO: Unused variable.
  $session_user = KalturaHelpers::getSessionUser();
  $config = KalturaHelpers::getServiceConfiguration();
  $config->serviceUrl = $values['server_url'];
  $kaltura_client = new KalturaClient($config);
  // $kaltura_client->setConfig($config);

  watchdog('regis', print_r($partner, TRUE));

  try {
    $res = $kaltura_client->partner->register($partner, $cms_pass);
    $res->cms_pass = $cms_pass;
  }
  catch (Exception $e) {
    $res = $e;
  }

  return $res;
}

/**
 * Helper function to retrieve partner's details from kaltura.
 *
 * @changes
 *  - kaltura client config set up to reflect changes in the API;
 *  - getsecrets now called instead of getPartner;
 *  - return array built using Kaltura Partner object;
 */
function kaltura_get_partner_details($partner_id, $cms_email, $cms_password, $server_url) {
  libraries_load('KalturaClient');

  // TODO: Unused variable.
  $session_user = KalturaHelpers::getSessionUser();
  $config = KalturaHelpers::getServiceConfiguration();
  $config->serviceUrl = $server_url;
  $config->partnerId = $partner_id;
  $kaltura_client = new KalturaClient($config);

  try {
    // Here we are trying to retrieve the partner details from the kaltura server.
    $result = $kaltura_client->partner->getsecrets($partner_id, $cms_email, $cms_password);
  }
  catch (Exception $e) {
    $result = $e;
  }

  return $result;
}

/**
 * Helper function that uses Kaltura's getDefaultWidget service to create some default widgets for
 * the partner, in case he does not have these yet.
 *
 * TODO: Useless unfinished function.
 *
 * @changes
 *  - changed client initialisation;
 *  - changed the getDefaultWidget function call to $kaltura_client->widget->get;
 * @questions
 *  - what do we do with the widget when we get them?
 */
function kaltura_create_widgets_for_partner($partner_id, $secret) {
  // $session_user = KalturaHelpers::getSessionUser();
  // $config = KalturaHelpers::getServiceConfiguration();
  // $config->partnerId = $partner_id;
  // $kaltura_client = new KalturaClient();
  // $kaltura_client->setConfig($config);

  try {
    $session_user = KalturaHelpers::getSessionUser();
    $kaltura_client = KalturaHelpers::getKalturaClient(1);
  }
  catch (Exception $e) {
    watchdog_exception('kaltura', $e);
  }

  $widgets = new KalturaSettings;

  foreach ($widgets->kdp_widgets as $type => $uiconfs) {
    foreach ($uiconfs as $skin => $confs) {
    // That doesn't work for me yet.
    // $result = $kaltura_client->widget->get("_1_".$confs['view_uiconf']);
    // $result = $kaltura_client->widget->get("_1_".$confs['remix_uiconf']);
    }
  }
}
/**
 * Determines how the general settings form will look like.
 *
 * @return mixed
 *  one of 2 states of forms:
 *    1) registration form (if variables do not exist in the DB) either as SaaS or Kaltura CE
 *    2) notifications settings + configuration test + settings form
 */
function kaltura_registration_form() {
  global $base_url;

  drupal_add_css(drupal_get_path('module', 'kaltura') . '/style/kaltura_reg_form.css');
  drupal_add_js(drupal_get_path('module', 'kaltura') . '/js/jquery.validate.min.js', array('group' => JS_LIBRARY));
  drupal_add_js(drupal_get_path('module', 'kaltura') . '/js/additional-methods.js', array('group' => JS_LIBRARY));

  // Check if we are already registered if so show status page and partner info.
  if (variable_get('kaltura_partner_id')) {
    return kaltura_status_form();
  }

  // load locale include file to get a country list from it
  include_once DRUPAL_ROOT . '/includes/locale.inc';

  $form['first_set'] = array(
    '#type' => 'fieldset',
    '#title' => t('Server settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['first_set']['server_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Server URL'),
    '#default_value' => variable_get('kaltura_server_url', 'http://www.kaltura.com'),
    '#description' => t('If you are working against Kaltura SaaS, leave this setting as it is. Otherwise, if you are working against a Kaltura CE or Kaltura On-Prem server, type the server URL here.'),
  );

  $form['first_set']['kaltura_partner_url2'] = array(
    '#type' => 'textfield',
    '#title' => t('Notification URL'),
    '#default_value' => variable_get('kaltura_partner_url2', $base_url),
    '#description' => t('server notifications allows the Kaltura video platform to update your Drupal server about the status of the media content. This is especially useful when uploading content in various formats that needs to be transcoded. In order to support server side notifications, your server must be publicly available on the internet. The notification URL is where notifications are sent to and where a notification handler script resides.'),
  );

  $form['first_set']['new_partner'] = array(
    '#type' => 'radios',
    '#title' => t('Partner'),
    '#options' => array(
      'new' => t('New'),
      'existing' => t('Existing'),
    ),
    '#default_value' => t('new'),
  );

  $form['ex_partner'] = array(
    '#type' => 'fieldset',
    '#title' => t('Existing partner registration'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="new_partner"]' => array('value' => t('existing')),
      ),
    ),
  );

  $form['ex_partner']['ex_partner_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Partner ID'),
    '#size' => 10,
  );

  $form['ex_partner']['ex_email'] = array(
    '#type' => 'textfield',
    '#title' => t('E-Mail'),
  );

  $form['ex_partner']['ex_password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#maxlength' => 64,
  );

  $form['registration'] = array(
    '#type' => 'fieldset',
    '#title' => t('New partner registration'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="new_partner"]' => array('value' => t('new')),
      ),
    ),
  );

  $form['registration']['first_name'] = array(
    '#type' => 'textfield',
    '#title' => t('First Name'),
    '#size' => 30,
    '#attributes' => array(
      'class' => array('required'),
      'title' => t('This field is required.'),
    ),
  );

  $form['registration']['last_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Last Name'),
    '#size' => 30,
    '#attributes' => array(
      'class' => array('required'),
      'title' => t('This field is required.'),
    ),
  );

  $form['registration']['email'] = array(
    '#type' => 'textfield',
    '#title' => t('E-Mail'),
    '#size' => 30,
    '#attributes' => array(
      'class' => array('required'),
      'title' => t('This field is required.'),
    ),
  );

  $form['registration']['phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone'),
    '#size' => 30,
    '#attributes' => array(
      'class' => array('required'),
      'title' => t('This field is required.'),
    ),
  );

  $form['registration']['company'] = array(
    '#type' => 'textfield',
    '#title' => t('Company'),
    '#size' => 30,
    '#attributes' => array(
      'class' => array('required'),
      'title' => t('This field is required.'),
    ),
  );

  $form['registration']['website'] = array(
    '#type' => 'textfield',
    '#title' => t('Website'),
    '#size' => 30,
  );

  $form['registration']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Job Title'),
    '#size' => 30,
    '#attributes' => array(
      'class' => array('required'),
      'title' => t('This field is required.'),
    ),
  );

  $form['registration']['vertical'] = array(
    '#type' => 'select',
    '#title' => t('Describe yourself'),
    // '#description' => t('/* description */'),
    '#options' => array(
      'Enterprise' => t('Enterprise / Small Business / Government Agency'),
      'Education' => t('Education Organization'),
      'Media' => t('Media Company / Agency'),
      'Service Provider' => t('CDN / ISP / Integrator / Hosting'),
      'Other' => t('Other'),
    ),
    '#attributes' => array(
      'class' => array('required'),
      'title' => t('This field is required.'),
    ),
  );

  $form['registration']['country'] = array(
    '#type' => 'select',
    '#title' => t('Country'),
    // '#description' => t('/* description */'),
    '#options' => country_get_list(),
    '#attributes' => array(
      'class' => array('required'),
      'title' => t('This field is required.'),
    ),
  );

  $form['registration']['state'] = array(
    '#type' => 'select',
    '#title' => t('State / Province'),
    // '#description' => t('/* description */'),
    '#options' => kaltura_us_states(),
    '#states' => array(
      'visible' => array(
        ':input[name="country"]' => array('value' => 'US'),
      ),
    ),
    '#default_value' => 'Not Applicable',
  );

  $form['registration']['would_you_like_to_be_contacted'] = array(
    '#type' => 'select',
    '#title' => t('Would you like a Kaltura Video Expert to contact you?'),
    // '#description' => t('/* description */'),
    '#options' => array(
      'yes' => t('Yes'),
      'no'  => t('Not right now'),
    ),
    '#states' => array(
      'visible' => array(
        ':input[name="server_url"]' => array('value' => 'http://www.kaltura.com'),
      ),
    ),
    '#attributes' => array(
      'class' => array('required'),
      'title' => t('This field is required.'),
    ),
    '#default_value' => 'yes',
  );

  $form['registration']['k_description'] = array(
    '#type' => 'textarea',
    '#title' => t('How do you plan to use Kaltura\'s video platform?'),
    '#cols' => 60,
  );

  $form['terms'] = array(
    '#type' => 'checkbox',
    '#title' => t('I accept !terms_of_use', array('!terms_of_use' => l(t('The Kaltura service Terms of Use'), 'http://corp.kaltura.com/terms_of_use_drupal'))),
    '#required' => TRUE,
    '#attributes' => array(
      'class' => array('required'),
      'title' => t('This field is required.'),
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('submit'),
  );

  return $form;
}

/**
 * Validate callback for kaltura_registration_form().
 *
 * @param $form
 * @param $form_state
 */
function kaltura_registration_form_validate(&$form, &$form_state) {
  $values = $form_state['values'];
  if ($values['new_partner'] == 'new') {
    $required = array('first_name', 'last_name', 'email', 'phone', 'company', 'title', 'country', 'would_you_like_to_be_contacted');
    foreach ($required as $field) {
      if (empty($values[$field])) {
        form_set_error('', t('The field !field_name is required', array('!field_name' => str_replace('_', ' ', $field))));
      }
    }
  }
  else {
    $required = array('ex_partner_id', 'ex_email', 'ex_password');
    foreach ($required as $field) {
      if (empty($values[$field])) {
        form_set_error('', t('The field !field_name is required', array('!field_name' => str_replace('ex_', ' ', $field))));
      }
    }
  }
}

/**
 * Submit callback for kaltura_registration_form().
 *
 * @param $form
 * @param $form_state
 */
function kaltura_registration_form_submit(&$form, &$form_state) {
  $values = $form_state['values'];

  libraries_load('KalturaClient');

  if ($values['new_partner'] == 'existing') {
    $res = kaltura_get_partner_details($values['ex_partner_id'], $values['ex_email'], $values['ex_password'], $values['server_url']);
  }
  else {
    $res = kaltura_register_partner($values);
  }

  if (!empty($res->id)) {
    variable_set('kaltura_partner_id', $res->id);
    variable_set('kaltura_subp_id', $res->id . '00');
    variable_set('kaltura_secret', $res->secret);
    variable_set('kaltura_admin_secret', $res->adminSecret);
    variable_set('kaltura_local_registration', 1);
    variable_set('kaltura_server_url', $values['server_url']);
    variable_set('kaltura_partner_cms_email', $res->adminEmail);
    variable_set('kaltura_partner_cms_password', $res->cmsPassword);
    variable_set('kaltura_partner_url2', $values['kaltura_partner_url2']);
    variable_set('kaltura_notification_type', 1);
    kaltura_update_kaltura_partner(2, $res->notificationUrl);
    kaltura_create_widgets_for_partner($res->id, $res->secret);
    drupal_set_message(t('Congratulations! You have successfully installed the Kaltura Video Module and registered for a Kaltura Partner ID'));
    drupal_goto('admin/config/media/kaltura');
  }
  else {
    $code = $res->getCode();
    $msg = $res->getMessage();
    if ($code == 'PARTNER_REGISTRATION_ERROR') {
      $msg = t('It seems you already have an account with Kaltura (the email you provided is in our records). If you want to create another Kaltura account, please enter a different email address.');
    }
    drupal_set_message($msg, 'error');
  }
}

/**
 * Helper function that returns a list of US states.
 *
 * TODO: Do we need to keep such a big array of data here?
 *
 * @access public
 * @return array
 */
function kaltura_us_states() {
  $state_list = array(
    'NA' => 'Not Applicable',
    'AL' => 'Alabama',
    'AK' => 'Alaska',
    'AZ' => 'Arizona',
    'AR' => 'Arkansas',
    'CA' => 'California',
    'CO' => 'Colorado',
    'CT' => 'Connecticut',
    'DE' => 'Delaware',
    'DC' => 'District Of Columbia',
    'FL' => 'Florida',
    'GA' => 'Georgia',
    'HI' => 'Hawaii',
    'ID' => 'Idaho',
    'IL' => 'Illinois',
    'IN' => 'Indiana',
    'IA' => 'Iowa',
    'KS' => 'Kansas',
    'KY' => 'Kentucky',
    'LA' => 'Louisiana',
    'ME' => 'Maine',
    'MD' => 'Maryland',
    'MA' => 'Massachusetts',
    'MI' => 'Michigan',
    'MN' => 'Minnesota',
    'MS' => 'Mississippi',
    'MO' => 'Missouri',
    'MT' => 'Montana',
    'NE' => 'Nebraska',
    'NV' => 'Nevada',
    'NH' => 'New Hampshire',
    'NJ' => 'New Jersey',
    'NM' => 'New Mexico',
    'NY' => 'New York',
    'NC' => 'North Carolina',
    'ND' => 'North Dakota',
    'OH' => 'Ohio',
    'OK' => 'Oklahoma',
    'OR' => 'Oregon',
    'PA' => 'Pennsylvania',
    'RI' => 'Rhode Island',
    'SC' => 'South Carolina',
    'SD' => 'South Dakota',
    'TN' => 'Tennessee',
    'TX' => 'Texas',
    'UT' => 'Utah',
    'VT' => 'Vermont',
    'VA' => 'Virginia',
    'WA' => 'Washington',
    'WV' => 'West Virginia',
    'WI' => 'Wisconsin',
    'WY' => 'Wyoming',
  );

  return $state_list;
}

/**
 * Implements hook_form().
 */
function kaltura_status_form() {
  global $base_url;

  $form['kaltura_notifications'] = array(
    '#type' => 'fieldset',
    '#title' => t('Kaltura Notifications'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['kaltura_notifications']['kaltura_partner_url2'] = array(
    '#type' => 'textfield',
    '#title' => t('Notification URL'),
    '#default_value' => variable_get('kaltura_partner_url2', $base_url),
    '#disabled' => TRUE,
    '#description' => t('Server notifications allows the Kaltura video platform to update your Drupal server about the status of the media content. This is especially useful when uploading content in various formats that needs to be transcoded. In order to support server side notifications, your server must be publicly available on the internet.'),
  );

  $form['kaltura_server_status'] = array(
    '#type' => 'fieldset',
    '#title' => t('Server Status'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  list($admin_session_status, $admin_session_error) = kaltura_check_server_status($admin = 1);
  list($normal_session_status, $normal_session_error) = kaltura_check_server_status($admin = 0);

  $status_class = ($admin_session_status && $normal_session_status) ? 'ok' : 'bad';
  $session_status = ($admin_session_status && $normal_session_status) ? 'OK' : 'ERROR';

  $error_status = FALSE;
  $error = ' kaltura_error: ';

  if ($admin_session_error) {
    $error .= '<br>admin session: ' . $admin_session_error;
    $error_status = TRUE;
  }
  if ($normal_session_error) {
    $error .= '<br>normal session: ' . $normal_session_error;
    $error_status = TRUE;
  }

  // TODO: Rewrite prefixes.
  $form['kaltura_server_status']['kaltura_Admin_test_server'] = array(
    '#type' => 'item',
    '#title' => t('Drupal to Kaltura Session Test'),
    '#prefix' => '<div class="kaltura_status_' . $status_class . '">',
    '#markup' => 'Session status: ' . $session_status . (($error_status) ? $error : ''),
    '#suffix' => '</div>',
  );

  // TODO: Do we need this commented stuff?? Function kaltura_test_notification_config() is not implemented yet!
  // if (kaltura_variable_get_real('kaltura_notification_type', 0) == 1) {
  //   list($status, $text) = kaltura_test_notification_config();
  //     $prefix = '<div class="kaltura_status_'. (($status)? 'ok': 'bad') .'">';
  //     $form['kaltura_server_status']['kaltura_notification_status'] = array(
  //       '#type' => 'item',
  //       '#title' => t('Kaltura to Drupal Notification Status'),
  //       '#prefix' => $prefix,
  //       '#markup' => $text,
  //       '#suffix' => '</div>'
  //    );
  //  }

  // TODO: Rewrite prefixes.
  list($status, $text) = kaltura_test_cron_job_status();
  $prefix = '<div class="kaltura_status_' . (($status) ? 'ok' : 'bad') . '">';
  $form['kaltura_server_status']['kaltura_cron_job_status'] = array(
    '#type' => 'item',
    '#title' => t('Cron-Job Status'),
    '#prefix' => $prefix,
    '#markup' => $text,
    '#suffix' => '</div>'
  );

  list($status, $text) = kaltura_test_crossdomain();
  $prefix = '<div class="kaltura_status_' . (($status) ? 'ok' : 'bad') . '">';
  $form['kaltura_server_status']['kaltura_crossdomain_status'] = array(
    '#type' => 'item',
    '#title' => t('CrossDomain.xml Status'),
    '#prefix' => $prefix,
    '#markup' => $text,
    '#suffix' => '</div>'
  );

  $form['kaltura_partner_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Partner Info'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['kaltura_partner_settings']['kaltura_partner_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Partner ID'),
    '#default_value' => variable_get('kaltura_partner_id'),
    '#size' => 20,
    '#maxlength' => 10,
    '#disabled' => TRUE,
  );

  $form['kaltura_partner_settings']['cms_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#default_value' => variable_get('kaltura_partner_cms_email'),
    '#disabled' => TRUE,
  );

  $form['kaltura_partner_settings']['cms_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#default_value' => variable_get('kaltura_partner_cms_password'),
    '#disabled' => TRUE,
  );

  return $form;
}

/**
 * Implements hook_form().
 */
function kaltura_settings_form() {
  if (variable_get('kaltura_partner_id')) {
    $k_helpers = new KalturaHelpers();
    $mobile = $k_helpers->hasMobileFlavores();
  }
  else {
    $mobile = FALSE;
  }

  $form['kaltura_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Kaltura general settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('You can perform various advanced administration actions on your media content via Kaltura Management Console. These actions include: setting access control, changing thumbnails, create players, update your email and password, et cetera. Access Kaltura Management Console here: !management_link', array('!management_link' => l('http://www.kaltura.com/index.php/kmc', 'http://www.kaltura.com/index.php/kmc'))),
  );

  $form['kaltura_settings']['kaltura_html5'] = array(
    '#type' => 'fieldset',
    '#title' => t('HTML5 settings mobile delivery'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['kaltura_settings']['kaltura_html5']['add_html5'] = array(
    '#type' => 'checkbox',
    '#title' => t('Support iPhone and iPad with HTML5'),
    '#default_value' => variable_get('kaltura_add_html5', $mobile),
    '#description' => t('The HTML5 video standard is supported in many mobile devices such as Apple’s iPhone and version 2 of the Google Android platform. Some of these popular mobile platforms don’t support Flash plugins and subsequently the video tag is the exclusive mechanism to distribute video in these mobile HTML browsers. By enabling this configuration, the Kaltura videos of this website would play on these devices.'),
    '#disabled' => !$mobile,
  );

  if ($mobile == FALSE) {
    $html5_desc_text_ce = t('Contact your Kaltura server administrator to enable HTML5 players');
    $html5_desc_text_saas = t('Contact Kaltura Sales to enable HTML5 players ') . l('http://site.kaltura.com/contact-us-drupal.html', 'http://site.kaltura.com/contact-us-drupal.html');
    $kaltura_server_url = variable_get('kaltura_server_url');

    $form['kaltura_settings']['kaltura_html5']['html5_text'] = array(
      '#markup' => $kaltura_server_url == 'http://www.kaltura.com' ? $html5_desc_text_saas : $html5_desc_text_ce,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('submit'),
  );

  return $form;
}

/**
 * Submit callback for kaltura_settings_form().
 *
 * @param $form
 * @param $form_state
 */
function kaltura_settings_form_submit($form, $form_state) {
  $vals = $form_state['values'];
  variable_set('kaltura_add_html5', $vals['add_html5']);

}

/**
 * Callback function for url 'admin/settings/kaltura/import'.
 *
 * TODO: This function should be thoroughly examined. Some weird stuff takes place.
 *
 * @changes
 *  - KalturaEntryFilter changed to KalturaMediaEntryFilter;
 *  - this may need some reworking;
 */
function kaltura_import_entries_page() {
  // Initialize variables.
  $entries = array();
  $entries_need_import = array();
  $each_page = 30;
  $total = 0;
  $media_total = 0;
  $finished = FALSE;
  $current_page = 1;
  $pager = '';

  if (isset($_POST['import_selected']) && isset($_POST['entries_to_import'])) {
    kaltura_import_entries($_POST['entries_to_import']);
    drupal_goto('admin/config/media/kaltura/import');
  }

  if (!variable_get('kaltura_partner_id')) {
    return t('You have not configured your partner details yet. Import cannot be performed until you setup your partner details');
  }

  if (empty($_GET['p'])) {
    $_GET['p'] = 1;
  }
  else {
    $current_page = $_GET['p'];
  }

  set_time_limit(30);
  while (!$finished) {
    // Get the media entries.
    list($media_total, $media_entries) = kaltura_get_my_entries($each_page, $current_page);
    $entries_need_import = array_merge($entries_need_import, $entries, $media_entries);
    $entries_need_import = kaltura_remove_existing_entries($entries_need_import);

    if (count($entries_need_import) >= $each_page) {
      $finished = TRUE;
    }

    if (($media_total < $current_page * $each_page) && ($total < $current_page * $each_page)) {
      $finished = TRUE;
    }

    $current_page++;
  }

  if (!count($entries_need_import)) {
    return t('No entries needed to be imported.');
  }

  if (count($entries_need_import) > $each_page) {
    $multi_dim = array_chunk($entries_need_import, $each_page, TRUE);
    $entries_need_import = $multi_dim[0];
  }

  if (($total + $media_total) > $each_page) {
    if ($media_total > 10000) {
      drupal_set_message(t('You have more than 10,000 entries. Not all entries are available for import in this interface. Please contact Kaltura Support for additional assistance'), 'warning');
    }
    $pages = ceil(($total + $media_total) / $each_page);
    $pager = '';
    $start_page = !empty($_GET['p']) ? $_GET['p'] : 1;
    if (($start_page - 5) <= 0) {
      $start_page = 1;
    }
    else {
      $start_page = $start_page - 5;
    }
    if ($pages > 10) {
      $end_page = ($start_page + 10 < $pages) ? $start_page + 10 : $pages;
      $pager .= l(t('First ...'), 'admin/config/media/kaltura/import') . '&nbsp';
    }
    else {
      $end_page = $pages;
    }
    for ($start = $start_page; $start <= $end_page; $start++) {
      if ($start == $_GET['p']) {
        $pager .= '<span>< ' . $start . ' ></span>';
      }
      else {
        $pager .= l($start, 'admin/config/media/kaltura/import', array('query' => array('p' => $start)));
      }
      $pager .= '&nbsp;';
    }
    if ($end_page > 10) {
      $pager .= l(t('... Last'), 'admin/config/media/kaltura/import', array('query' => array('p' => $pages))) . '&nbsp';
    }
  }

  $entries_options = array();
  if (is_array($entries_need_import) && count($entries_need_import)) {
    $ksetings = new KalturaSettings();
    foreach ($entries_need_import as $entry => $details) {
      $entries_options[$details['id']] = $details['name'] . ': ' . '(' . $ksetings->media_types_map[$details['type']] . ') - ' . $details['id'];
    }
  }

  $output = drupal_get_form('kaltura_create_entries_form', $entries_options);
  $output = drupal_render($output);
  return $pager . $output . $pager;
}

/**
 * Helper function that uses kaltura_get_entries() to retrieve a list of specific entries
 * belongs to the partner, and call kaltura_import_entry() for each of the retrieved list
 *
 * @param array $entries
 */
function kaltura_import_entries($entries) {
  $my_entries = kaltura_get_entries($entries, count($entries), TRUE);
  foreach ($my_entries[0]->objects as $entry) {
    kaltura_import_entry($entry);
  }
}

/**
 * Helper function that gets an entry retrieved from Kaltura,
 * builds a node object according to entry, and invokes node_insert to save the node.
 *
 * @param object $entry_details
 */
function kaltura_import_entry($entry_details) {
  $entry = kaltura_kmc_obj_to_drupal_array($entry_details);
  db_insert('node_kaltura')
    ->fields($entry)
    ->execute();
}

/**
 * Gets and object retrieved with the kaltura API and turns it to an entry array
 * that will be inserted in to the drupal db.
 *
 * @param mixed $obj
 *
 * @access public
 * @return array
 */
function kaltura_kmc_obj_to_drupal_array($obj) {
  $ent['kaltura_entryid'] = $obj->id;
  $ent['kaltura_tags'] = $obj->tags;
  $ent['kaltura_admin_tags'] = $obj->adminTags;
  $ent['kstatus'] = $obj->status;
  $ent['kaltura_media_type'] = $obj->mediaType;
  $ent['kaltura_duration'] = $obj->duration;
  $ent['kaltura_thumbnail_url'] = $obj->thumbnailUrl;
  $ent['kaltura_partner_data'] = $obj->partnerData;
  $ent['kaltura_source'] = $obj->sourceType;
  $ent['kaltura_width'] = $obj->width;
  $ent['kaltura_height'] = $obj->height;
  $ent['kaltura_download_url'] = $obj->downloadUrl;
  $ent['kaltura_views'] = $obj->views;
  $ent['kaltura_plays'] = $obj->plays;
  $ent['kaltura_votes'] = $obj->votes;
  $ent['kaltura_rank'] = $obj->rank;
  $ent['kaltura_total_rank'] = $obj->totalRank;
  $ent['kaltura_title'] = $obj->name;
  $ent['kaltura_description'] = $obj->description;
  $ent['kaltura_puser_id'] = is_numeric($obj->userId) ? $obj->userId : 1;
  $ent['kaltura_created_date'] = $obj->createdAt;
  return $ent;
}

/**
 * Helper function to provide form fields containing all entries for import
 * each entry as checkbox option.
 *
 * @param $form
 * @param $form_state
 * @param array $entries_options
 *
 * @return mixed
 */
function kaltura_create_entries_form($form, &$form_state, $entries_options = array()) {
  $form['desc'] = array(
    '#markup' => t('The list below includes media entries of your Kaltura account that are not synced to the Drupal server. Please select media entries to import to the Drupal server and submit. Imported media would be available to use within the Drupal site.'),
  );

  $form['check_all'] = array(
    '#type' => 'checkbox',
    '#title' => t('check all'),
    '#attributes' => array('class' => array('checkall')),
  );

  $form['entries_to_import'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Choose the entries you want to import'),
    '#options' => $entries_options,
  );

  $form['import_now'] = array(
    '#type' => 'submit',
    '#name' => 'import_selected',
    '#value' => t('Import selected'),
  );

  return $form;
}

/**
 * Helper functuion to get all entries of partner from Kaltura's DB.
 *
 * @changes
 *   - now retrieving all media entries - needs to import mix entries as well;
 */
function kaltura_get_my_entries($page_size = 30, $page = 1) {
  libraries_load('KalturaClient');

  try {
    $returned_entries = array();
    $is_admin = 1;
    // TODO: Unused variable.
    $session_user = KalturaHelpers::getSessionUser();
    // This is admin task and should be able to import all entries in the account.
    $kaltura_client = KalturaHelpers::getKalturaClient($is_admin);
    $pager_filter = new KalturaFilterPager();
    $kmf = new KalturaBaseEntryFilter();
    $kmf->typeEqual = KalturaEntryType::MEDIA_CLIP;
    $kmf->status = 2;
    $pager_filter->pageIndex = $page;
    $pager_filter->pageSize = $page_size;
    $result = $kaltura_client->baseEntry->listAction($kmf, $pager_filter);
    if ($result->totalCount > 0 && !empty($result->objects)) {
      $count = $result->totalCount;
      if (is_array($result->objects) && count($result->objects)) {
        foreach ($result->objects as $entry) {
          $returned_entries[$entry->id] = array(
            'id' => $entry->id,
            'name' => $entry->name,
            'type' => $entry->mediaType,
          );
        }
      }
      return array($count, $returned_entries);
    }
  }
  catch (Exception $e) {
    watchdog_exception('kaltura', $e);
  }

  return array(0, array());
}

/**
 * Helper function that receives a list of entries, and returns a new list
 * which does not contain entries that are already represented as nodes.
 *
 * @param array $entries
 *
 * @return array
 */
function kaltura_remove_existing_entries($entries) {
  $new_entries = array();
  if (is_array($entries) && !empty($entries)) {
    foreach ($entries as $entry) {
      $sel = db_select('node_kaltura', 'k')
        ->fields('k', array('kaltura_entryid'))
        ->condition('kaltura_entryid', $entry['id'], '=')
        ->countQuery()
        ->execute()
        ->fetchField();
      if (!$sel) {
        $new_entries[] = $entry;
      }
    }
  }
  return $new_entries;
}


/**
 * Helper function that checks the drupal cron job status.
 *
 * Cron job is required to collect statictics about kaltura items.
 *
 * @return array
 */
function kaltura_test_cron_job_status() {
  if (variable_get('cron_last')) {
    return array(TRUE, t('Cron has run. Be sure that you have a cron job configured, so you don\'t need to run it manually. Statistics about kaltura items will only be updated at cron run.'));
  }
  else {
    return array(FALSE, t('Cron has not run. If you don\'t configure cron job, statistics about kaltura items will not be updated.'));
  }
}

/**
 * Helper function that tests the notification status of the module.
 *
 * This function tries to close a loop with the kaltura server in a "real-time" test.
 * The notification handler sets a variable in the DB before this function run ends.
 * Therefore we need to use the kaltura_variable_get_real() function to check the variable status in the DB
 * and not in the $_GLOBALS which are set before this function even start.
 *
 * TODO: Not yet implemented.
 */
function kaltura_test_notification_config() {
  return array(TRUE, t('Test call not yet implemented, sorry'));

  if (kaltura_variable_get_real('kaltura_notification_status', 0) == 0) {
    $session_user = KalturaHelpers::getSessionUser();
    $kaltura_client = KalturaHelpers::getKalturaClient();
    $result = $kaltura_client->testNotification($session_user);
    sleep(3);
  }

  if (kaltura_variable_get_real('kaltura_notification_status', 0) === 1) {
    return array(TRUE, t('Drupal server receives notification from Kaltura server.'));
  }

  return array(FALSE, t('Drupal server fails to receive notifications from Kaltura. Please fix your configuration or disable server notifications.'));
}

/**
 * Replace default Drupal's variable_get() function with a "real-time" one.
 *
 * This function checks a variable status in the DB and not in the $_GLOBALS.
 * TODO: why do we need this one????
 */
function kaltura_variable_get_real($var, $default) {
  $result = db_select('variable', 'v')
    ->fields('v')
    ->condition('name', $var, '=')
    ->execute()
    ->fetchAssoc();
  if (!$result) {
    return $default;
  }

  return unserialize($result['value']);
}

/**
 * Helper function that checks if crossdomain.xml is in the right place.
 *
 * @return array
 */
function kaltura_test_crossdomain() {
  global $base_url;
  $cd_req = drupal_http_request($base_url . '/crossdomain.xml');
  if ($cd_req->code == '200') {
    return kaltura_parse_crossdomain($cd_req->data);
  }
  else {
    // Fall back to fetching and retrieving file contents (should we
    // succeed if HTTP request has failed, though?
    $filename = $_SERVER['DOCUMENT_ROOT'] . '/crossdomain.xml';
    if ($cd_xml = @file_get_contents($filename)) {
      return kaltura_parse_crossdomain($cd_xml);
    }
    else {
      return array(
        FALSE,
        t('crossdomain.xml file could not be found in your site\'s root directory') . ' [' . $filename . ']' .
        t('Please read !help_link.', array('!help_link' => l(t('here'), 'admin/help/kaltura'))),
      );
    }
  }
}

/**
 * Helper function that checks if crossdomain.xml says the right things.
 *
 * @param $crossdomain_xml
 *
 * @return array
 */
function kaltura_parse_crossdomain($crossdomain_xml) {
  if (!empty($crossdomain_xml)) {
    $cd_xml_content = simplexml_load_string($crossdomain_xml);
    foreach ($cd_xml_content->children() as $child) {
      $kaltura_ok = FALSE;
      $headers_ok = FALSE;

      $atts = $child->attributes();
      if ($atts['domain'] == '*' || $atts['domain'] == '*.kaltura.com') {
        $kaltura_ok = TRUE;
      }
      if ($atts['headers'] == '*' && ($atts['domain'] == '*' || $atts['domain'] == '*.kaltura.com')) {
        $headers_ok = TRUE;
      }
      if ($kaltura_ok && $headers_ok) {
        return array(TRUE, t('crossdomain.xml is in place and seems to be OK'));
      }
    }
  }
  return array(
    FALSE,
    t('crossdomain.xml is in place, but it is probably not configured properly. Please read !help_link.', array('!help_link' => l(t('here'), 'admin/help/kaltura'))
    ),
  );
}

/**
 * Helper function that updates partner notification settings in kaltura's server.
 *
 * @param int $notifications_config
 * @param string $notification_url
 *
 * @return null
 */
function kaltura_update_kaltura_partner($notifications_config = 2, $notification_url = '') {

  try {
    $partner = new KalturaPartner();
    $partner->allowMultiNotification = '1';
    $partner->notify = 1;
    $partner->notificationsConfig = '*=' . $notifications_config;
    $partner->notificationUrl = $notification_url ? $notification_url : url('kaltura/notification_handler', array('absolute' => TRUE));

    // TODO: Unused variable.
    $session_user = KalturaHelpers::getSessionUser();
    $kaltura_client = KalturaHelpers::getKalturaClient(1);
    $cms_pass = variable_get('kaltura_partner_cms_password', '');

    $result = $kaltura_client->partner->update($partner, $cms_pass);
    return $result;
  }
  catch (Exception $e) {
    watchdog_exception('kaltura', $e);
  }
}

/**
 * Helper function that tries to open session with the kaltura server.
 *
 * Used to verify partner details, and display errors if there are.
 *
 * @changed
 *  - added new variable_get statements required for the api calls;
 *  - updated KalturaClient creation process;
 *  - changed API calls;
 *
 * TODO: check correct format for error checking of result.
 */
function kaltura_check_server_status($admin) {
  try {
    $k_helpers = new KalturaHelpers();
    $k_helpers->getKalturaClient($admin);
    $kaltura_status = TRUE;
    $kaltura_error = '';
  }
  catch(Exception $ex) {
    $kaltura_status = FALSE;
    $kaltura_error = $ex->getMessage();
  }
  return array($kaltura_status, $kaltura_error);
}

/**
 * Helper function to generate a cms password.
 *
 * cms is the "administration panel" for kaltura's partners in kaltura's website.
 *
 * @todo Use Drupal methods.
 */
function kaltura_generate_cms_password() {
  $pass = md5(time() . ip_address());
  $pass = drupal_substr($pass, 0, 8);
  return $pass;
}
