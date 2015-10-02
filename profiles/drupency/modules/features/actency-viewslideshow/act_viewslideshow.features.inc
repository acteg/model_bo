<?php
/**
 * @file
 * act_viewslideshow.features.inc
 */

/**
 * Implements hook_views_api().
 */
function act_viewslideshow_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_image_default_styles().
 */
function act_viewslideshow_image_default_styles() {
  $styles = array();

  // Exported image style: slider_large.
  $styles['slider_large'] = array(
    'name' => 'slider_large',
    'label' => 'Slider_large',
    'effects' => array(
      1 => array(
        'label' => 'Échelle',
        'help' => 'La mise à l\'échelle maintiendra les proportions originales de l\'image. Si une seule dimension est précisée, l\'autre dimension sera calculée automatiquement.',
        'effect callback' => 'image_scale_effect',
        'dimensions callback' => 'image_scale_dimensions',
        'form callback' => 'image_scale_form',
        'summary theme' => 'image_scale_summary',
        'module' => 'image',
        'name' => 'image_scale',
        'data' => array(
          'width' => 300,
          'height' => 480,
          'upscale' => 0,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: slider_popin_large.
  $styles['slider_popin_large'] = array(
    'name' => 'slider_popin_large',
    'label' => 'Slider_popin_large',
    'effects' => array(
      5 => array(
        'label' => 'Redimensionner',
        'help' => 'Le redimensionnement ne respecte pas les proportions de l\'image. Ce qui peut se traduire par des images étirées ou compressées.',
        'effect callback' => 'image_resize_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'image',
        'name' => 'image_resize',
        'data' => array(
          'width' => 600,
          'height' => 400,
        ),
        'weight' => 2,
      ),
    ),
  );

  return $styles;
}
