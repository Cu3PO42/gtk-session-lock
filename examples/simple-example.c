#include "gtk-session-lock.h"
#include <gtk/gtk.h>
#include <stdio.h>

GtkSessionLockLock *lock;
GtkApplication *app;

static void unlock( GtkWidget *widget, gpointer data) {

    gtk_session_lock_lock_unlock_and_destroy(lock);

    gdk_display_sync(gdk_display_get_default());
    g_application_quit(G_APPLICATION(app));
}

static GtkWindow* create_lock_window() {
    GtkWindow *window = GTK_WINDOW (gtk_application_window_new(app));
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(entry),FALSE);
    gtk_widget_set_valign(entry, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(entry, GTK_ALIGN_CENTER);

    gtk_container_add(GTK_CONTAINER(window), entry);
    
    g_signal_connect(G_OBJECT(entry), "activate", G_CALLBACK(unlock), NULL);

    return window;
}

static void activate (GtkApplication* app, void *_data) {

    if (!gtk_session_lock_is_supported()) {
        printf("Your Wayland compositor does not support the ext-session-lock protocol\n");
        g_application_quit(G_APPLICATION(app));
    }

    lock = gtk_session_lock_prepare_lock();

    gtk_session_lock_lock_lock(lock);
    
    GdkDisplay *display = gdk_display_get_default();
    for (int i = 0; i < gdk_display_get_n_monitors(display); ++i) {
        GdkMonitor *monitor = gdk_display_get_monitor(gdk_display_get_default(), i);
        
        GtkWindow *window = create_lock_window();
        gtk_session_lock_lock_new_surface(lock, window, monitor);

        gtk_widget_show_all(GTK_WIDGET(window));  
    }
}

int main (int argc, char **argv) {

    app = gtk_application_new ("Cu3PO42.gtk-session-lock-example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    int status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);
    return status;
}
