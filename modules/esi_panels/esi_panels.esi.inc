<?php
/**
 * @file
 * ESI prepare, render and flush hooks.
 */

/**
 * Prepare an ESI panels pane for rendering.
 * Defined in hook_esi_component_info().
 *
 * @see esi_panels_esi_component_info()
 */
function esi_panels__esi_pane_prepare($component_key) {
  // If the component key contains "::", it has addressable content in it.
  if (strpos($component_key, "::") === FALSE) {
    return FALSE;
  }
  else {
    $matches = array();
    if (preg_match("/(.*?):(.*)/", $component_key, $matches)) {
      list($component_key, $theme, $address) = $matches;
    }
  }

  $args = array_slice(func_get_args(), 1);

  // Grab additional info if it is needed.
  $url = array_pop($args);

  $pane = new stdClass();
  $pane->esi_meta = array(
    'address' => $address,
    'theme' => $theme,
    'url' => base64_decode($url),
  );

  // Allow other modules to alter the context information here.
  // A certain implementation might adjust the addressable string to add extra
  // args or do something similar.
  // @see hook_esi_block_context_alter()
  drupal_alter('esi_panels_pane_context', $pane);

  esi_panels__restore_context($pane);

  return $pane;
}

/**
 * Restore the original context that was used when a block was displayed.
 */
function esi_panels__restore_context(&$pane) {
  // Restore the theme.
  global $theme;
  $theme = $pane->esi_meta['theme'];

  $_GET['q'] = $pane->esi_meta['url'];
  $_SERVER['REQUEST_URI'] = $pane->esi_meta['url'];
  drupal_static_reset('drupal_get_destination');

  // Load the actual pane info through the addressable content system.
  $pane_meta = $pane->esi_meta;
  ctools_include('content');
  if ($pane = ctools_get_addressable_content($pane_meta['address'], 'pane')) {
    $pane->esi_meta = $pane_meta;
  }
}

/**
 * Render the HTML for a single pane.
 * Defined in hook_esi_component_info().
 *
 * @see esi_panels_esi_component_info()
 */
function esi_panels__esi_pane_render($pane) {
  // Set HTTP headers.
  esi_panels_set_http_headers($pane);

  // Use addressable content exclusively.
  if ($address = $pane->esi_meta['address']) {
    ctools_include('content');
    $content = ctools_get_addressable_content($address);
  }

  // CTools will return an empty string if any of a number of errors occur.
  if (empty($content)) {
    return '';
  }

  return $content;
}

/**
 * Set HTTP headers to control caching of ESI fragments.
 */
function esi_panels_set_http_headers($pane) {
  $ttl = $pane->cache['settings']['esi_ttl'];

  $headers = array();
  $headers[] = array('Cache-Control', "private, max-age=$ttl");

  // Allow other modules to alter the headers.
  // @see hook_esi_block_cache_headers_alter().
  drupal_alter('esi_panels_cache_headers', $headers);

  foreach ($headers as $header) {
    drupal_add_http_header($header[0], $header[1]);
  }
}

/**
 * Flush the ESI pane caches.
 * Defined in hook_esi_component_info().
 *
 * @see esi_panels_esi_component_info()
 */
function esi_panels__esi_pane_flush() {
  // @TODO.
}
