#ifndef LOCK_SURFACE_H
#define LOCK_SURFACE_H

#include <stdbool.h>
#include "custom-shell-surface.h"

typedef struct _LockSurface LockSurface;

// Functions that mutate this structure should all be in lock-surface.c to make the logic easier to understand
// Struct is declared in this header to prevent the need for excess getters
struct _LockSurface
{
    CustomShellSurface super;

    // Cannot be changed
    GdkMonitor *monitor; // Must not be null
    bool first_commit_done;

    // Not set by user requests
    struct ext_session_lock_v1 *session_lock; // The session lock Wayland object used to get the surface
    struct ext_session_lock_surface_v1 *lock_surface; // The actual lock surface Wayland object (can be NULL)
    GtkRequisition current_allocation; // Last size allocation, or (0, 0) if there hasn't been one
    GtkRequisition last_configure_size; // Last size received from a configure event
};

LockSurface *lock_surface_new (GtkWindow *gtk_window, GdkMonitor *monitor, struct ext_session_lock_v1 *session_lock);

// Safe cast, returns NULL if wrong type sent
LockSurface *custom_shell_surface_get_lock_surface (CustomShellSurface *shell_surface);

#endif // LOCK_SURFACE_H

