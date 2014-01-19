#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "definitions.h"

void dict_load();

BOOL dict_is_word(char *word);
BOOL dict_is_prefix(char *prfx);

void dict_clean_up();

#endif