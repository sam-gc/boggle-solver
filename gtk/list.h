#ifndef LIST_H
#define LIST_H

#include <gtk/gtk.h>

void init_list(GtkWidget *list);
void add_to_list(GtkWidget *list, const gchar *str);
void move_to_end_of_list(GtkWidget *list);
void clear_list(GtkWidget *list);

#endif
