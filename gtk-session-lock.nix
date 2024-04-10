{ lib
, stdenv
, meson
, ninja
, pkg-config
, wayland-scanner
, wayland
, gtk3
, gobject-introspection
, vala
}:

stdenv.mkDerivation (finalAttrs: {
  pname = "gtk-session-lock";
  version = "0.2.0";

  outputs = [ "out" "dev" ];

  src = ./.;

  strictDeps = true;

  depsBuildBuild = [
    pkg-config
  ];

  nativeBuildInputs = [
    meson
    ninja
    pkg-config
    gobject-introspection
    vala
    wayland-scanner
  ];

  buildInputs = [
    wayland
    gtk3
  ];

  meta = with lib; {
    description = "A library to create Wayland session lockers using GTK";
    homepage = "https://github.com/Cu3PO42/gtk-session-lock";
    license = licenses.gpl3Plus;
    platforms = platforms.linux;
  };
})
