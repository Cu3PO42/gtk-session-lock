# GTK Session Lock for GTK3

This is a library to use [GTK 3](https://www.gtk.org/) to build screen lockers using the secure [ext-session-lock-v1](https://wayland.app/protocols/ext-session-lock-v1) protocol. This Library is compatible with C, C++ and any language that supports GObject introspection files (Python, Vala, etc, see using the library below).

This library is a fork of the incredible [gtk-layer-shell](https://github.com/wmww/gtk-layer-shell), which has laid all the groundwork necessary to make this happen.

## Reporting Bugs
To report a crash or other problem using this library open a new [issue on Github](https://github.com/Cu3PO42/gtk-session-lock/issues). Try to include a minimum reproducer if possible (ideally in C). **DO NOT REPORT GTK SESSION LOCK BUGS TO UPSTREAM GTK**. If you can reproduce the problem without including or linking to the gtk-session-lock library **at all** then and only then report it to GTK instead of here.

## Supported Desktops
This library only works on Wayland, and only on Wayland compositors that support the ext-session-lock-v1 protocol. Session lock __is supported__ on:

- wlroots based compositors (such as __Sway__ or __Hyprland__)
- __Mir__-based compositors

ext-session-lock-v1 __is not supported__ on:
- Gnome-on-Wayland
- KDE Plasma
- Any X11 desktop

## Limitations

The ext-session-lock-v1 protocol can only show one surface per monitor.
This means, only one window can be shown, therefore popups aren't supported.
Unfortunately, tooltips are also new windows and do not work.
This is an inherent limitation and can only be worked around, not resolved.

## Using the Library

### In the abstract

The operations in this library need to be performed in a specific order to guarantee correct behavior.
1. You SHOULD check for support of the `ext-session-lock-v1` protocl via the `gtk_session_lock_is_supported()` method.
2. To lock the session, first prepare a lock via `gtk_session_lock_prepare_lock()`. The purpose of the prepare operation is to perform additional setup before actually performing the lock.
3. You MUST connect to the lock's `locked` signal. You SHOULD connect to its `finished` signal.
4. After correcting to signals, call the `gtk_session_lock_lock_lock(lock)` method.
6. If you receive the `finished` signal, the session could not be locked. You SHOULD call `gtk_session_lock_lock_destroy(lock)` to dispose of the Wayland objects and avoid memory leaks.
7. For every monitor:
   1. Create a Gtk Window, but do not yet show it.
   2. You MUST call `gtk_session_lock_lock_new_surface(lock, window, monitor)` to prepare the window for display on the given monitor before it is realized.
   3. You SHOULD show (i.e. realize and map) the window as soon as possible. If you do not, the compositor will display a solid color.
   4. You MUST NOT create two windows on the same monitor.
8. If you receive the `locked` signal, your session is now locked. Your compositor will not be showing any information other than your lockscreen or solid colors.
9. You SHOULD listen to monitor connection and disconnection events and create new windows on demand.
10. Before calling `hide` on the window, you MUST call `gtk_session_lock_unmap_lock_window(window)` on it. You MUST NOT call this method before `destroy`. This additional step is necessary to maintain compatibility with gtk-layer-shell.
11. Once you wish to unlock the session, e.g. after you have authenticated the user, you SHOULD first call `gtk_session_lock_lock_unlock_and_destroy(lock)`.
12. Subsequently you SHOULD destroy all windows you previously created as lock surfaces.
13. Before exiting your application, you MUST wait for a Wayland display sync. To this end, call `gdk_display_sync(gdk_display_get_default())`.

### C/C++
The easiest way to build against GTK Session Lock is to use the `gtk-session-lock-0` pkg-config package. Refer to your build system or the pkg-config docs for further instructions. [examples/simple-example.c](examples/simple-example.c) is a minimal complete app written in C.

### Other Languages

The library can be used from any language supporting GObject Introspection.
Please refer to the docs for your respective language for instructions.

## Examples

kotontrion has kindly contributed C and Python examples, which you can find in the `examples/` subdirectory.

## Building From Source
1. Clone this repo
2. Install build dependencies (see below)
3. `$ meson setup build`
4. `$ ninja -C build`
5. `$ sudo ninja -C build install`
6. `$ sudo ldconfig`

### Build Dependencies
* [Meson](https://mesonbuild.com/) (>=0.45.1)
* [libwayland](https://gitlab.freedesktop.org/wayland/wayland) (>=1.10.0)
* [GTK3](https://www.gtk.org/) (>=3.22.0)
* [GObject introspection](https://gitlab.gnome.org/GNOME/gobject-introspection/)
* [Vala](https://wiki.gnome.org/Projects/Vala) (only required if vapi is enabled)

The only officially supported way to build GTK Session Lock is via the included Nix Flake.
You can also obtain these dependencies for development purposes by starting a shell with `nix develop .#`.

The following may or may not work on Ubuntu 18.04 and later:
```
sudo apt install meson libwayland-dev libgtk-3-dev gobject-introspection libgirepository1.0-dev gtk-doc-tools valac
```

### Meson Options
* `-Dintrospection` (default: `true`): If to build GObject Introspection data (used for bindings to languages other than C/C++)
* `-Dvapi` (default: `true`): If to build VAPI data (allows this library to be used in Vala). Requires `-Dintrospection=true`

## Licensing
GTK Session Lock is licensed under the GNU General Public License version 3.0 or any later version approved by me (Cu3PO42).

Some source files, which are entirely or almost entirely unmodified from the original gtk-layer-shell are also available under the terms of the MIT license.
