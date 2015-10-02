<?php
/**
 * @file
 * featutres_actualite.features.inc
 */

/**
 * Implements hook_node_info().
 */
function featutres_actualite_node_info() {
  $items = array(
    'actualit_s' => array(
      'name' => t('Actualités'),
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
