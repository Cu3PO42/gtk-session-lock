srcs = files(
    'api.c',
    'gtk-wayland.c',
    'custom-shell-surface.c',
    'lock-surface.c',
    'gtk-priv-access.c',
    'session-lock.c')

version_args = [
    '-DGTK_SESSION_LOCK_MAJOR=' + meson.project_version().split('.')[0],
    '-DGTK_SESSION_LOCK_MINOR=' + meson.project_version().split('.')[1],
    '-DGTK_SESSION_LOCK_MICRO=' + meson.project_version().split('.')[2],
]

gtk_session_lock_lib = library('gtk-session-lock',
    srcs, client_protocol_srcs,
    c_args: version_args,
    include_directories: [gtk_session_lock_inc],
    dependencies: [gtk, wayland_client, gtk_priv],
    version: meson.project_version(),
    soversion: lib_so_version,
    install: true)

pkg_config_name = 'gtk-session-lock-0'

# GObject introspection file used to interface with other languages
if get_option('introspection')
    gir = gnome.generate_gir(
        gtk_session_lock_lib,
        dependencies: [gtk],
        sources: srcs + files('../include/gtk-session-lock.h'),
        namespace: 'GtkSessionLock',
        nsversion: '0.1',
        identifier_prefix: 'GtkSessionLock',
        symbol_prefix: 'gtk_session_lock',
        export_packages: pkg_config_name,
        includes: [ 'Gtk-3.0' ],
        header: 'gtk-session-lock/gtk-session-lock.h',
        install: true)

    if get_option('vapi')
        gnome.generate_vapi(
            pkg_config_name,
            sources: [gir[0]],
            packages: 'gtk+-3.0',
            install: true)
    endif
endif

pkg_config.generate(
    name: 'gtk-session-lock',
    version: meson.project_version(),
    libraries: [gtk_session_lock_lib],
    filebase: pkg_config_name,
    subdirs: 'gtk-session-lock',
    description: 'Use the ext-session-lock-v1 Wayland protocol with GTK',
    url: 'https://github.com/Cu3PO42/gtk-session-lock')
