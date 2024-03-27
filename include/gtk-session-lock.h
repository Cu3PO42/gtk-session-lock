#ifndef GTK_SESSION_LOCK_H
#define GTK_SESSION_LOCK_H

#include <gtk/gtk.h>
#include <glib-object.h>

/**
 * SECTION:gtk-session-lock
 * @title: Gtk Session Lock
 * @short_description: A library to write screen lockers using GTK
 */

G_BEGIN_DECLS

/**
 * gtk_session_lock_get_major_version:
 *
 * Returns: the major version number of the GTK Session Lock library
 */
guint gtk_session_lock_get_major_version ();

/**
 * gtk_session_lock_get_minor_version:
 *
 * Returns: the minor version number of the GTK Session Lock library
 */
guint gtk_session_lock_get_minor_version ();

/**
 * gtk_session_lock_get_micro_version:
 *
 * Returns: the micro/patch version number of the GTK Session Lock library
 */
guint gtk_session_lock_get_micro_version ();

/**
 * gtk_session_lock_is_supported:
 *
 * May block for a Wayland roundtrip the first time it's called.
 *
 * Returns: %TRUE if the platform is Wayland and Wayland compositor supports the
 * ext_session_lock_v1 protocol.
 */
gboolean gtk_session_lock_is_supported ();

/**
 * gtk_session_lock_get_protocol_version:
 *
 * May block for a Wayland roundtrip the first time it's called.
 *
 * Returns: version of the ext_session_lock_v1 protocol supported by the
 * compositor or 0 if the protocol is not supported.
 */
guint gtk_session_lock_get_protocol_version ();

/**
 * gtk_session_lock_is_lock_window:
 * @window: A #GtkWindow that may or may not have a lock surface.
 *
 * Returns: if @window has been initialized as a lock surface.
 */
gboolean gtk_session_lock_is_lock_window (GtkWindow *window);

/**
 * gtk_session_lock_get_ext_session_lock_surface_v1:
 * @window: A lock surface.
 *
 * Returns: The underlying lock surface Wayland object
 */
struct ext_session_lock_surface_v1 *gtk_session_lock_get_ext_session_lock_surface_v1 (GtkWindow *window);

#define GTK_SESSION_LOCK_TYPE_LOCK (gtk_session_lock_lock_get_type ())
G_DECLARE_FINAL_TYPE(GtkSessionLockLock, gtk_session_lock_lock, GTK_SESSION_LOCK, LOCK, GObject)

/**
 * gtk_session_lock_prepare_lock:
 *
 * Prepare a new #SessionLockLock. You should connect signals to it before
 * calling its lock method.
 *
 * Returns: (transfer full): A new #SessionLockLock.
 */
GtkSessionLockLock *gtk_session_lock_prepare_lock (void);

/**
 * gtk_session_lock_lock_lock:
 *
 * Actually perform the locking operation. This should be run after connecting
 * to this object's signals.
 *
 * The compositor will hide all surfaces except those created via the
 * #gtk_session_lock_lock_new_surface method.
 */
void gtk_session_lock_lock_lock (GtkSessionLockLock *lock);

/**
 * gtk_session_lock_lock_destroy:
 *
 * Destory an inactive lock object. You should call this method only after
 * receiving the finished signal.
 */
void gtk_session_lock_lock_destroy (GtkSessionLockLock *lock);

/**
 * gtk_session_lock_lock_unlock_and_destroy:
 *
 * Unlock an active session lock and dispose of it. It cannot be reused.
 * You should generally call this method only after verifying a user's
 * identity.
 */
void gtk_session_lock_lock_unlock_and_destroy (GtkSessionLockLock *lock);

/**
 * gtk_session_lock_lock_new_surface:
 *
 * This method must be called after gtk_session_lock_lock_lock. If the session
 * is locked successfully, the specified window will be shown on the given
 * monitor. You must only ever call this method once for a given lock and
 * monitor. The window will automatically be stretched to cover the entire
 * screen.
 */
void gtk_session_lock_lock_new_surface (GtkSessionLockLock *lock, GtkWindow *gtk_window, GdkMonitor *monitor);

G_END_DECLS

#endif // GTK_SESSION_LOCK_H
