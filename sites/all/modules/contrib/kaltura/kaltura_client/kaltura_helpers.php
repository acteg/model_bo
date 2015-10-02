<?php

/**
 * functions edited getSessionUser
 *
 *
 */
class KalturaHelpers
{
  function __construct() {
    if (function_exists('libraries_load')) {
      libraries_load('KalturaClient');
    }
    else {
      $status_report = l("Status report", 'admin/reports/status');
      drupal_set_message(t("Kaltura module now requires Libraries module to be installed and enabled. Please follow instructions listed on the $status_report page."), 'error');
    }
  }

  function getContributionWizardFlashVars($ks, $kshowId, $partner_data, $type, $comment, $simple = false, $uiConf = KALTURASETTINGS_CW_UICONF_ID)
  {
    $sessionUser = KalturaHelpers::getSessionUser();
    $config = KalturaHelpers::getServiceConfiguration();

    $flashVars = array();

    $flashVars["userId"] = $sessionUser->id;
    if (!$simple)
    {
      $flashVars["sessionId"] = $ks;
    }
    else
    {
      $flashVars["ks"] = $ks;
      $flashVars["entryId"] 	 = -1;
      $flashVars["jsDelegate"]   = "delegate";
      $flashVars["maxUploads"]   = 1;
      $flashVars["subPId"]   = $config->subPartnerId;
    }

    if ($sessionUser->id == KALTURASETTINGS_ANONYMOUS_USER_ID) {
      $flashVars["isAnonymous"] = true;
    }

    $flashVars["partnerId"] 	= $config->partnerId;
    $flashVars["subPartnerId"] 	= $config->subPartnerId;
    if ($kshowId)
      // TODO: change the following line for roughcut
      $flashVars["kshow_id"] 	= ($type == 'entry')? $type.'-'.$kshowId: $kshowId;
    else
      $flashVars["kshow_id"] 	= -2;

    $flashVars["afterAddentry"] 	= "onContributionWizardAfterAddEntry";
    $flashVars["close"] 		= "onContributionWizardClose";
    $flashVars["partnerData"] 	= $partner_data;

    if ($simple)
      $flashVars["uiConfId"] 		= KALTURASETTINGS_CW_UICONF_ID_SIMPLE;
    else if (!$comment)
      $flashVars["uiConfId"] 		= (empty($uiConf) ? KALTURASETTINGS_CW_UICONF_ID : $uiConf);
    else
      $flashVars["uiConfId"] 		= KALTURASETTINGS_CW_COMMENTS_UICONF_ID;

    $flashVars["terms_of_use"] 	= "http://corp.kaltura.com/tandc" ;

    return $flashVars;
  }


  function getSimpleEditorFlashVars($ks, $kshowId, $type, $partner_data, $uiConfId = null)
  {
    $sessionUser = KalturaHelpers::getSessionUser();
    $config = KalturaHelpers::getServiceConfiguration();


    $flashVars = array();

    if ($type == 'entry')
    {
      $flashVars["entry_id"] 		= $kshowId;
      $flashVars["kshow_id"] 		= 'entry-'.$kshowId;
    } else {
      $flashVars["entry_id"] 		= -1;
      $flashVars["kshow_id"] 		= $kshowId;
    }

    $flashVars["partner_id"] 	= $config->partnerId;
    $flashVars["partnerData"] 	= $partner_data;
    $flashVars["subp_id"] 		= $config->subPartnerId;
    $flashVars["uid"] 			= $sessionUser->id;
    $flashVars["ks"] 			= $ks;
    $flashVars["backF"] 		= "onSimpleEditorBackClick";
    $flashVars["saveF"] 		= "onSimpleEditorSaveClick";
    if ($uiConfId)
      $flashVars["uiConfId"] 		= $uiConfId;
    else
      $flashVars["uiConfId"] 		= KALTURASETTINGS_SE_UICONF_ID;

    return $flashVars;
  }


