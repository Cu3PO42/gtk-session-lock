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

#include "gtk-priv-access.h"
#include "gtk-wayland.h"

#include "wayland-client.h"

typedef enum _PositionMethod
{
  POSITION_METHOD_ENUM
} PositionMethod;
typedef void *EGLSurface;
typedef void *GdkWaylandWindowExported;
typedef void *GdkWaylandTabletToolData;

#include "gdk_window_impl_priv.h"
#include "gdk_window_priv.h"
#include "gdk_window_impl_wayland_priv.h"
#include "gdk_window_impl_class_priv.h"
#include "gdk_wayland_pointer_frame_data_priv.h"
#include "gdk_wayland_pointer_data_priv.h"
#include "gdk_wayland_seat_priv.h"
#include "gdk_wayland_touch_data_priv.h"
#include "gdk_wayland_tablet_data_priv.h"

#include <glib-2.0/glib.h>

void
gdk_window_set_priv_mapped (GdkWindow *gdk_window)
{
    GdkWindowImplWayland *window_impl = (GdkWindowImplWayland *)gdk_window_priv_get_impl (gdk_window);
    gdk_window_impl_wayland_priv_set_mapped (window_impl, TRUE);
}
