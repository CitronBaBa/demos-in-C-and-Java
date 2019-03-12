/*
--  parser.c
--  Created by 王朝 on 18/12/2018.
*/
void prog_init(program *prog)
{   int i;
    prog->loc=0;
    for(i=0;i<MAXPROG;i++)
    {   prog->text[i][0]='\0';
    }
}

void prog_readwords(program *prog, FILE *fp)
{   int i=0;
    char s[MAXWORD];
    while(fscanf(fp,"%s",s)==1 && i<MAXPROG)
    {    if (strlen(s)<MAXWORD)
         {   strcpy(prog->text[i++],s);
         }
         else
         {   fprintf(stderr,"one token exceeds the length limitation\n");
             exit(2);
         }
    }
    assert(i<MAXPROG);
}

void whole(program *prog)
{   if(!strsame(prog->text[prog->loc++],"{"))
    {   PUTERROR("The first bracket is missing");
    }
    instrctlist(prog);
}

void instrctlist(program *prog)
{   if(strsame(prog->text[prog->loc],"}"))
    {    prog->loc++;
         return;
    }
    instruction(prog);
    instrctlist(prog);
}

void instruction(program *prog)
{   char *s=prog->text[prog->loc];

    if(strsame(s,"FD"))
    {    prog->loc++;
         varnum(prog);
         return;
    }
    if(strsame(s,"LT"))
    {    prog->loc++;
         varnum(prog);
         return;
    }
    if(strsame(s,"RT"))
    {    prog->loc++;
         varnum(prog);
         return;
    }
    if(strsame(s,"DO"))
    {    prog->loc++;
         doloop(prog);
         return;
    }
    if(strsame(s,"SET"))
    {    prog->loc++;
         set(prog);
         return;
    }
    PUTERROR("Instruction is not compatible");
}

void varnum(program *prog)
{   int i,ii;
    double sum=0;
    bool decimal=FALSE;
    char c;

    c=prog->text[prog->loc][0];
    if(c>='0' && c<='9')
    {   for(i=0,ii=0;(c=prog->text[prog->loc][i])!='\0';i++)
        {    if(c>='0' && c<='9')
             {   sum = sum*10 + (double)(c-'0');
                 if (decimal == TRUE)
                 {   ii++;
                 }
             }
             else if(c=='.' && decimal== FALSE)
             {   decimal = TRUE;
             }
             else
             {   PUTERROR("Please enter a valid number after the instruction");
             }
        }
        for(;ii>0;ii--)
        {   sum /= 10;
        }
    }
    else
    {   var(prog);
    }
    prog->loc++;
}

void var(program *prog)
{   int i;
    char c;
    for(i=0;(c=prog->text[prog->loc][i])!='\0';i++)
    {   if(  !((c>='a' && c<='z')||( c>='A' && c<='Z'))  )
        {    PUTERROR("Variable name is not allowed");
        }
    }
    if(i==0)
    {   PUTERROR("Variable name is not allowed");
    }
}

operand op(program *prog)
{   char* s=prog->text[prog->loc];
    if(strsame(s,"+"))
    {    prog->loc++;
         return add;
    }
    if(strsame(s,"-"))
    {    prog->loc++;
         return sub;
    }
    if(strsame(s,"*"))
    {    prog->loc++;
         return mul;
    }
    if(strsame(s,"/"))
    {    prog->loc++;
         return divd;
    }
    PUTERROR("Incorrect operand entered\n");
}

void set(program *prog)
{   char* varname;

    var(prog);
    varname = prog->text[prog->loc];
    prog->loc++;

    if(!strsame(prog->text[prog->loc],":="))
    {   PUTERROR("SET syntax wrong\n");
    }
    prog->loc++;

    polish(prog);
}

void polish(program *prog)
{   char c=prog->text[prog->loc][0];
    operand operate;
    if(c==';')
    {   prog->loc++;
        return;
    }

    if(c=='+' || c=='-'|| c=='*'|| c=='/')
    {   operate=op(prog);
        polish(prog);
        return;
    }

    varnum(prog);
    polish(prog);
    return;
}

void doloop(program *prog)
{   char *varname;
    
    var(prog);
    varname = prog->text[prog->loc++];

    if(!strsame(prog->text[prog->loc++],"FROM"))
    {   PUTERROR("Do loop missing specifier 'FROM'");
    }

    varnum(prog);

    if(!strsame(prog->text[prog->loc++],"TO"))
    {   PUTERROR("Do loop missing specifier 'TO'");
    }

    varnum(prog);

    if(!strsame(prog->text[prog->loc++],"{"))
    {   PUTERROR("Do loop missing specifier '{'");
    }

    instrctlist(prog);
}