  function getAdvancedEditorFlashVars($ks, $kshowId, $type, $partner_data, $uiConfId = null)
  {
    $sessionUser = KalturaHelpers::getSessionUser();
    $config = KalturaHelpers::getServiceConfiguration();


    $flashVars = array();

    if ($type == 'entry')
    {
      $flashVars["entry_id"] 		= $kshowId;
      $flashVars["kshow_id"] 		= 'entry-'.$kshowId;
    } else {
      $flashVars["entry_id"] 		= -1;
      $flashVars["kshow_id"] 		= $kshowId;
    }

    $flashVars["partner_id"] 	= $config->partnerId;
    $flashVars["partnerData"] 	= $partner_data;
    $flashVars["subp_id"] 		= $config->subPartnerId;
    $flashVars["uid"] 			= $sessionUser->id;
    $flashVars["ks"] 			= $ks;
    $flashVars["backF"] 		= "onSimpleEditorBackClick";
    $flashVars["saveF"] 		= "onSimpleEditorSaveClick";
    if ($uiConfId)
      $flashVars["uiConfId"] 		= $uiConfId;
    else
      $flashVars["uiConfId"] 		= KALTURASETTINGS_AE_UICONF_ID;

    return $flashVars;
  }

  function getKalturaPlayerFlashVars($ks, $kshowId = -1, $entryId = -1)
  {
    $sessionUser = KalturaHelpers::getSessionUser();
    //		$config = KalturaHelpers::getServiceConfiguration();

    $flashVars = array();

    //		$flashVars["kshowId"] 		= $kshowId;
    //		$flashVars["entryId"] 		= $entryId;
    //		$flashVars["partner_id"] 	= $config->partnerId;
    //		$flashVars["subp_id"] 		= $config->subPartnerId;
    $flashVars["uid"] 			= $sessionUser->id;
    //		$flashVars["ks"] 			= $ks;

    return $flashVars;
  }

  function flashVarsToString($flashVars)
  {
    $flashVarsStr = "";
    foreach ($flashVars as $key => $value)
    {
      $flashVarsStr .= ($key . "=" . urlencode($value) . "&");
    }
    return substr($flashVarsStr, 0, strlen($flashVarsStr) - 1);
  }

  function getSwfUrlForBaseWidget()
  {
    return KalturaHelpers::getSwfUrlForWidget(KALTURASETTINGS_BASE_WIDGET_ID);
  }

  function getSwfUrlForWidget($widgetId)
  {
    return KalturaHelpers::getKalturaServerUrl() . "/kwidget/wid/" . $widgetId;
  }

  function getContributionWizardUrl($uiConfId = null)
  {
    if ($uiConfId)
    {
      if (KALTURASETTINGS_CW_UICONF_ID_SIMPLE == $uiConfId)
      {
        return KalturaHelpers::getKalturaServerUrl() . "/kupload/ui_conf_id/" . $uiConfId;
      }
      else
      {
        return KalturaHelpers::getKalturaServerUrl() . "/kcw/ui_conf_id/" . $uiConfId;
      }
    }
    else
      return KalturaHelpers::getKalturaServerUrl() . "/kcw/ui_conf_id/" . KALTURASETTINGS_CW_UICONF_ID;
  }

  function getSimpleEditorUrl($uiConfId = null)
  {
    if ($uiConfId)
      return KalturaHelpers::getKalturaServerUrl() . "/kse/ui_conf_id/" . $uiConfId;
    else
      return KalturaHelpers::getKalturaServerUrl() . "/kse/ui_conf_id/" . KALTURASETTINGS_SE_UICONF_ID;
  }

  function getAdvancedEditorUrl($uiConfId = null)
  {
    if ($uiConfId)
      return KalturaHelpers::getKalturaServerUrl() . "/kae/ui_conf_id/" . $uiConfId;
    else
      return KalturaHelpers::getKalturaServerUrl() . "/kae/ui_conf_id/" . KALTURASETTINGS_AE_UICONF_ID;
  }

