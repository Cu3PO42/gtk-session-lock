
#include "../include/gtk-session-lock.h"

#include "gtk-wayland.h"
#include "lock-surface.h"

#include "ext-session-lock-v1-client.h"

GtkSessionLockLock *gtk_session_lock_prepare_lock (void)
{
    gtk_wayland_init_if_needed ();
    struct ext_session_lock_manager_v1 *lock_manager = gtk_wayland_get_session_lock_manager_global ();

    g_return_val_if_fail (lock_manager, NULL);

    return g_object_new (GTK_SESSION_LOCK_TYPE_LOCK, "lock-manager", lock_manager, NULL);
}

// =======================================
// GtkSessionLockLock

struct _GtkSessionLockLockClass
{
    GObjectClass parent_class;
};

struct _GtkSessionLockLock
{
    GObject parent_instance;
};

typedef struct {
    struct ext_session_lock_manager_v1 *lock_manager;
    struct ext_session_lock_v1 *lock_obj;
} GtkSessionLockLockPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (GtkSessionLockLock, gtk_session_lock_lock, G_TYPE_OBJECT)

enum {
    SESSION_LOCK_LOCK_PROP_LOCK_MANAGER = 1,
    SESSION_LOCK_LOCK_PROP_LOCK_OBJ,
    SESSION_LOCK_LOCK_LAST_PROP
};

enum {
    SESSION_LOCK_LOCK_SIGNAL_LOCKED = 1,
    SESSION_LOCK_LOCK_SIGNAL_FINISHED,
    SESSION_LOCK_LOCK_LAST_SIGNAL
};

static GParamSpec *session_lock_lock_properties[SESSION_LOCK_LOCK_LAST_PROP] = { NULL, };
static guint session_lock_lock_signals[SESSION_LOCK_LOCK_LAST_SIGNAL] = { 0, };

static void
session_lock_lock_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    GtkSessionLockLock *self = GTK_SESSION_LOCK_LOCK (object);
    GtkSessionLockLockPrivate *priv = gtk_session_lock_lock_get_instance_private (self);

    switch (property_id) {
        case SESSION_LOCK_LOCK_PROP_LOCK_MANAGER:
            priv->lock_manager = g_value_get_pointer (value);
            break;
        case SESSION_LOCK_LOCK_PROP_LOCK_OBJ:
            priv->lock_obj = g_value_get_pointer (value);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
            break;
    }
}

static void
session_lock_lock_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    GtkSessionLockLock *self = GTK_SESSION_LOCK_LOCK (object);
    GtkSessionLockLockPrivate *priv = gtk_session_lock_lock_get_instance_private (self);

    switch (property_id) {
        case SESSION_LOCK_LOCK_PROP_LOCK_MANAGER:
            g_value_set_pointer (value, priv->lock_manager);
            break;
        case SESSION_LOCK_LOCK_PROP_LOCK_OBJ:
            g_value_set_pointer (value, priv->lock_obj);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
            break;
    }
}

static void
gtk_session_lock_lock_class_init (GtkSessionLockLockClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->set_property = session_lock_lock_set_property;
    object_class->get_property = session_lock_lock_get_property;

    session_lock_lock_properties[SESSION_LOCK_LOCK_PROP_LOCK_MANAGER] = g_param_spec_pointer (
        "lock-manager",
        "Lock manager",
        "The lock manager",
        G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_STATIC_STRINGS
    );

    session_lock_lock_properties[SESSION_LOCK_LOCK_PROP_LOCK_OBJ] = g_param_spec_pointer (
        "lock-obj",
        "Lock object",
        "The lock object",
        G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS
    );

    g_object_class_install_properties (object_class, SESSION_LOCK_LOCK_LAST_PROP, session_lock_lock_properties);

    session_lock_lock_signals[SESSION_LOCK_LOCK_SIGNAL_LOCKED] = g_signal_new (
        "locked",
        G_TYPE_FROM_CLASS (klass),
        G_SIGNAL_RUN_FIRST,
        0,
        NULL,
        NULL,
        NULL,
        G_TYPE_NONE,
        0
    );

    session_lock_lock_signals[SESSION_LOCK_LOCK_SIGNAL_FINISHED] = g_signal_new (
        "finished",
        G_TYPE_FROM_CLASS (klass),
        G_SIGNAL_RUN_FIRST,
        0,
        NULL,
        NULL,
        NULL,
        G_TYPE_NONE,
        0
    );
}

static void
gtk_session_lock_lock_init (GtkSessionLockLock *self)
{
    GtkSessionLockLockPrivate *priv = gtk_session_lock_lock_get_instance_private (self);
    priv->lock_obj = NULL;
}

static void
session_lock_lock_handle_locked (void *data, struct ext_session_lock_v1 *lock_obj)
{
    GtkSessionLockLock *self = GTK_SESSION_LOCK_LOCK (data);
    g_signal_emit (self, session_lock_lock_signals[SESSION_LOCK_LOCK_SIGNAL_LOCKED], 0);
}

static void
session_lock_lock_handle_finished (void *data, struct ext_session_lock_v1 *lock_obj)
{
    GtkSessionLockLock *self = GTK_SESSION_LOCK_LOCK (data);
    g_signal_emit (self, session_lock_lock_signals[SESSION_LOCK_LOCK_SIGNAL_FINISHED], 0);
}

static const struct ext_session_lock_v1_listener lock_listener = {
    .locked = session_lock_lock_handle_locked,
    .finished = session_lock_lock_handle_finished,
};

void
gtk_session_lock_lock_lock (GtkSessionLockLock *self)
{
    GtkSessionLockLockPrivate *priv = gtk_session_lock_lock_get_instance_private (self);
    priv->lock_obj = ext_session_lock_manager_v1_lock (priv->lock_manager);

    ext_session_lock_v1_add_listener (priv->lock_obj, &lock_listener, self);
}

void
gtk_session_lock_lock_destroy (GtkSessionLockLock *self)
{
    GtkSessionLockLockPrivate *priv = gtk_session_lock_lock_get_instance_private (self);
    // TODO: handle errors
    if (priv->lock_obj != NULL) {
        ext_session_lock_v1_destroy (priv->lock_obj);
        priv->lock_obj = NULL;
    }
}

void
gtk_session_lock_lock_unlock_and_destroy (GtkSessionLockLock *self)
{
    GtkSessionLockLockPrivate *priv = gtk_session_lock_lock_get_instance_private (self);
    if (priv->lock_obj != NULL) {
        ext_session_lock_v1_unlock_and_destroy (priv->lock_obj);
        priv->lock_obj = NULL;
    }
}

void
gtk_session_lock_lock_new_surface (GtkSessionLockLock *self, GtkWindow *gtk_window, GdkMonitor *monitor)
{
    GtkSessionLockLockPrivate *priv = gtk_session_lock_lock_get_instance_private (self);
    lock_surface_new(gtk_window, monitor, priv->lock_obj);
}
