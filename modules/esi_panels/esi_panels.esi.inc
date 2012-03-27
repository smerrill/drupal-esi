<?php
/**
 * ESI prepare, render and flush hooks.
 */


/**
 * Prepare an ESI panels pane for rendering.
 * Defined in hook_esi_component_info().
 *
 * @see esi_panels_esi_component_info().
 */
function esi_panels__esi_pane_prepare($component_key) {
  // Use the parameters to reconstruct:
  // - Display
  // - Pane
  // - Contexts

  // @TODO: Hack hack hacky.
  $GLOBALS['esi_panels_esi_request'] = TRUE;

  $theme = $component_key;
  $args = array_slice(func_get_args(), 1);
  // Grab the address from the args. (This will include extra args!)
  $address = array_pop($args);

  // Grab additional info if it is needed.
  $url = array_pop($args);

  $pane = new stdClass();
  $pane->esi_meta = array(
    'address' => $address,
    'theme' => $theme,
    'url' => base64_decode($url),
  );

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
}

/**
 * Render the HTML for a single pane.
 * Defined in hook_esi_component_info().
 *
 * @see esi_panels_esi_component_info()
 */
function esi_panels__esi_pane_render($pane) {
  // Much of this is from the "standard" display renderer:
  // see panels_renderer_standard::prepare().
  ctools_include('content');
  $content = ctools_get_addressable_content($pane->address);

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

  foreach($headers as $header) {
    drupal_add_http_header($header[0], $header[1]);
  }
}

/**
 * Flush the ESI block caches.
 * Defined in hook_esi_component_info().
 *
 * @see esi_panels_esi_component_info()
 */
function esi_panels__esi_pane_flush() {
  // @TODO.

}
