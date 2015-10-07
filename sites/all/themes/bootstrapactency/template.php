<?php

/**
 * @file
 * template.php
 */


function bootstrapactency_preprocess_page(&$vars) {
  /**
   * Special Front page ()
   */
  if (request_uri() === "/") {
    //$vars['theme_hook_suggestions'][] = "page__frontpage";
  }



}
