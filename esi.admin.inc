<?php
/**
 * @file
 * Admin integration for the ESI module.
 */

/**
 * Callback for the admin_menu module which clears all ESI caches.
 */
function esi_admin_menu_flush_cache() {
  // @TODO: Check this integrates with varnish/expire properly: this should
  // expire *all* known external caches.
  cache_clear_all('esi:', 'cache_page', TRUE);

  drupal_set_message(t('Edge-side-include caches cleared.'));

  // The menu callback actions the cache-clear request, but doesn't display a
  // page.  Ensure the user is redirected back to a suitable page.
  drupal_goto();
}
