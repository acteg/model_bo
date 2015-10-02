<?php

/**
 * @file
 * Displays embed object of the Kaltura player.
 *
 * Available variables:
 * - $id: Object HTML ID.
 * - $height: Player height.
 * - $width: Player width.
 * - $src: URL to a SWF file.
 * - $delivery: Flash vars.
 * - $thumb_url: URL to a thumbnail file.
 * - $title: Object title.
 * - $description: Object description.
 *
 * @see template_preprocess_kaltura_embed_player()
 *
 * @ingroup themeable
 */
?>
<object id="<?php print $id; ?>"
        name="kaltura_player"
        class="KalturaDynamicPlayer"
        type="application/x-shockwave-flash"
        allowFullScreen="true"
        allowNetworking="all"
        allowScriptAccess="always"
        height="<?php print $height; ?>"
        width="<?php print $width; ?>"
        xmlns:dc="http://purl.org/dc/terms/"
        xmlns:media="http://search.yahoo.com/searchmonkey/media/"
        rel="media:video"
        resource="<?php print $src; ?>"
        data="<?php print $src; ?>">
  <param name="allowFullScreen" value="true" />
  <param name="allowNetworking" value="all" />
  <param name="allowScriptAccess" value="always" />
  <param name="bgcolor" value="#000000" />
  <param name="flashVars" value="<?php print $delivery; ?>" />
  <param name="flashVars" value="&{FLAVOR}" />
  <param name="movie" value="<?php print $src; ?>" />
  <a rel="media:thumbnail" href="<?php print $thumb_url; ?>"></a>
  <span property="dc:description" content="<?php print $description; ?>"></span>
  <span property="media:title" content="<?php print $title; ?>"></span>
  <span property="media:width" content="<?php print $width; ?>"></span>
  <span property="media:height" content="<?php print $height; ?>"></span>
  <span property="media:type" content="application/x-shockwave-flash"></span>
</object>
