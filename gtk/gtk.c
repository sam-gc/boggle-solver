#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <pthread.h>

#include "../dictionary.h"
#include "../hashset.h"
#include "../solver.h"
#include "../build.h"
#include "../definitions.h"
#include "list.h"

GtkWidget *textboxes[4][4];
GtkWidget *window, *list;
int tb_index = 0;

BOOL strequ(char *a, char *b)
{
    return strcmp(a, b) == 0;
}

int length_sort(const char *a, const char *b)
{
    int alen = strlen(a), blen = strlen(b);
    if(alen < blen)
        return 1;
    if(blen < alen)
        return -1;

    return strcmp(a, b);
}

gint delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    gtk_main_quit();
    return(FALSE);
}

void *load_dict_threaded(void *data)
{
    dict_load();
    return NULL;
}

void backspace_pressed(GtkEntry *entry, gpointer user_data)
{
    printf("Back pressed.\n");
    if(tb_index > 0)
        tb_index--;

    int x = tb_index % 4;
    int y = tb_index / 4;

    GtkEntry *en = GTK_ENTRY(textboxes[x][y]);
    gtk_widget_grab_focus(GTK_WIDGET(en));
}

gboolean tb_focus(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    int idx = *((int *)user_data);

    tb_index = idx;

    return FALSE;
}

gboolean key_pressed(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
    if(tb_index < 15)
        tb_index++;

    int x = tb_index % 4;
    int y = tb_index / 4;

    GtkEntry *en = GTK_ENTRY(textboxes[x][y]);
    gtk_entry_set_text(GTK_ENTRY(widget), event->string);
    gtk_editable_set_position(GTK_EDITABLE(widget), -1);
    gtk_widget_grab_focus(GTK_WIDGET(en));

    return TRUE;
}

void solve_clicked(GtkButton *button, gpointer data)
{
    solver_start_add();

    int x, y;
    for(x = 0; x < 4; x++)
    {
        for(y = 0; y < 4; y++)
        {
            GtkEntry *entry = GTK_ENTRY(textboxes[x][y]);
            const char *text = gtk_entry_get_text(entry);
            if(strlen(text) < 1)
            {
                GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Please fill out all of the tiles.");
                gtk_dialog_run(GTK_DIALOG(dialog));
                gtk_widget_destroy(dialog);
                return;
            }

            solver_add(text[0]);
        }
    }

    Hashset set = solver_solve();
    Hashlist *found = HS_to_list(&set);
    HS_list_sort(found, length_sort);

    clear_list(list);

    int count = 0;
    while(found)
    {
        Hashlist *next = found->next;
        add_to_list(list, found->value);
        found = next;
        count++;
    }

    printf("Found %d %s.\n", count, (count == 1 ? "word" : "words"));
    HS_free(&set);
}

void clear_clicked(GtkButton *button, gpointer data)
{
    int x, y;
    for(x = 0; x < 4; x++)
        for(y = 0; y < 4; y++)
            gtk_entry_set_text(GTK_ENTRY(textboxes[x][y]), "");

    clear_list(list);
}

int main(int argc, char *argv[])
{
    if(argc > 1)
    {
        if(strequ(argv[1], "--version"))
        {
            printf("Boggle Solver version 2.0\nBuild number %X\n", BUILDNUMBER);
            return 0;
        }
    }

    gdk_threads_init();

    // Load dictionary
    pthread_t dictThread;
    pthread_create(&dictThread, NULL, load_dict_threaded, NULL);
    pthread_detach(dictThread);

    GtkWidget *textbox_table, *solve_b, *clear_b, *list_scroll;

    gtk_init(&argc, &argv); //140

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_widget_set_size_request(window, 653, 485);
    gtk_window_set_title(GTK_WINDOW(window), "Boggle Solver");

    gtk_signal_connect(GTK_OBJECT(window), "delete_event", GTK_SIGNAL_FUNC(delete_event), NULL);

    textbox_table = gtk_table_new(6, 5, FALSE);
    list = gtk_tree_view_new();

    list_scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(list_scroll), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);
    gtk_widget_set_size_request(list_scroll, 190, -1);
    gtk_container_add(GTK_CONTAINER(list_scroll), list);
    gtk_table_attach_defaults(GTK_TABLE(textbox_table), list_scroll, 4, 5, 0, 5);
    gtk_table_set_col_spacings(GTK_TABLE(textbox_table), 10);
    gtk_table_set_row_spacings(GTK_TABLE(textbox_table), 10);

    solve_b = gtk_button_new_with_label("Solve");
    clear_b = gtk_button_new_with_label("Clear");

    gtk_widget_set_size_request(solve_b, 100, 27);
    gtk_widget_set_size_request(clear_b, 100, 27);

    gtk_table_attach_defaults(GTK_TABLE(textbox_table), solve_b, 0, 1, 4, 5);
    gtk_table_attach_defaults(GTK_TABLE(textbox_table), clear_b, 3, 4, 4, 5);

    gtk_signal_connect(GTK_OBJECT(solve_b), "clicked", GTK_SIGNAL_FUNC(solve_clicked), NULL);
    gtk_signal_connect(GTK_OBJECT(clear_b), "clicked", GTK_SIGNAL_FUNC(clear_clicked), NULL);

    int x, y;
    for(x = 0; x < 4; x++)
    {
        for(y = 0; y < 4; y++)
        {
            textboxes[x][y] = gtk_entry_new();
            GtkWidget *tb = textboxes[x][y];
            int *idx = malloc(sizeof(int));
            *idx = y * 4 + x;
            gtk_entry_set_max_length(GTK_ENTRY(tb), 1);
            gtk_table_attach_defaults(GTK_TABLE(textbox_table), tb, x, x + 1, y, y + 1);
            gtk_entry_set_width_chars(GTK_ENTRY(tb), 5);
            gtk_entry_set_alignment(GTK_ENTRY(tb), 0.5);
            gtk_widget_set_size_request(tb, 100, 100);
            gtk_widget_modify_font(tb, pango_font_description_from_string("Ubuntu 55"));
            gtk_signal_connect(GTK_OBJECT(tb), "backspace", GTK_SIGNAL_FUNC(backspace_pressed), NULL);
            gtk_signal_connect(GTK_OBJECT(tb), "key-press-event", GTK_SIGNAL_FUNC(key_pressed), NULL);
            gtk_signal_connect(GTK_OBJECT(tb), "button-press-event", GTK_SIGNAL_FUNC(tb_focus), idx);
        }
    }

    gtk_container_add(GTK_CONTAINER(window), textbox_table);

    init_list(list);

    gtk_widget_show_all(window);

    gtk_main();

    dict_clean_up();

    gdk_threads_leave();

    return 0;
}
