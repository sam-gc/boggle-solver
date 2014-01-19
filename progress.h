#ifndef PROGRESS_H
#define PROGRESS_H

typedef struct {
    char *title;
    int max;
    int numbars;
} ProgressBar;

ProgressBar pb_create(char *title, int max);
void pb_begin(ProgressBar *pb);
void pb_update(ProgressBar *pb, int current);

#endif