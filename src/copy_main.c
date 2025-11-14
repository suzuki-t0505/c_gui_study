#include <gtk/gtk.h>

static void print_hello(GtkWidget *widget, gpointer data) {
    g_print("Hello World\n");
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *btn;
    GtkWidget *box;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Hello!!");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 600);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    gtk_window_set_child(GTK_WINDOW(window), box);


    grid = gtk_grid_new();
    gtk_box_append(GTK_BOX(box), grid);


    btn = gtk_button_new_with_label("Button 1");
    g_signal_connect(btn, "clicked", G_CALLBACK(print_hello), NULL);
    gtk_grid_attach(GTK_GRID(grid), btn, 0, 0, 1, 1);

    btn = gtk_button_new_with_label("Button 2");
    g_signal_connect(btn, "clicked", G_CALLBACK(print_hello), NULL);
    gtk_grid_attach(GTK_GRID(grid), btn, 1, 0, 1, 1);

    btn = gtk_button_new_with_label("Quit");
    g_signal_connect_swapped(btn, "clicked", G_CALLBACK(gtk_window_destroy), window);
    gtk_grid_attach(GTK_GRID(grid), btn, 0, 1, 2, 1);

    gtk_window_present(GTK_WINDOW(window));

    // GtkBuilder *builder = gtk_bulder_new();
    // gtk_builder_add_from_file(builder, "builder.ui", NULL);

    // GObject *window = gtk_builder_get_object(builder, "window");
    // get_window_set_application(GTK_WINDOW(window), app);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.tk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}