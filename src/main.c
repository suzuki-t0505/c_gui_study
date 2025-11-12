#include <gtk/gtk.h>

static void on_clicked(GtkWidget *btn, gpointer win) {
    gtk_window_close(GTK_WINDOW(win));
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(win), "Hello GTK3 (Docker)");
    gtk_window_set_default_size(GTK_WINDOW(win), 600, 400);

    g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // GTK3 はコンテナ API
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);

    // ラベルの作成
    GtkWidget *label = gtk_label_new("title");
    gtk_container_add(GTK_CONTAINER(box), label);

    // テキストボックスの作成
    GtkWidget *entry = gtk_entry_new();
    gtk_container_add(GTK_CONTAINER(box), entry);

    // ボタンの作成
    GtkWidget *btn = gtk_button_new_with_label("Click me");
    g_signal_connect(btn, "clicked", G_CALLBACK(on_clicked), win);
    gtk_container_add(GTK_CONTAINER(win), box);
    gtk_box_pack_start(GTK_BOX(box), btn, TRUE, TRUE, 0);

    gtk_widget_show_all(win);
    gtk_main();
    return 0;
}