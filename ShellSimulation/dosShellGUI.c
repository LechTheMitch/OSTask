#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COMMANDS 100
#define BUFFER_SIZE 1024

typedef struct {
    char dos_command[50];
    char linux_command[50];
} CommandMapping;

CommandMapping mappings[MAX_COMMANDS];
int mapping_count = 0;
GtkTextView *commandOutput;

void load_mappings(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Mapping file error");
        exit(EXIT_FAILURE);
    }

    while (fscanf(file, "%49[^=]=%49s\n", mappings[mapping_count].dos_command, mappings[mapping_count].linux_command) == 2) {
        mapping_count++;
    }

    fclose(file);
}

char* translate_command(const char *input) {
    for (int i = 0; i < mapping_count; i++) {
        if (strcmp(input, mappings[i].dos_command) == 0) {
            return mappings[i].linux_command;
        }
    }
    return NULL;
}

static void on_run_command(GtkButton *button, gpointer user_data) {
    GtkEntry *entry = GTK_ENTRY(user_data);
    const char *input = gtk_editable_get_text(GTK_EDITABLE(entry));

    char command[50], args[BUFFER_SIZE] = "";
    sscanf(input, "%s %[^\n]", command, args);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(commandOutput);
    gtk_text_buffer_set_text(buffer, "", -1);

    char *linux_cmd = translate_command(command);
    if (linux_cmd) {
        char full_cmd[BUFFER_SIZE];
        snprintf(full_cmd, sizeof(full_cmd), "%s %s", linux_cmd, args);

        FILE *fp = popen(full_cmd, "r");
        if (fp) {
            char line[BUFFER_SIZE];
            while (fgets(line, sizeof(line), fp)) {
                gtk_text_buffer_insert_at_cursor(buffer, line, -1);
            }
            pclose(fp);
        } else {
            gtk_text_buffer_set_text(buffer, "Failed to execute command.", -1);
        }
    } else {
        gtk_text_buffer_set_text(buffer, "Unknown DOS command.", -1);
    }
}

static void on_show_commands(GtkButton *button, gpointer user_data) {
    GtkWindow *parent = GTK_WINDOW(user_data);
    GString *command_list = g_string_new("");

    for (int i = 0; i < mapping_count; i++) {
        g_string_append_printf(command_list, "%s → %s\n", mappings[i].dos_command, mappings[i].linux_command);
    }

    GtkWidget *dialog = gtk_message_dialog_new(parent,
        GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Available DOS Commands");

    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog), "%s", command_list->str);
    gtk_window_present(GTK_WINDOW(dialog));
    g_signal_connect(dialog, "response", G_CALLBACK(gtk_window_destroy), NULL);
    g_string_free(command_list, TRUE);
}

static void activate(GtkApplication *app, gpointer user_data) {
    load_mappings("commandsList.txt");

    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "DOS Shell Simulation");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_window_set_child(GTK_WINDOW(window), vbox);

    GtkWidget *commandInput = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(commandInput), "Enter DOS command...");
    gtk_box_append(GTK_BOX(vbox), commandInput);

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(vbox), hbox);

    GtkWidget *run_button = gtk_button_new_with_label("Run");
    GtkWidget *show_button = gtk_button_new_with_label("Show Commands");

    gtk_box_append(GTK_BOX(hbox), run_button);
    gtk_box_append(GTK_BOX(hbox), show_button);

    GtkWidget *scrolled = gtk_scrolled_window_new();
    gtk_widget_set_vexpand(scrolled, TRUE);
    commandOutput = GTK_TEXT_VIEW(gtk_text_view_new());
    gtk_text_view_set_editable(commandOutput, FALSE);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), GTK_WIDGET(commandOutput));
    gtk_box_append(GTK_BOX(vbox), scrolled);

    g_signal_connect(run_button, "clicked", G_CALLBACK(on_run_command), commandInput);
    g_signal_connect(show_button, "clicked", G_CALLBACK(on_show_commands), window);

    gtk_window_present(GTK_WINDOW(window));
}

int main (int argc, char **argv)
{
    GtkApplication *app;
    int status;
    app = gtk_application_new ("org.osTeam.shellSimulation", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}