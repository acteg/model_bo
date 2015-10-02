<?php

/**
 * @file
 * Hooks provided by Kaltura module.
 */

/**
 * Returns the URL the user will be redirected to "after AddEntry in the CW".
 *
 * Only first module implemented this hook will be counted.
 *
 * @return string
 *   A string containing a URL.
 *
 * @see kaltura_cw_destination()
 *
 * @todo Provide a better description.
 */
function hook_kaltura_cw_destination() {
  return url('kaltura/entries');
}

/**
 * Executes any additional tasks after "notification is received".
 *
 * @param array $notification_data
 *   Associative array as it is contained in each element of $data property of
 *   object of KalturaNotificationClient class. Keys include:
 *   - notification_id.
 *   - notification_type.
 *
 * @see kaltura_notification_handler()
 * @see \KalturaNotificationClient
 *
 * @todo Provide a better description.
 * @todo Describe all possible elements of $notification_data array.
 */
function hook_kaltura_notification_handler($notification_data) {
  // @todo Add an example.
}
