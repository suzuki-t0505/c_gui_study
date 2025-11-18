#include <gtk/gtk.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    int id;
    char title[256];
    struct tm date;
    bool completed;
    struct tm completed_date;
} Task;

typedef struct {
    GtkWidget *row_box;
    GtkWidget *status_label;
    GtkWidget *completed_date_label;
    GtkWidget *button;
    Task *task;
} TaskRowData;

typedef struct {
    GtkWidget *title_entry;
    GtkWidget *date_entry;
    GtkWidget *title_label;
    GtkWidget *date_label;
    GtkWidget *task_list;
    Task *tasks;
    int task_size;
} TaskInputWidgets;

typedef struct {
    GtkWidget *window;
    TaskInputWidgets *task_input_widgets;
} BaseWidgets;

void CreateDateTime(const char*, struct tm*);
void CreateDateTimeStr(char*, int, struct tm*);

void CreateDateTime(const char* date, struct tm* datetime) {
    char str_year[5];
    strncpy(str_year, date, 4);
    int year = atoi(str_year);

    char str_month[3];
    strncpy(str_month, date+4, 2);
    int month = atoi(str_month);

    char str_day[3];
    strncpy(str_day, date+6, 2);
    int day = atoi(str_day);

    datetime->tm_year = year - 1900;
    datetime->tm_mon = month - 1;
    datetime->tm_mday = day;
    datetime->tm_hour = 23;
    datetime->tm_min = 59;
    datetime->tm_sec = 59;
    datetime->tm_wday = 1;
    mktime(datetime);
}

void CreateDateTimeStr(char* buf, int size, struct tm* datetime) {
    strftime(buf, size, "%Y/%m/%d", datetime);
}

static void quit_cb(gpointer data) {
    BaseWidgets *base_widgets = (BaseWidgets *)data;
    gtk_window_close(GTK_WINDOW(base_widgets->window));
    free(base_widgets->task_input_widgets);
    free(base_widgets);
}

static void completed_task(GtkWidget *widget, gpointer data) {
    // ここにボタンを押したときの日時をtaskのcomplated_dateに追加&taskのcompltedをtrue
    // 表示をcompletedに変更
    TaskRowData *row_data = (TaskRowData *)data;
    Task *task = row_data->task;

    g_print("completed task -> %d, %s\n", task->id, task->title);
    
    task->completed = true;
    gtk_label_set_text(GTK_LABEL(row_data->status_label), "completed");

    time_t current_unix_time;
    time(&current_unix_time);
    task->completed_date = *localtime(&current_unix_time);

    char datetime_str[256];
    CreateDateTimeStr(datetime_str, sizeof(datetime_str), &task->completed_date);
    gtk_label_set_text(GTK_LABEL(row_data->completed_date_label), datetime_str);

    gtk_box_remove(GTK_BOX(row_data->row_box), row_data->button);
    free(row_data);
}

static void submit_task(GtkWidget *widget, gpointer data) {
    Task new_task;
    TaskInputWidgets *task_input_widgtes = (TaskInputWidgets *)data;

    const char *text = gtk_editable_get_text(GTK_EDITABLE(task_input_widgtes->title_entry));
    strcpy(new_task.title, text);
    gtk_label_set_text(GTK_LABEL(task_input_widgtes->title_label), new_task.title);

    text = gtk_editable_get_text(GTK_EDITABLE(task_input_widgtes->date_entry));
    CreateDateTime(text, &new_task.date);
    char datetime_str[256];
    CreateDateTimeStr(datetime_str, sizeof(datetime_str), &new_task.date);
    gtk_label_set_text(GTK_LABEL(task_input_widgtes->date_label), datetime_str);

    new_task.id = task_input_widgtes->task_size + 1;
    new_task.completed = false;
    task_input_widgtes->tasks = realloc(task_input_widgtes->tasks, sizeof(Task) * (task_input_widgtes->task_size + 1));
    task_input_widgtes->tasks[task_input_widgtes->task_size] = new_task;
    task_input_widgtes->task_size++;

    GtkWidget *row_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    char label_text[256];

    GtkWidget *id_label = gtk_label_new(NULL);
    sprintf(label_text, "id: %d", new_task.id);
    gtk_label_set_text(GTK_LABEL(id_label), label_text);

    GtkWidget *title_label = gtk_label_new(NULL);
    gtk_label_set_xalign(GTK_LABEL(title_label), 0.0);
    gtk_label_set_text(GTK_LABEL(title_label), new_task.title);

    GtkWidget *date_label = gtk_label_new(NULL);
    gtk_label_set_xalign(GTK_LABEL(date_label), 0.0);
    gtk_label_set_text(GTK_LABEL(date_label), datetime_str);

    GtkWidget *status_label = gtk_label_new(NULL);
    gtk_label_set_xalign(GTK_LABEL(status_label), 0.0);
    gtk_label_set_text(GTK_LABEL(status_label), new_task.completed ? "completed" : "incomplete");

    GtkWidget *completed_date_label = gtk_label_new(NULL);
    gtk_label_set_xalign(GTK_LABEL(completed_date_label), 0.0);

    GtkWidget *button = gtk_button_new_with_label("complete?");

    TaskRowData *row_data = malloc(sizeof(TaskRowData));
    row_data->row_box = row_box;
    row_data->status_label = status_label;
    row_data->completed_date_label = completed_date_label;
    row_data->button = button;
    row_data->task = &task_input_widgtes->tasks[task_input_widgtes->task_size - 1];

    g_signal_connect(button, "clicked", G_CALLBACK(completed_task), row_data);

    gtk_box_append(GTK_BOX(row_box), id_label);
    gtk_box_append(GTK_BOX(row_box), title_label);
    gtk_box_append(GTK_BOX(row_box), date_label);
    gtk_box_append(GTK_BOX(row_box), status_label);
    gtk_box_append(GTK_BOX(row_box), completed_date_label);
    gtk_box_append(GTK_BOX(row_box), button);

    gtk_list_box_append(GTK_LIST_BOX(task_input_widgtes->task_list), row_box);
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
    task_input_widgets->task_list = GTK_WIDGET(gtk_builder_get_object(builder, "task_list"));
    task_input_widgets->tasks = NULL;
    task_input_widgets->task_size = 0;

    BaseWidgets *base_widgets = malloc(sizeof(BaseWidgets));
    base_widgets->window = GTK_WIDGET(window);
    base_widgets->task_input_widgets = task_input_widgets;

    GObject *button = gtk_builder_get_object(builder, "quit");
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

    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}