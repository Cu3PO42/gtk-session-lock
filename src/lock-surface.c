#include "lock-surface.h"

#include "gtk-session-lock.h"
#include "custom-shell-surface.h"
#include "gtk-wayland.h"

#include "ext-session-lock-v1-client.h"

#include <gtk/gtk.h>
#include <gdk/gdkwayland.h>

/*
 * Sets the window's geometry hints (used to force the window to be a specific size)
 * Needs to be called whenever last_configure_size are changed
 * Forces window (by setting the max and min hints) to be of the size of the last .configure event
 */
static void
lock_surface_update_size (LockSurface *self)
{
    GtkWindow *gtk_window = custom_shell_surface_get_gtk_window ((CustomShellSurface *)self);

    gint width = self->last_configure_size.width;
    gint height = self->last_configure_size.height;

    GdkGeometry hints;
    hints.min_width = width;
    hints.max_width = width;
    hints.min_height = height;
    hints.max_height = height;

    gtk_window_set_geometry_hints (gtk_window,
                                   NULL,
                                   &hints,
                                   GDK_HINT_MIN_SIZE | GDK_HINT_MAX_SIZE);
}

static void
lock_surface_handle_configure (void *data,
                               struct ext_session_lock_surface_v1 *surface,
                               uint32_t serial,
                               uint32_t w,
                               uint32_t h)
{
    LockSurface *self = data;

    self->last_configure_size = (GtkRequisition) {
        .width = (gint)w,
        .height = (gint)h,
    };

    lock_surface_update_size (self);

    ext_session_lock_surface_v1_ack_configure (surface, serial);

    if (!self->first_commit_done)
    {
        GtkWindow *gtk_window = custom_shell_surface_get_gtk_window(&self->super);
        GdkWindow *gdk_window = gtk_widget_get_window (GTK_WIDGET (gtk_window));

        gdk_window_thaw_updates(gdk_window);
        self->first_commit_done = true;
    }
}

static const struct ext_session_lock_surface_v1_listener lock_surface_listener = {
    .configure = lock_surface_handle_configure,
};

static void
lock_surface_map (CustomShellSurface *super, struct wl_surface *wl_surface)
{
    LockSurface *self = (LockSurface *)super;

    g_return_if_fail (!self->lock_surface);

    struct ext_session_lock_v1 *session_lock = self->session_lock;
    g_return_if_fail (session_lock);

    struct wl_output *output = gdk_wayland_monitor_get_wl_output (self->monitor);

    GtkWindow *gtk_window = custom_shell_surface_get_gtk_window(super);
    GdkWindow *gdk_window = gtk_widget_get_window (GTK_WIDGET (gtk_window));
    gdk_window_freeze_updates(gdk_window);

    self->lock_surface = ext_session_lock_v1_get_lock_surface (session_lock,
                                                               wl_surface,
                                                               output);
    g_return_if_fail (self->lock_surface);

    ext_session_lock_surface_v1_add_listener (self->lock_surface, &lock_surface_listener, self);
}

static void
lock_surface_unmap (CustomShellSurface *super)
{
    LockSurface *self = (LockSurface *)super;

    if (self->lock_surface) {
        ext_session_lock_surface_v1_destroy (self->lock_surface);
        self->lock_surface = NULL;
    }
}

static void
lock_surface_finalize (CustomShellSurface *super)
{
    LockSurface *self = (LockSurface *)super;
    lock_surface_unmap (super);
}

static const CustomShellSurfaceVirtual lock_surface_virtual = {
    .map = lock_surface_map,
    .unmap = lock_surface_unmap,
    .finalize = lock_surface_finalize,
};

LockSurface *
lock_surface_new (GtkWindow *gtk_window, GdkMonitor *monitor, struct ext_session_lock_v1 *session_lock)
{
    g_return_val_if_fail (monitor, NULL);

    g_return_val_if_fail (session_lock, NULL);
    LockSurface *self = g_new0 (LockSurface, 1);
    self->super.virtual = &lock_surface_virtual;
    custom_shell_surface_init ((CustomShellSurface *)self, gtk_window);

    self->current_allocation = (GtkRequisition) {
        .width = 0,
        .height = 0,
    };
    self->last_configure_size = self->current_allocation;
    self->monitor = monitor;
    self->session_lock = session_lock;
    self->lock_surface = NULL;
    self->first_commit_done = false;

    return self;
}

LockSurface *
custom_shell_surface_get_lock_surface (CustomShellSurface *shell_surface)
{
    if (shell_surface && shell_surface->virtual == &lock_surface_virtual)
        return (LockSurface *)shell_surface;
    else
        return NULL;
}
