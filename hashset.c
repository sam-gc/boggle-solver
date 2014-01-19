#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashset.h"

typedef Hashlist node;

unsigned long djb2(char *str)
{
    unsigned long hash = 5381;
    char c;

    while(c = *str++)
        hash = ((hash << 5) + hash) + c;

    return hash;
}

Hashset HS_create(int hash_size, char copies_str)
{
    Hashset set;
    set.hash_size = hash_size;
    set.copies_str = copies_str;
    set.buckets = malloc(sizeof(node *) * hash_size);

    int i;
    for(i = 0; i < hash_size; i++)
        set.buckets[i] = NULL;

    return set;
}

void HS_add(Hashset *set, char *str)
{
    unsigned long hash = djb2(str);
    hash %= set->hash_size;

    node *n;
    if(!set->buckets[hash])
    {
        n = malloc(sizeof(node));
        set->buckets[hash] = n;
    }
    else
    {
        node *last = NULL;
        for(n = set->buckets[hash]; n; n = n->next)
        {
            if(strcmp(n->value, str) == 0)
                return;

            last = n;
        }

        n = last;
        n->next = malloc(sizeof(node));
        n = n->next;
    }

    char *value;
    if(set->copies_str)
    {
        value = malloc(strlen(str) + 1);
        strcpy(value, str);
    }
    else
        value = str;

    n->value = value;
    n->next = NULL;
}

int HS_contains(Hashset *set, char *str)
{
    unsigned long hash = djb2(str) % set->hash_size;

    if(!set->buckets[hash])
        return 0;

    node *n = set->buckets[hash];
    for(; n; n = n->next)
        if(strcmp(n->value, str) == 0)
            return 1;

    return 0;
}

int HS_count(Hashset *set)
{
    int count = 0;

    int i;
    for(i = 0; i < set->hash_size; i++)
    {
        node *n = set->buckets[i];
        if(!n)
            continue;

        for(; n; n = n->next)
            count++;
    }

    return count;
}

void HS_remove(Hashset *set, char *str)
{
    unsigned long hash = djb2(str) % set->hash_size;

    if(!set->buckets[hash])
        return;

    node *n = set->buckets[hash];
    node *prev = NULL;
    for(; n; n = n->next)
    {
        if(strcmp(n->value, str))
        {
            prev = n;
            continue;
        }

        node *next = n->next;

        if(prev)
            prev->next = next;
        else
            set->buckets[hash] = next;

        if(set->copies_str)
            free(n->value);
        free(n);

        break;
    }
}

void HS_free(Hashset *set)
{
    int i;
    for(i = 0; i < set->hash_size; i++)
    {
        node *n = set->buckets[i];
        if(!n)
            continue;

        while(n)
        {
            node *next = n->next;
            if(set->copies_str)
                free(n->value);
            free(n);
            n = next; 
        }
    }

    free(set->buckets);
}

void hs_list_add(Hashlist **node, char *value)
{
    Hashlist *top = *node;
    if(top)
    {
        for(; top->next; top = top->next);
    }

    Hashlist *next = malloc(sizeof(Hashlist));
    next->value = value;
    next->next = NULL;
    
    if(top)
        top->next = next;
    else
        *node = next;
}

Hashlist *HS_to_list(Hashset *set)
{
    Hashlist *retval = NULL;

    int i;
    for(i = 0; i < set->hash_size; i++)
    {
        node *n = set->buckets[i];

        if(!n)
            continue;

        for(; n; n = n->next)
            hs_list_add(&retval, n->value);
    }

    return retval;
}