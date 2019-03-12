/*
 Created by 王朝 on 18/12/2018.
*/

void prog_init(program *prog)
{   int i;
    prog->loc=0;
    prog->stk.top=0;
    prog->state.x=WWIDTH/2;
    prog->state.y=WHEIGHT/2;
    prog->state.angle=0;
    prog->errorflag=FALSE;
    for(i=0;i<MAXPROG;i++)
    {   prog->text[i][0]='\0';
    }
    for(i=0;i<MAXVAR;i++)
    {   prog->varlist[i].name[0]='\0';
    }
}

void prog_sdlinit(program *prog)
{   SDL_Implement* sdlobject = &(prog->sdlobject);
    if (prog->drawflag)
    {   Graph_init(sdlobject);
        Graph_SetRenderDrawColor(sdlobject, rand()%SDL_8BITCOLOUR,
                                            rand()%SDL_8BITCOLOUR,
                                            rand()%SDL_8BITCOLOUR);
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
{   if(prog->errorflag)
    {   return;
    }
    if(strsame(prog->text[prog->loc],"}"))
    {    prog->loc++;
         return;
    }
    instruction(prog);
    instrctlist(prog);
}

void instruction(program *prog)
{   char *s=prog->text[prog->loc];

    /* where we record exit instruction*/
    if(prog->drawflag)
    {   Graph_Events(&(prog->sdlobject));
    }
    if(prog->sdlobject.finished && prog->drawflag)
    {   exit(0);
    }

    if(strsame(s,"FD"))
    {    prog->loc++;
         move_draw(prog, varnum(prog));
         return;
    }
    if(strsame(s,"LT"))
    {    prog->loc++;
         shift_angle(prog, (-1)*varnum(prog));
         return;
    }
    if(strsame(s,"RT"))
    {    prog->loc++;
         shift_angle(prog, varnum(prog));
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

double varnum(program *prog)
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
        if(prog->errorflag)
        {   return sum;
        }
        sum = findvariable(prog, prog->text[prog->loc]);
        if(prog->errorflag)
        {   return sum;
        }
    }
    prog->loc++;
    return sum;
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
    PUTERROR("Incorrect operand entered");
    return divd;
}

void set(program *prog)
{   char* varname;

    var(prog);
    varname = prog->text[prog->loc];
    prog->loc++;

    if(!strsame(prog->text[prog->loc],":="))
    {   PUTERROR("SET syntax wrong");
    }
    prog->loc++;

    polish(prog);
    setvariable(prog,varname,pop(prog));
}

void polish(program *prog)
{   char c=prog->text[prog->loc][0];
    double value_a, value_b;
    operand operate;

    if(c==';')
    {   prog->loc++;
        if(prog->stk.top!=1)
        {   PUTERROR("artihmetical euqation is incomplete")
        }
        return;
    }

    if(c=='+' || c=='-'|| c=='*'|| c=='/')
    {   operate=op(prog);
        if(prog->errorflag)
        {   return;
        }
        value_b=pop(prog);
        value_a=pop(prog);
        if(operate==add)
        {   push(prog,value_a+value_b);
        }
        if(operate==sub)
        {   push(prog,value_a-value_b);
        }
        if(operate==mul)
        {   push(prog,value_a*value_b);
        }
        if(operate==divd)
        {   push(prog,value_a/value_b);
        }
        if(prog->errorflag)
        {   return;
        }
        polish(prog);
        return;
    }

    push(prog,varnum(prog));
    if(prog->errorflag)
    {   return;
    }
    polish(prog);
    return;
}

void doloop(program *prog)
{   char *varname;
    int from,to,i;
    int loop_location;

    var(prog);
    if(prog->errorflag)
    {   return;
    }
    varname = prog->text[prog->loc++];

    if(!strsame(prog->text[prog->loc++],"FROM"))
    {   PUTERROR("Do loop missing specifier 'FROM'");
    }

    from = (int)varnum(prog);
    if(prog->errorflag)
    {   return;
    }

    if(!strsame(prog->text[prog->loc++],"TO"))
    {   PUTERROR("Do loop missing specifier 'TO'");
    }

    to = (int)varnum(prog);
    if(prog->errorflag)
    {   return;
    }

    if(!strsame(prog->text[prog->loc++],"{"))
    {   PUTERROR("Do loop missing specifier '{'");
    }

    for(i=from, loop_location=prog->loc ;i<=to ;i++)
    {   if(prog->errorflag)
        {   return;
        }
        setvariable(prog,varname,i);
        prog->loc=loop_location;
        instrctlist(prog);
    }
}


void push(program *prog, double value)
{   if(prog->stk.top>=MAXSTACK)
    {   PUTERROR("stack is overloaded");
    }
    prog->stk.hill[prog->stk.top]=value;
    prog->stk.top++;
}

double pop(program *prog)
{   if(prog->stk.top<=0)
    {   PUTERROR("can not pop from an empty stack")
    }
    prog->stk.top--;
    return prog->stk.hill[prog->stk.top];
}

double findvariable(program *prog, char* varname)
{   int i;
    for(i=0; (prog->varlist[i].name[0])!='\0'; i++)
    {   if(strsame(prog->varlist[i].name,varname))
            {   return prog->varlist[i].value;
            }
    }
    PUTERROR("Variable used before declaration");
    return 0;
}

void setvariable(program *prog, char* varname, double value)
{   int i;
    /* find the variable or an empty space*/
    for(i=0; prog->varlist[i].name[0]!='\0'
            && !strsame(prog->varlist[i].name,varname) ;i++);
    strcpy(prog->varlist[i].name,varname);
    prog->varlist[i].value=value;
}

void move_draw(program *prog, double length)
{    double x0,y0,x1,y1,dx,dy;
     x0 = prog->state.x;
     y0 = prog->state.y;
     dx = length * cos(prog->state.angle);
     dy = length * sin(prog->state.angle);
     x1 = x0 + dx;
     y1 = y0 + dy;
     prog->state.x = x1;
     prog->state.y = y1;
     if (prog->drawflag)
     {   SDL_RenderDrawLine(prog->sdlobject.renderer, (int)x0, (int)y0, (int)x1, (int)y1);
         Graph_UpdateScreen(&(prog->sdlobject));
         SDL_Delay(25);
     }
}

void shift_angle(program *prog, double angle)
{    double angle_r;
     angle_r = R_PI * angle / 180;
     prog->state.angle += angle_r;
     while(prog->state.angle > 2*R_PI)
     {   prog->state.angle -= 2*R_PI;
     }
}
