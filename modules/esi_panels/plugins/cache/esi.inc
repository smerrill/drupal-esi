<?php
/**
 * @file
 * ESI cache plugin.  Substitutes esi-tags for a panel-pane's content.
 */

// Plugin definition
$plugin = array(
  'title' => t("ESI"),
  'description' => t('ESI caching is a proxy-based cache. Panes are replaced by <esi> tags and requested separately by the proxy.'),
  'cache get'   => 'esi_panels_esi_cache_get_cache',
  'cache set'   => 'esi_panels_esi_cache_set_cache',
  'cache clear' => 'esi_panels_esi_cache_clear_cache',
  'settings form' => 'esi_panels_esi_cache_settings_form',
  'settings form submit' => 'esi_panels_esi_cache_settings_form_submit',
  'defaults' => array(
    // // TODO: review settings.
    // 'max_age' => (int)variable_get('esi_panels_esi_panels_default_max_age', ESI_PANEL_DEFAULT_MAX_AGE),
    // 'scope' => (int)variable_get('esi_panels_esi_panels_default_scope', ESI_PANEL_DEFAULT_SCOPE),
  ),
);

