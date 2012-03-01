<?php
/**
 * @file
 * API documentation for hooks defined by the ESI module.
 */

/**
 * Declare a handler for delivering content through ESI.
 */
function hook_esi_component() {
  return array(
    'block' => array(
      'preprocess' => 'esi_block__esi_block_prepare',
      'render'     => 'esi_block__esi_block_render',
      'flush'      => 'esi_block__esi_block_flush',
      'file'       => 'esi_block.esi.inc',
    ),
  );
}

/**
 * Alter the ESI handlers.
 *
 * @param Array $esi_component_info
 * Array of ESI component handlers.
 */
function hook_esi_component_info_alter(&$esi_component_info) {
  // Change the block handler to use a custom renderer.
  $esi_component_info['block']['render'] = 'my_custom_fast_renderer';
}

/**
 * Alter the rendered ESI component.
 *
 * @param Array $esi_rendered_component
 * An ESI component, delivered as a renderable array.
 */
function hook_esi_rendered_component_alter(&$esi_rendered_component) {
  $key = key($esi_rendered_component);
  // Add another theme wrapper.
  $esi_rendered_component[$key]['#theme_wrappers'][] = 'custom_rounded_corners';
}
