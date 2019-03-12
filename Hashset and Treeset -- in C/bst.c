#include <string.h>
#define PUT_ERROR(STR) fprintf(stderr, STR); exit(EXIT_FAILURE)

void recur_free(node *current);
bool recur_isin(node* source, char* v);
void recur_insert(node* current, char* v, dic* tree);
node* createnode(dic* tree);

dic* dic_init(int size)
{   dic *tree;
    tree = (dic*)calloc(1,sizeof(dic));
    if(tree == NULL)
    {   PUT_ERROR("Memory request for dictionary failled \n");
    }
    tree->size = size;
    return tree;
}
void dic_insert(dic* tree, char* v)
{    if(tree->source == NULL)
     {   tree->source = createnode(tree);
         strcpy(tree->source->word,v);
     }
     else
     {   recur_insert(tree->source,v,tree);
     }
}

void recur_insert(node* current, char* v, dic* tree)
{   if(strcmp(v,current->word) > 0)
    {   if(current->left == NULL)
        {   current->left = createnode(tree);
            strcpy(current->left->word,v);
        }
        else
        {   recur_insert(current->left,v,tree);
        }
    }
    if(strcmp(v,current->word) <= 0)
    {   if(current->right == NULL)
        {   current->right = createnode(tree);
            strcpy(current->right->word,v);
        }
        else
        {   recur_insert(current->right,v,tree);
        }
    }
}

bool dic_isin(dic* tree, char *v)
{   return recur_isin(tree->source,v);
}

bool recur_isin(node* current, char* v)
{   int difference;
    if(current == NULL)
    {   return FALSE;
    }
    if((difference = strcmp(v,current->word)) > 0)
    {   return recur_isin(current->left,v);
    }
    if(difference < 0)
    {   return recur_isin(current->right,v);
    }
    return TRUE;
}

void dic_free(dic **s)
{   dic *tree;
    tree = *s;
    if (tree->source != NULL)
    {   recur_free(tree->source);
        free(tree->source->word);
        free(tree->source);
    }
    free(tree);
    *s = NULL;
}

void recur_free(node *current)
{   if(current->left != NULL)
    {   recur_free(current->left);
        free(current->left->word);
        free(current->left);
    }
    if(current->right != NULL)
    {   recur_free(current->right);
        free(current->right->word);
        free(current->right);
    }
}

node* createnode(dic* tree)
{   node* current;
    current = (node*)calloc(1,sizeof(node));
    if(current == NULL)
    {   PUT_ERROR("Memory request for node failled \n");
    }
    current->word = (char*)calloc(tree->size,sizeof(char));
    if(current->word == NULL)
    {   PUT_ERROR("Memory request for the word failled \n");
    }
    return current;
}
