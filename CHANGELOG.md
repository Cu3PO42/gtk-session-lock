# Changelog

## 0.2

* Adds compatibility with gtk-layer-shell. Both libraries can now be used in the same process without interfering with each other.
* The required order of operations when hiding a session surface is slightly different. The new function `gtk_session_lock_unmap_lock_window` must be called before hiding a window rendered as a lockscreen.
* Adds examples for C and Python.

## 0.1

* Initial release of the library.

## [Unreleased]
