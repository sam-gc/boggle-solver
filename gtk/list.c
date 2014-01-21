#include <gtk/gtk.h>
#include "list.h"

enum
{
    LIST_ITEM = 0,
    N_COLUMNS
};

void init_list(GtkWidget *list)
{

    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkListStore *store;

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("List Items",
    renderer, "text", LIST_ITEM, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

    store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);

    gtk_tree_view_set_model(GTK_TREE_VIEW(list), GTK_TREE_MODEL(store));

    g_object_unref(store);
}

void add_to_list(GtkWidget *list, const gchar *str)
{
    GtkListStore *store;
    GtkTreeIter iter;

    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));

    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter, LIST_ITEM, str, -1);
}

void move_to_end_of_list(GtkWidget *list)
{
    GtkListStore *store;
    GtkTreeIter iter;
    
    gdk_threads_enter();
    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));
    
    gtk_tree_model_get_iter_first(GTK_TREE_MODEL(store), &iter);
    gtk_list_store_move_before(store, &iter, NULL);
    gdk_threads_leave();
}

void clear_list(GtkWidget *list)
{
    GtkListStore *store;
    
    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));
    
    gtk_list_store_clear(store);
}
