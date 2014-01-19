#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dictionary.h"
#include "hashset.h"

Hashset words;
Hashset prefixes;

void dict_load()
{
    words = HS_create(40000, 1);
    prefixes = HS_create(40000, 1);

    FILE *file = fopen("words.txt", "r");
    char *line = NULL;
    size_t len;

    while((getline(&line, &len, file)) != -1)
    {
        line[strlen(line) - 1] = '\0';
        if(strlen(line) > 2)
            HS_add(&words, line);

        int len = strlen(line);
        int i;
        for(i = 1; i < len; i++)
        {
            char curr[100];
            strcpy(curr, line);
            curr[i] = '\0';
            HS_add(&prefixes, curr);
        }
    }

    if(line)
        free(line);

    fclose(file);
}

BOOL dict_is_word(char *word)
{
    char *conv = malloc(strlen(word));
    strcpy(conv, word);

    int i;
    for(i = 0; i < strlen(word); i++)
        conv[i] = toupper(conv[i]);

    BOOL retval = HS_contains(&words, conv);
    free(conv);

    return retval;
}

BOOL dict_is_prefix(char *prfx)
{
    char *conv = malloc(strlen(prfx));
    strcpy(conv, prfx);

    int i;
    for(i = 0; i < strlen(prfx); i++)
        conv[i] = toupper(conv[i]);

    BOOL retval = HS_contains(&prefixes, conv);
    free(conv);
    
    return retval;
}

void dict_clean_up()
{
    HS_free(&words);
    HS_free(&prefixes);
}