  function getThumbnailUrl($widgetId = null, $entryId = null, $width = 240, $height= 180)
  {
    $config = KalturaHelpers::getServiceConfiguration();
    $url = KalturaHelpers::getKalturaServerUrl();
    $url .= "/p/" . $config->partnerId;
    $url .= "/sp/" . $config->subPartnerId;
    $url .= "/thumbnail";
    if ($widgetId)
      $url .= "/widget_id/" . $widgetId;
    else if ($entryId)
      $url .= "/entry_id/" . $entryId;
    $url .= "/width/" . $width;
    $url .= "/height/" . $height;
    $url .= "/type/2";
    $url .= "/bgcolor/000000";
    return $url;
  }


  /**
   * Initialise variables for the config object
   * @return unknown_type
   */
  static function getServiceConfiguration() {
    $partnerId = variable_get('kaltura_partner_id', 0);

    if ($partnerId == '') $partnerId = 0;
    $subPartnerId = variable_get('kaltura_subp_id', 0);
    if ($subPartnerId == '') $subPartnerId = 0;

    $config = new KalturaConfiguration();
    $k_helpers = new KalturaHelpers();
    $config->serviceUrl = $k_helpers->getKalturaServerUrl();
    $config->subPartnerId = $subPartnerId;
    $config->partnerId = $partnerId;
    $config->curlTimeout = 60;

    //$config->setLogger(new KalturaLogger());
    return $config;
  }

  /**
   * get the url of the server either from drupal or the settings file
   * @return unknown_type
   */
  function getKalturaServerUrl() {
    $url = variable_get('kaltura_server_url', KALTURASETTINGS_SERVER_URL);
    $url = $url ? rtrim($url, '/') : rtrim(KALTURASETTINGS_SERVER_URL, '/');
    if ($GLOBALS['is_https']) {
      $url = preg_replace('/^\/\//', 'https://', $url, 1);
    }
    else {
      $url = preg_replace('/^\/\//', 'http://', $url, 1);
    }
    return $url;
  }

  /**
   * CMAC
   * gets the username and id of the current drupal user
   * change: replaced KalturaSessionUser with KalturaUser object
   * TODO: add more variables to the kaltura user object
   * @return KalturaUser object
   */
  function getSessionUser() {
    global $user;

    $kalturaUser = new KalturaUser();

    if ($user->uid) {
      $kalturaUser->id= $user->uid;
      $kalturaUser->screenName = $user->name;
      $kalturaUser->email = $user->mail;

    }
    else
    {
      $kalturaUser->id = KALTURASETTINGS_ANONYMOUS_USER_ID;
    }

    return $kalturaUser;
  }

  /**
   * oferc
   * @return: the list of players defined for the account
   */
  function getSitePlayers(&$arr)
  {
    static $players;

    $arr['48501'] = array('name' => 'Light', 'width' => 0, 'height' => 0);
    $arr['48502'] = array('name' => 'Dark', 'width' => 0, 'height' => 0);

    if (empty($players))
    {
      try {
        $players = array();
        $k_helpers = new KalturaHelpers();
        $client = $k_helpers->getKalturaClient(true);
        $listResponse = $client->uiConf->listAction();
        for ($i=0; $i < $listResponse->totalCount; $i++)
        {
          if ($listResponse->objects[$i]->objType == KalturaUiConfObjType::PLAYER)
          {
            //Don't show playlist as regular player
            if (stristr($listResponse->objects[$i]->tags, "playlist") != FALSE)
            {
              continue;
            }
            $arr[$listResponse->objects[$i]->id] = array(
              'name'   => $listResponse->objects[$i]->name,
              'width'  => $listResponse->objects[$i]->width,
              'height' => $listResponse->objects[$i]->height
            );
            $players[$listResponse->objects[$i]->id] = array(
              'name'   => $listResponse->objects[$i]->name,
              'width'  => $listResponse->objects[$i]->width,
              'height' => $listResponse->objects[$i]->height
            );
          }
        }
      }
      catch (Exception $e) {
        watchdog_exception('kaltura', $e);
      }
    }
    else
    {
      foreach ($players as $key => $sitePlayer)
      {
        $arr[$key] = $sitePlayer;
      }
    }
  }

