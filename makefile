solver: boggle.c build.h definitions.h dictionary.c dictionary.h hashset.c hashset.h solver.c solver.h progress.c progress.h
	incrbuild build.h
	gcc -o solver boggle.c dictionary.c hashset.c solver.c progress.c -pthread -g -O0

solver-release: boggle.c build.h definitions.h dictionary.c dictionary.h hashset.c hashset.h solver.c solver.h progress.c progress.h
	incrbuild build.h
	gcc -o solver boggle.c dictionary.c hashset.c solver.c progress.c -pthread -O3

solver-gtk: gtk/gtk.c build.h definitions.h dictionary.c dictionary.h hashset.c hashset.h solver.c solver.h progress.c progress.h gtk/list.c gtk/list.h
	incrbuild build.h
	gcc -o solver-gtk gtk/gtk.c dictionary.c hashset.c solver.c progress.c gtk/list.c -pthread -g -O0 `pkg-config --cflags --libs gtk+-2.0`

solver-gtk-release: gtk/gtk.c build.h definitions.h dictionary.c dictionary.h hashset.c hashset.h solver.c solver.h progress.c progress.h gtk/list.c gtk/list.h
	incrbuild build.h
	gcc -o solver-gtk gtk/gtk.c dictionary.c hashset.c solver.c progress.c gtk/list.c -pthread -O3 `pkg-config --cflags --libs gtk+-2.0`
