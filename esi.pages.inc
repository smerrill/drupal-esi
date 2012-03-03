<?php
/**
 * @file
 * Delivery handlers for the ESI module.
 */

/**
 * Menu callback to handle an ESI component.
 *
 * @see esi_component_load().
 */
function esi_handle_component($component) {
  // The menu wildcard loader will return NULL for invalid components, so that
  // the menu-handler will delegate 404 delivery here.
  if (empty($component)) {
    esi_fast_404();
  }

  // Remove the component from the arguments.
  $args = array_slice(func_get_args(),1);

  // Load in the include file if provided.
  if (!empty($component['file'])) {
    $filepath = $component['filepath'] . '/' . $component['file'];
    if (file_exists($filepath)) {
      include_once($filepath);
    }
  }

  // Allow modules to preproccess the request, set up context, etc.
  // Any arguments returned by the preprocess handler are passed to the render
  // handler.
  if (isset($component['preprocess'])) {
    $result = call_user_func_array($component['preprocess'], $args);
    $args = (is_array($result)) ? $result : array($result);
  }

  // Get the renderable content of the component.
  $content = call_user_func_array($component['render'], $args);

  // Ensure the content is a renderable array (even if a string was returned).
  $content_block = (is_array($content) ? $content : array('#markup' => $content));

  return $content_block;
}

/**
 * Minimal delivery for an ESI component. Replaces drupal_deliver_html_page().
 *
 * @see drupal_deliver_page()
 * @see drupal_deliver_html_page()
 */
function esi_deliver_esi_component($esi_rendered_component) {
  if (isset($esi_rendered_component) && is_null(drupal_get_http_header('Content-Type'))) {
    drupal_add_http_header('Content-Type', 'text/html; charset=utf-8');
  }

  // Send appropriate HTTP-Header for browsers and search engines.
  global $language;
  drupal_add_http_header('Content-Language', $language->language);

  // Allow other modules to alter the result of the ESI component.
  drupal_alter('esi_rendered_component', $esi_rendered_component);

  if (isset($esi_rendered_component)) {
    print drupal_render($esi_rendered_component);
  }

  // Perform end-of-request tasks.
  // Even though it's just an ESI component, invoke in case there's any session
  // activity to commit.
  drupal_page_footer();
}

/**
 * Generate a fast 404 (consisting of a single HTML comment).
 * Full delivery is not desired, because it would typically result in embedding
 * a complete, themed Drupal 404 page *within* another page.
 *
 * @see drupal_fast_404()
 */
function esi_fast_404() {
  drupal_add_http_header('Status', '404 Not Found');
  drupal_add_http_header('X-ESI', 'Component not recognised');
  watchdog('page not found', check_plain($_GET['q']), NULL, WATCHDOG_WARNING);

  // Unlike drupal_fast_404(), the HTML furniture (html tag, head tag, etc) are
  // not delivered 
  echo "<!-- esi_fast_404 -->\n";

  // Unlike normal page-views, invoking hook_exit() is unneccessary overhead.
  // This use of exit matches the behaviour of drupal_fast_404().
  exit;
}
