#include <gtk/gtk.h>

static void quit_cb(GtkWindow *window) {
    gtk_window_close(window);
}

static void print_hello(GtkWidget *widget, gpointer data) {
    g_print("Hello World\n");
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "builder.ui", NULL);

    GObject *window = gtk_builder_get_object (builder, "window");
    gtk_window_set_application (GTK_WINDOW(window), app);
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 600);


    GObject *button = gtk_builder_get_object(builder, "button1");
    g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

    button = gtk_builder_get_object(builder, "button2");
    g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

    button = gtk_builder_get_object(builder, "quit");
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(quit_cb), window);

    GObject *text_view = gtk_builder_get_object(builder, "text");
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_text(buffer, "Hello, this is some text", -1);

    gtk_widget_set_visible(GTK_WIDGET(window), TRUE);

    g_object_unref (builder);
}

int main(int argc, char **argv) {
    #ifdef GTK_SRCDIR
        g_chdir (GTK_SRCDIR);
    #endif
    GtkApplication *app = gtk_application_new("org.tk.example", G_APPLICATION_DEFAULT_FLAGS);
    int status;

    app = gtk_application_new("org.tk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}