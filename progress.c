#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "colors.h"
#include "progress.h"

int get_console_width()
{
    struct winsize w;
    w.ws_col = 0;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    return w.ws_col;
}

ProgressBar pb_create(char *title, int max)
{
    ProgressBar pb = {title, max};
    return pb;
}

void pb_begin(ProgressBar *pb)
{
    printf("%s\n" LIGHT_RED "[", pb->title);
    int dist = get_console_width() - 2;
    printf("\e[%dC]", dist);
    fflush(stdout);
}

void pb_update(ProgressBar *pb, int current)
{
    int width = get_console_width();
    int numbars = ((float)current / pb->max) * (width - 2);

    if(numbars == pb->numbars)
        return;

    pb->numbars = numbars;

    printf("\e[%dD" LIGHT_RED "[" DEFAULT, width);
    int i;
    for(i = 0; i < numbars - 1; i++)
        printf("=");
    if(numbars > 0)
        printf(">");

    printf("\e[%dC" LIGHT_RED "]" DEFAULT, width - numbars - 2);
    fflush(stdout);
}