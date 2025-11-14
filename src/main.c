#include <gtk/gtk.h>

typedef struct {
    GtkWidget *title_entry;
    GtkWidget *date_entry;
    GtkWidget *title_label;
    GtkWidget *date_label;
} TaskInputWidgets;

typedef struct {
    GtkWidget *window;
    TaskInputWidgets *task_input_widgets;
} BaseWidgets;

static void quit_cb(gpointer data) {
    BaseWidgets *base_widgets = (BaseWidgets *)data;
    gtk_window_close(GTK_WINDOW(base_widgets->window));
    free(base_widgets->task_input_widgets);
    free(base_widgets);
}

static void print_hello(GtkWidget *widget, gpointer data) {
    g_print("Hello World\n");
}

static void submit_task(GtkWidget *widget, gpointer data) {
    g_print("call\n");
    TaskInputWidgets *task_input_widgtes = (TaskInputWidgets *)data;

    const char *title_text = gtk_editable_get_text(GTK_EDITABLE(task_input_widgtes->title_entry));
    g_print("%s\n", title_text);
    gtk_label_set_text(GTK_LABEL(task_input_widgtes->title_label), title_text);

    const char *date_text = gtk_editable_get_text(GTK_EDITABLE(task_input_widgtes->date_entry));
    g_print("%s\n", date_text);
    gtk_label_set_text(GTK_LABEL(task_input_widgtes->date_label), date_text);
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "builder.ui", NULL);

    GObject *window = gtk_builder_get_object (builder, "window");
    gtk_window_set_application (GTK_WINDOW(window), app);
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 600);

    TaskInputWidgets *task_input_widgets = malloc(sizeof(TaskInputWidgets));

    task_input_widgets->title_entry = GTK_WIDGET(gtk_builder_get_object(builder, "title_input"));
    task_input_widgets->date_entry = GTK_WIDGET(gtk_builder_get_object(builder, "date_input"));
    task_input_widgets->title_label = GTK_WIDGET(gtk_builder_get_object(builder, "title_label"));
    task_input_widgets->date_label = GTK_WIDGET(gtk_builder_get_object(builder, "date_label"));

    BaseWidgets *base_widgets = malloc(sizeof(BaseWidgets));
    base_widgets->window = GTK_WIDGET(window);
    base_widgets->task_input_widgets = task_input_widgets;

    GObject *button = gtk_builder_get_object(builder, "button1");
    g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

    button = gtk_builder_get_object(builder, "button2");
    g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

    button = gtk_builder_get_object(builder, "quit");
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(quit_cb), base_widgets);


    button = gtk_builder_get_object(builder, "submit_button");
    g_signal_connect(button, "clicked", G_CALLBACK(submit_task), task_input_widgets);


    gtk_widget_set_visible(GTK_WIDGET(window), TRUE);

    g_object_unref(builder);
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