  /**
   * oferc
   * this method is defined just for clearence, acctualy it is the same as regular players
   * @return: the list of players defined for the account
   */

  function getSitePlaylistPlayers(&$arr)
  {
    $arr['1292302'] = array('name' => 'Playlist', 'width' => 0, 'height' => 0);
    return KalturaHelpers::getSitePlayers($arr);
  }

  function getKalturaClient($isAdmin = false, $privileges = null)
  {
    // get the configuration to use the kaltura client
    $kalturaConfig = KalturaHelpers::getServiceConfiguration();

    if (!$privileges) $privileges = 'edit:*';
    // inititialize the kaltura client using the above configurations
    $kalturaClient = new KalturaClient($kalturaConfig);
    // get the current logged in user
    $k_helpers = new KalturaHelpers();
    $sessionUser = $k_helpers->getSessionUser();


    // get the variables requireed to start a session
    $partnerId = variable_get('kaltura_partner_id', '');
    $secret = variable_get('kaltura_secret', '');
    $adminSecret = variable_get('kaltura_admin_secret', '');


    if ($isAdmin)
    {
      $result = $kalturaClient->session->start($adminSecret, $sessionUser->id, KalturaSessionType::ADMIN, $partnerId, 86400, $privileges);
    }
    else
    {
      $result = $kalturaClient->session->start($secret, $sessionUser->id, KalturaSessionType::USER, $partnerId, 86400, $privileges);
    }
    $len = strlen($result);
    /** proper method for error checking please
    if ($len!=116)
    {
      watchdog("kaltura", $result );
      return null;
    }else {
     */
    // set the session so we can use other service methods
    $kalturaClient->setKs($result);
    //}

    return $kalturaClient;
  }

  function uploadFile ($isAdmin = TRUE, $file, $name) {
    try {
      $kaltura_client = KalturaHelpers::getKalturaClient($isAdmin);
      $session_user = KalturaHelpers::getSessionUser();

      $token = $kaltura_client->baseEntry->upload($file);
      $entry = new KalturaBaseEntry();
      $entry ->name = $name;
      $res = $kaltura_client->baseEntry->addFromUploadedFile($entry, $token, NULL);
    }
    catch(Exception $ex) {
      $res = $ex->getMessage();
    }
    return $res;
  }

  function hasMobileFlavores() {
    try {
      $kaltura_client = KalturaHelpers::getKalturaClient(TRUE);
      $session_user = KalturaHelpers::getSessionUser();
      $filter = new KalturaPermissionFilter();
      // Class KalturaPermissionName don't exist in later versions of client
      // library, so just replace the needed property with its value.
      $filter->nameEqual = 'FEATURE_MOBILE_FLAVORS';
      // create a permission service object
      $permissionService = new KalturaPermissionService($kaltura_client);
      $res = $permissionService->listAction($filter);
      if ($res->totalCount == 1) { // expected only one permission or no permissions at all
        if($res->objects[0]->status == KalturaPermissionStatus::ACTIVE) {
          return TRUE;
        }
      }
    }
    catch (Exception $e) {
      watchdog_exception('kaltura', $e);
    }

    return FALSE;
  }
}


class KalturaContentCategories
{
  var $categories = array(
    'Arts & Literature',
    'Automotive',
    'Business',
    'Comedy',
    'Education',
    'Entertainment',
    'Film & Animation',
    'Gaming',
    'Howto & Style',
    'Lifestyle',
    'Men',
    'Music',
    'News & Politics',
    'Nonprofits & Activism',
    'People & Blogs',
    'Pets & Animals',
    'Science & Technology',
    'Sports',
    'Travel & Events',
    'Women',
  );
}
