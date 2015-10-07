<?php

/**
 * @file
 * page.vars.php
 */

/**
 * Implements hook_preprocess_page().
 *
 * @see page.tpl.php
 */
function mytheme_preprocess_node(&$variables) {
    //dpm($variables['view']->current_display);
    if ($variables['is_front']) {
        //$variables['theme_hook_suggestions'][] = 'node__front'; // node__view__contents__block_2
        //dpm($variables['theme_hook_suggestions']);
    }


    if (isset($variables['type']) && $variables['type'] == 'group') {
        $variables['submitted'] = "";
    }

    if (isset($variables['view']->current_display) && $variables['view']->current_display == "block_2" && $variables['is_front']) {
        //dpm($variables);
        //dpm("|" . current_path() . "|");
        //dpm($variables);
        $variables['submitted'] = "";
        if (isset($variables['og_group_ref']['und'][0]['target_id'])) {
            $gid = $variables['og_group_ref']['und'][0]['target_id'];
            $node = node_load($gid);
            if (isset($node->title)) {
                $name = l($node->title, "node/$gid");
                $variables['submitted'] = t("Submitted by !username on !datetime", array('!username' => $name, '!datetime' => $variables['date']));
            }
        }
    }


    //
    //Submitted by <a href="/en/users/admin" title="View user profile." class="username" xml:lang="" about="/en/users/admin" typeof="sioc:UserAccount" property="foaf:name" datatype="">Admin</a> on Wed, 09/02/2015 - 14:40
    // 
    // // Add information about the number of sidebars.
//  if (!empty($variables['page']['sidebar_first']) && !empty($variables['page']['sidebar_second'])) {
//    $variables['content_column_class'] = ' class="col-sm-6"';
//  }
//  elseif (!empty($variables['page']['sidebar_first']) || !empty($variables['page']['sidebar_second'])) {
//    $variables['content_column_class'] = ' class="col-sm-9"';
//  }
//  else {
//    $variables['content_column_class'] = ' class="col-sm-12"';
//  }
//
//  if(bootstrap_setting('fluid_container') === 1) {
//    $variables['container_class'] = 'container-fluid';
//  }
//  else {
//    $variables['container_class'] = 'container';
//  }
//
//  // Primary nav.
//  $variables['primary_nav'] = FALSE;
//  if ($variables['main_menu']) {
//    // Build links.
//    $variables['primary_nav'] = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
//    // Provide default theme wrapper function.
//    $variables['primary_nav']['#theme_wrappers'] = array('menu_tree__primary');
//  }
//
//  // Secondary nav.
//  $variables['secondary_nav'] = FALSE;
//  if ($variables['secondary_menu']) {
//    // Build links.
//    $variables['secondary_nav'] = menu_tree(variable_get('menu_secondary_links_source', 'user-menu'));
//    // Provide default theme wrapper function.
//    $variables['secondary_nav']['#theme_wrappers'] = array('menu_tree__secondary');
//  }
//
//  $variables['navbar_classes_array'] = array('navbar');
//
//  if (bootstrap_setting('navbar_position') !== '') {
//    $variables['navbar_classes_array'][] = 'navbar-' . bootstrap_setting('navbar_position');
//  }
//  elseif(bootstrap_setting('fluid_container') === 1) {
//    $variables['navbar_classes_array'][] = 'container-fluid';
//  }
//  else {
//    $variables['navbar_classes_array'][] = 'container';
//  }
//  if (bootstrap_setting('navbar_inverse')) {
//    $variables['navbar_classes_array'][] = 'navbar-inverse';
//  }
//  else {
//    $variables['navbar_classes_array'][] = 'navbar-default';
//  }
}

/**
 * Implements hook_process_page().
 *
 * @see page.tpl.php
 */
//function bootstrap_process_page(&$variables) {
//  $variables['navbar_classes'] = implode(' ', $variables['navbar_classes_array']);
//}
