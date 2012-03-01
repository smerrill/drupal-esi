<?php
/**
 * @file
 * Delivery handlers for the ESI module.
 */

/**
 * Menu callback to handle an ESI component.
 */
function esi_handle_component($component) {
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
