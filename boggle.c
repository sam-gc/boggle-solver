#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "definitions.h"
#include "build.h"
#include "dictionary.h"
#include "solver.h"
#include "hashset.h"

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

    dict_load();

    solver_start_add();

    int i;
    for(i = 0; i < 16; i++)
    {
        solver_add(getchar());
    }

    Hashset words = solver_solve();

    Hashlist *listed = HS_to_list(&words);
    for(i = -1; listed; i++, listed = listed->next)
        printf("%s\n", listed->value);

    while(listed)
    {
        Hashlist *next = listed->next;
        free(listed);
        listed = next;
    }

    printf("Found %d words.\n", HS_count(&words));

    HS_free(&words);
    dict_clean_up();

    return 0;
}