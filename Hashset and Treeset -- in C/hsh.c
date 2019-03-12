#include <string.h>

#define PUT_ERROR(STR) fprintf(stderr, STR); exit(EXIT_FAILURE)
#define LEN_INITIAL 21
#define ALPHABET 26
#define REHASH_CONDITION 2/3
#define REHASH_MULTI 10

void link_free(word * target);
int hash(char* v, dic* s);
void rehash(dic* s,int biggerlen);

dic* dic_init(int size)
{   dic *s;
    s = calloc(1,sizeof(dic));
    s->lib = (word **) calloc(LEN_INITIAL,sizeof(word*));
    if(s->lib == NULL)
    {   PUT_ERROR("Memory request for new library array fails\n");
    }
    s->len = LEN_INITIAL;
    s->wordsize = size;
    s->count = 0;
    return s;
}

void dic_insert(dic* s, char* v)
{    int index;
     word *target, *new;
     word **temp;
    
     /* rehash when condition is met */
     if(s->count > (s->len*REHASH_CONDITION))
     {   rehash(s,s->len*REHASH_MULTI);
     }

     /* hashing */
     index = hash(v,s);

     /* create a new word */
     new = (word *)calloc(1,sizeof(word));
     new->words = (char *)calloc(s->wordsize,sizeof(char));
     if(new->words == NULL)
     {   PUT_ERROR("Memory request for new word fails\n");
     }
     strcpy(new->words, v);

     /* find empty space and insert this word */
     target = s->lib[index];
     temp = s->lib+index;
     while(target != NULL)
     {    temp = &(target->word_next);
          target = target->word_next;
     }
     *temp = new;
     s->count++;
}

bool dic_isin(dic*s, char*v)
{   int index;
    word* target;
    
    /* hashing */
    index = hash(v,s);

    target = s->lib[index];
    while(target != NULL)
    {   if(strcmp(v,target->words) == 0)
        {   return TRUE;
        }
        target = target->word_next;
    }
    return FALSE;
}

void dic_free(dic **d)
{   int i;
    dic *p = *d;

    for(i=0;i<p->len;i++)
    {    if((p->lib[i]) != NULL)
         {  link_free(p->lib[i]);
         }
    }

    free(p->lib);
    free(p);
    *d = NULL;
}

void link_free(word *target)
{   if(target->word_next != NULL)
    {   link_free(target->word_next);
    }
    free(target->words);
    free(target);
}

int hash(char* v, dic *s)
{    long unsigned i;
     long unsigned sum = 0;

     for(i=0;v[i]!='\0';i++)
     {   sum = sum * ALPHABET + (v[i]-'a');
     }
     return (int) (sum % (s->len));
}

void rehash(dic* s, int biggerlen)
{    dic *temp;
     word *target;
     int i;
    
     /* create a buffer dictionary */
     temp = (dic*)calloc(1,sizeof(dic));
     temp->lib = (word **)calloc(biggerlen,sizeof(word*));
     if(temp->lib == NULL)
     {   PUT_ERROR("Memory request for temporary libaray fails\n");
     }
     temp->len = biggerlen;
     temp->wordsize = s->wordsize;
     temp->count = 0;

     /* rehashing the libaray */
      for(i=0;i<s->len;i++)
      {   target = s->lib[i];
          while(target != NULL)
          {   dic_insert(temp, target->words);
              target = target->word_next;
          }
      }

      /* free old lib */
      for(i=0;i<s->len;i++)
      {   if((s->lib[i]) != NULL)
          {  link_free(s->lib[i]);
          }
      }
      free(s->lib);

      /* pointing to new lib */
      s->len = biggerlen;
      s->lib = temp->lib;

      /* clean buffer dictionary (while keeping its lib) */
      free(temp);
}
