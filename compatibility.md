# GTK compatibility
__NOTE: the warning that linked you to this page is probably a red herring. GTK Session Lock has a hard time *explicitly* supporting each version of GTK, but new unsupported versions almost always work in practice.__

Due to limitations of GTK, GTK Session Lock depends on unstable GTK internals. Each release works with all versions of GTK >=3.22.0 at the time of release, but future GTK updates may occasionally break them. See [gtk-priv/README.md](gtk-priv/README.md) for more details.

The table below shows which GTK Session Lock versions work with which GTK versions. If there is a mistake, missing info or you have any other questions, please [open an issue](https://github.com/Cu3PO42/gtk-session-lock/issues).

| GTK Session Lock | Explicitly supported GTK | Highest GTK known to work | GTK versions known to crash |
|---|---|---|---|
| __v0.1.0__ | v3.20.0 - v3.24.39 | v3.24.39 | - |
