#include <stdio.h>
#include <stdlib.h>

struct word
{   char* words;
    struct word* word_next;
};
typedef struct word word;

struct dictionary
{   word **lib;
    int wordsize;
    int len;
    int count;
};
typedef struct dictionary dic;

enum bool {FALSE,TRUE};
typedef enum bool bool;

dic* dic_init(int size);
void dic_insert(dic* s, char* v);
bool dic_isin(dic*s, char *v);
void dic_free(dic **s);

