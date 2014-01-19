#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/timeb.h>
#include "dictionary.h"
#include "hashset.h"
#include "progress.h"

#define NUMWORDS 267751

#define THREADED

#ifdef THREADED
#define MUTLOCK(MUT) pthread_mutex_lock(&MUT)
#define MUTUNLOCK(MUT) pthread_mutex_unlock(&MUT)
pthread_mutex_t word_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t prfx_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t status_mut = PTHREAD_MUTEX_INITIALIZER;
#else
#define MUTLOCK(MUT)
#define MUTUNLOCK(MUT)
#endif

Hashset words;
Hashset prefixes;

BOOL dictionary_loaded = NO;
BOOL dictionary_is_uppercase = NO;

int toupper(int);
int tolower(int);

long millis()
{
    struct timeb tb;
    ftime(&tb);
    return (long)(tb.time * 1000 + tb.millitm);
}

void dict_load()
{
    ProgressBar pb = pb_create("Loading Dictionary...", NUMWORDS);
    pb_begin(&pb);
    MUTLOCK(word_mut);
    words = HS_create(100000, 1);
    MUTUNLOCK(word_mut);
    MUTLOCK(prfx_mut);
    prefixes = HS_create(100000, 1);
    MUTUNLOCK(prfx_mut);

    FILE *file = fopen("words.txt", "r");
    char *line = NULL;
    size_t len;

    int count = 0;
    long start = millis();

    while((getline(&line, &len, file)) != -1)
    {
        line[strlen(line) - 1] = '\0';
        if(strlen(line) > 2)
        {
            MUTLOCK(word_mut);
            HS_add(&words, line);
            MUTUNLOCK(word_mut);
        }

        int len = strlen(line);
        int i;
        for(i = 1; i < len; i++)
        {
            char curr[100];
            strcpy(curr, line);
            curr[i] = '\0';
            MUTLOCK(prfx_mut);
            HS_add(&prefixes, curr);
            MUTUNLOCK(prfx_mut);
        }
        pb_update(&pb, ++count);
    }

    if(line)
        free(line);

    fclose(file);

    MUTLOCK(status_mut);
    dictionary_loaded = YES;
    MUTUNLOCK(status_mut);

    printf("Dictionary done loading. Took: %ldms.\n", millis() - start);
}

BOOL dict_is_word(char *word)
{
    MUTLOCK(status_mut);
    if(!dictionary_loaded)
    {
        printf("Dictionary not loaded!\n");
        return NO;
    }
    MUTUNLOCK(status_mut);

    char *conv = malloc(strlen(word) + 1);
    
    strcpy(conv, word);

    int (*strfunc)(int) = dictionary_is_uppercase ? &toupper : &tolower;

    int i;
    for(i = 0; i < strlen(word); i++)
        conv[i] = strfunc(conv[i]);

    MUTLOCK(word_mut);
    BOOL retval = HS_contains(&words, conv);
    MUTUNLOCK(word_mut);

    free(conv);

    return retval;
}

BOOL dict_is_prefix(char *prfx)
{
    MUTLOCK(status_mut);
    if(!dictionary_loaded)
    {
        printf("Dictionary not loaded!\n");
        return NO;
    }
    MUTUNLOCK(status_mut);

    char *conv = malloc(strlen(prfx) + 1);
    strcpy(conv, prfx);

    int (*strfunc)(int) = dictionary_is_uppercase ? &toupper : &tolower;

    int i;
    for(i = 0; i < strlen(prfx); i++)
        conv[i] = strfunc(conv[i]);

    MUTLOCK(prfx_mut);
    BOOL retval = HS_contains(&prefixes, conv);
    MUTUNLOCK(prfx_mut);
    free(conv);
    
    return retval;
}

void dict_clean_up()
{
    MUTLOCK(word_mut);
    HS_free(&words);
    MUTUNLOCK(word_mut);
    MUTLOCK(prfx_mut);
    HS_free(&prefixes);
    MUTUNLOCK(prfx_mut);
}
