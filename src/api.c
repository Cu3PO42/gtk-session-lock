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

#include "gtk-session-lock.h"
#include "gtk-wayland.h"
#include "custom-shell-surface.h"
#include "lock-surface.h"

#include "ext-session-lock-v1-client.h"

#include <gdk/gdkwayland.h>

#pragma GCC visibility push(protected)

guint
gtk_session_lock_get_major_version ()
{
    return GTK_SESSION_LOCK_MAJOR;
}

guint
gtk_session_lock_get_minor_version ()
{
    return GTK_SESSION_LOCK_MINOR;
}

guint
gtk_session_lock_get_micro_version ()
{
    return GTK_SESSION_LOCK_MICRO;
}

gboolean
gtk_session_lock_is_supported ()
{
    if (!GDK_IS_WAYLAND_DISPLAY (gdk_display_get_default ()))
        return FALSE;
    gtk_wayland_init_if_needed ();
    return gtk_wayland_get_session_lock_manager_global () != NULL;
}

guint
gtk_session_lock_get_protocol_version ()
{
    if (!GDK_IS_WAYLAND_DISPLAY (gdk_display_get_default ()))
        return 0;
    gtk_wayland_init_if_needed ();
    struct ext_session_lock_manager_v1 *lock_manager = gtk_wayland_get_session_lock_manager_global ();
    if (!lock_manager)
        return 0;
    return ext_session_lock_manager_v1_get_version (lock_manager);
}

static LockSurface*
gtk_window_get_lock_surface (GtkWindow *window)
{
    g_return_val_if_fail (window, NULL);
    CustomShellSurface *shell_surface = gtk_window_get_custom_shell_surface (window);
    if (!shell_surface) {
        g_critical ("GtkWindow is not a lock surface. Make sure you called gtk_session_lock_init_for_window ()");
        return NULL;
    }
    LockSurface *lock_surface = custom_shell_surface_get_lock_surface (shell_surface);
    if (!lock_surface) {
        g_critical ("Custom wayland shell surface is not a lock surface");
        return NULL;
    }
    return lock_surface;
}

gboolean
gtk_session_lock_is_lock_window (GtkWindow *window)
{
    g_return_val_if_fail (window, FALSE);
    CustomShellSurface *shell_surface = gtk_window_get_custom_shell_surface (window);
    if (!shell_surface)
        return FALSE;
    LockSurface *lock_surface = custom_shell_surface_get_lock_surface (shell_surface);
    return lock_surface != NULL;
}

struct ext_session_lock_surface_v1 *
gtk_session_lock_get_lock_surface (GtkWindow *window)
{
    LockSurface *lock_surface = gtk_window_get_lock_surface (window);
    if (!lock_surface) return NULL; // Error message already shown in gtk_window_get_lock_surface
    return lock_surface->lock_surface;
}

void
gtk_session_lock_unmap_lock_window (GtkWindow *window)
{
    g_return_if_fail (window);
    CustomShellSurface *shell_surface = gtk_window_get_custom_shell_surface (window);
    if (!shell_surface)
        return;
    custom_shell_surface_unmap (shell_surface);
}
