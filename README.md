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

## Using the Library

### C/C++
The easiest way to build against GTK Session Lock is to use the `gtk-session-lock-0` pkg-config package. Refer to your build system or the pkg-config docs for further instructions. [examples/simple-example.c](examples/simple-example.c) is a minimal complete app written in C.

### Other Languages

The library can be used from any language supporting GObject Introspection.
Please refer to the docs for your respective language for instructions.

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
