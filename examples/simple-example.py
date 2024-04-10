import gi
gi.require_version('Gtk', '3.0')
gi.require_version('GtkSessionLock', '0.1')
from gi.repository import Gtk, Gdk, GtkSessionLock

if(not GtkSessionLock.is_supported()):
    quit()

lock = GtkSessionLock.prepare_lock()
display = Gdk.Display.get_default()

def unlock(widget):
    lock.unlock_and_destroy()
    display.sync()
    quit()

def create_lock_window():
    window = Gtk.Window()
    entry = Gtk.Entry(visibility=False,
                      valign=Gtk.Align.CENTER,
                      halign=Gtk.Align.CENTER)
    entry.connect("activate", unlock)
    window.add(entry)
    return window

def on_locked():
    print("Your session is now locked.")

def on_finished():
    print("Finished event received. Session could not be locked.")
    quit()

lock.connect("locked", on_locked)
lock.connect("finished", on_finished)

lock.lock_lock()

for i in range(display.get_n_monitors()):
    window = create_lock_window()
    monitor = display.get_monitor(i)
    lock.new_surface(window, monitor)
    window.show_all()

Gtk.main()
