
int main(int argc, char **argv)
{   dic *dl;
    int i;
    FILE* fw;
    char str[50];
    int index;
    word *target;
    dl=dic_init(50);
    fw = fopen(argv[1], "r");
    if(!fw){
       fprintf(stderr, "Can't open %s or %s\n", argv[1]);
       exit(1);
    }

    while(fscanf(fw, "%s", str) == 1){
       dic_insert(dl, str);
    }
    fclose(fw);
    printf("\n%d words have been recorded\n",dl->count);
    printf("nellie is in? %d\n", dic_isin(dl,"nellie"));
    printf("%d is the index\n",index=hash("nellie",dl));
     printf("base line is %d\n",dl->len);
    target=dl->lib[index];
    while(target!=NULL)
    {
      printf("%s is in that index\n",target->words);
      target=target->word_next;
    }
    return 0;
}
/*
d=dic_init(50);
for(i=0;i<155;i++)
{
   dic_insert(d,"ffff");
}
printf("\n%d\n",dic_isin(d,"ffff"));

dic_free(&d);
assert(d==NULL);
return 0;*/

dic* dic_init(int size)
{   dic *s;
    s = calloc(1,sizeof(dic));
    s->lib = (word **) calloc(LEN_INITIAL,sizeof(word*));
    s->len = LEN_INITIAL;
    s->wordsize = size;
    s->count=0;
    return s;
}

void dic_insert(dic* s, char* v)
{    int index;
     word *target, *new;
     word **temp;
     char n[]="nellie";
     /* hash expand*/
     if( (s->count/2)>s->len )
     {   printf("\n\nrehash conditions met,new len is %d\n",s->count);
         rehash(s,s->count);
         printf("rehash completed\n\n");
     }

     /* hashing */
     index = hash(v,s);
     if(strcmp(n,v)==0)
     {
     printf("nellie is now in %d\n",index);
     printf("base line is %d\n",s->len);
     }
     /* insert word when there is no collision */
     new = (word *)calloc(1,sizeof(word));
     new->words= (char *)calloc(s->wordsize,sizeof(char));

     strcpy(new->words, v);

     target = s->lib[index];
     temp = s->lib+index;
     while(target!=NULL)
     {    temp = &(target->word_next);
          target = target->word_next;
     }
     *temp = new;

     /*printf("words _%s_ recorded\n",(*temp)->words);*/
     s->count++;
     /*printf("dic has %d words \n",s->count);*/
}

int dic_isin(dic*s, char*v)
{   int index;
    word* target;
    /*hashing*/
    index = hash(v,s);
    target = s->lib[index];
    while(target!=NULL)
    {   if(strcmp(v,target->words)==0)
        {   return 1;
        }
        target=target->word_next;
    }
    return 0;
}

void dic_free(dic **d)
{   int i;
    dic *p = *d;

    for(i=0;i<p->len;i++)
    {    if((p->lib[i])!=NULL)
         {  link_free(p->lib[i]);
         }
    }

    free(p->lib);
    free(p);
    *d=NULL;
}

void link_free(word *target)
{   if(target->word_next==NULL)
    {   free(target->words);
        free(target);
    }
    else
    {   link_free(target->word_next);
        free(target->words);
        free(target);
    }
}

int hash(char* v, dic *s)
{    int i,ii;
     int base=1,sum=0;

     for(i=0;v[i]!='\0';i++)
     {   for(ii=0;ii<i;ii++)
         {   base *= 'z'-'a'+1;
         }
         sum += (v[i]-'a')*base;
     }
     if (sum<0)
     {   sum=-sum;
     }
     return sum % (s->len);
}

void rehash(dic* s, int biggerlen)
{    dic *temp;
     int i;
     word *target;
     printf("\nrehash is happening!!\n");
     temp = dic_init(50);
     temp->lib=(word **)calloc(biggerlen,sizeof(word*));
     temp->len=biggerlen;

     /* rehashing the libaray*/
      for(i=0;i<s->len;i++)
      {   target=s->lib[i];
          while(target!=NULL)
          {   dic_insert(temp, target->words);
              target = target->word_next;
          }
      }
      printf("\nredo complete");

      /*free old lib*/
      for(i=0;i<s->len;i++)
      {   if((s->lib[i])!=NULL)
          {  link_free(s->lib[i]);
          }
      }
      free(s->lib);

      printf("\nfree complete\n");
      /* pointing to new lib*/
      s->len=biggerlen;
      s->lib=temp->lib;

      /*clean buffer dictionary */
      free(temp);
}
