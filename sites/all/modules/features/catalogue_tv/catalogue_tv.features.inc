<?php
/**
 * @file
 * catalogue_tv.features.inc
 */

/**
 * Implements hook_node_info().
 */
function catalogue_tv_node_info() {
  $items = array(
    'saisons' => array(
      'name' => t('Saisons'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'series' => array(
      'name' => t('Séries'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'video_local' => array(
      'name' => t('Video Local'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
