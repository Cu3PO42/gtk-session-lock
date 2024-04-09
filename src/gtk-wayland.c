/* This entire file is licensed under MIT
 *
 * Copyright 2020 Sophie Winter
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "gtk-wayland.h"

#include "custom-shell-surface.h"
#include "gtk-priv-access.h"

#include "ext-session-lock-v1-client.h"

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkwayland.h>

static struct wl_registry *wl_registry_global = NULL;
static struct ext_session_lock_manager_v1 *lock_manager_global = NULL;

static gboolean has_initialized = FALSE;

gboolean
gtk_wayland_get_has_initialized (void)
{
    return has_initialized;
}

struct ext_session_lock_manager_v1 *
gtk_wayland_get_session_lock_manager_global ()
{
    return lock_manager_global;
}

static void
wl_registry_handle_global (void *_data,
                           struct wl_registry *registry,
                           uint32_t id,
                           const char *interface,
                           uint32_t version)
{
    (void)_data;

    if (strcmp (interface, ext_session_lock_manager_v1_interface.name) == 0) {
        g_warn_if_fail (ext_session_lock_manager_v1_interface.version >= 1);
        lock_manager_global = wl_registry_bind (registry,
                                                id,
                                                &ext_session_lock_manager_v1_interface,
                                                MIN((uint32_t)ext_session_lock_manager_v1_interface.version, version));
    }
}

static void
wl_registry_handle_global_remove (void *_data,
                                  struct wl_registry *_registry,
                                  uint32_t _id)
{
    (void)_data;
    (void)_registry;
    (void)_id;

    // TODO
}

static const struct wl_registry_listener wl_registry_listener = {
    .global = wl_registry_handle_global,
    .global_remove = wl_registry_handle_global_remove,
};

void
gtk_wayland_init_if_needed ()
{
    if (has_initialized)
        return;

    GdkDisplay *gdk_display = gdk_display_get_default ();
    g_return_if_fail (gdk_display);
    g_return_if_fail (GDK_IS_WAYLAND_DISPLAY (gdk_display));

    struct wl_display *wl_display = gdk_wayland_display_get_wl_display (gdk_display);
    wl_registry_global = wl_display_get_registry (wl_display);
    wl_registry_add_listener (wl_registry_global, &wl_registry_listener, NULL);
    wl_display_roundtrip (wl_display);

    if (!lock_manager_global)
        g_warning ("It appears your Wayland compositor does not support the Session Lock protocol");

    has_initialized = TRUE;
}
