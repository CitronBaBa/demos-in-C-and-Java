#include <stdio.h>
#include <stdlib.h>

struct node
{   char *word;
    struct node *left;
    struct node *right;
};
typedef struct node node;

struct dic
{   node *source;
    int size;
};
typedef struct dic dic;

enum bool {FALSE,TRUE};
typedef enum bool bool;

dic* dic_init(int size);
void dic_insert(dic* s, char* v);
bool dic_isin(dic*s, char *v);
void dic_free(dic **s);
