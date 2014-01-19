#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include "definitions.h"
#include "build.h"
#include "dictionary.h"
#include "solver.h"
#include "hashset.h"

void *load_dict_threaded(void *data)
{
    dict_load();
    return NULL;
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

int main(int argc, char *argv[])
{
    if(argc > 1)
    {
        if(strcmp(argv[1], "--version") == 0)
        {
            printf("Boggle Solver version " VERSION "\nBuild %X\n", BUILDNUMBER);
            return 0;
        }
    }

    pthread_t dictThread;
    pthread_create(&dictThread, NULL, load_dict_threaded, NULL);
    pthread_detach(dictThread);

    solver_start_add();

    int i;
    for(i = 0; i < 16; i++)
    {
        solver_add(getchar());
    }

    Hashset words = solver_solve();

    Hashlist *listed = HS_to_list(&words);
    HS_list_sort(listed, length_sort);

    while(listed)
    {
        printf("%s\n", listed->value);
        Hashlist *next = listed->next;
        free(listed);
        listed = next;
    }

    printf("Found %d words.\n", HS_count(&words));

    HS_free(&words);
    dict_clean_up();

    return 0;
}