/*
--  Created by 王朝 on 18/12/2018.
*/

#include "turtlebay.h"

void prog_init(program *prog)
{   int i;
    prog->loc = 0;
    prog->stk.top = 0;
    prog->state.x = WWIDTH*3/4;
    prog->state.y = WHEIGHT;
    prog->state.angle = INITIAL_ANGLE/180.0*R_PI;
    prog->terminate = FALSE;
    prog->parsermode = parsing;

    for(i=0;i<MAXPROG;i++)
    {   prog->text[i][0]='\0';
    }
    for(i=0;i<MAXVAR;i++)
    {   prog->varlist[i].name[0]='\0';
    }
}

void parser_operation(program *prog)
{    prog->parsermode = parsing;
     whole(prog);
     prog->loc = 0;
     if ( prog->terminate == FALSE)
     {   prog->parsermode = interpreting;
         whole(prog);
     }
     prog->loc = 0;
}

void whole(program *prog)
{   if(!strsame(prog->text[prog->loc++],"{"))
    {   TERMINATE("The first bracket is missing");
    }
    instrctlist(prog);
    if (prog->terminate)
    {   return;
    }
}

void instrctlist(program *prog)
{   if (prog->terminate)
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
    double value;
    if(prog->terminate)
    {   return;
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
    if(strsame(s,"ROTATE"))
    {    prog->loc++;
         value = varnum(prog)/180*R_PI;
         if(prog->parsermode == parsing)
         {   return;
         }
         else
         {   prog->turtlebay->rotate_angle = value;
         }
         return;
    }
    if(strsame(s,"HARD"))
    {    prog->loc++;
         value = varnum(prog);
         if(prog->parsermode == parsing)
         {   return;
         }
         else
         {   prog->turtlebay->hardness = value;
         }
         return;
    }
    TERMINATE("Instruction is not compatible");
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
             {   TERMINATE("Please enter a valid number after the instruction");
             }
        }
        for(;ii>0;ii--)
        {   sum /= 10;
        }
    }
    else
    {   var(prog);
        if(prog->terminate)
        {   return 1;
        }
        sum = findvariable(prog, prog->text[prog->loc]);
        if(prog->terminate)
        {   return 1;
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
        {    TERMINATE("Variable name is not allowed");
        }
    }
    if(i==0)
    {   TERMINATE("Variable name is not allowed");
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
    TERMINATE("Incorrect operand entered\n");
    return divd;
}

void set(program *prog)
{   char* varname;

    var(prog);
    if(prog->terminate)
    {   return;
    }
    varname = prog->text[prog->loc];
    prog->loc++;

    if(!strsame(prog->text[prog->loc],":="))
    {   TERMINATE("SET syntax wrong\n");
    }
    prog->loc++;

    polish(prog);
    if(prog->terminate)
    {   return;
    }

    setvariable(prog,varname,pop(prog));
    if(prog->terminate)
    {   return;
    }
}

void polish(program *prog)
{   char c=prog->text[prog->loc][0];
    double value_a, value_b;
    operand operate;
    if(prog->terminate)
    {   return;
    }
    if(c==';')
    {   prog->loc++;
        if (prog->stk.top != 1)
        {    TERMINATE("Polish Equation is wrong");
        }
        return;
    }

    if(c=='+' || c=='-'|| c=='*'|| c=='/')
    {   operate=op(prog);
        if(prog->terminate)
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
        polish(prog);
        return;
    }

    push(prog,varnum(prog));
    if(prog->terminate)
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
    if(prog->terminate)
    {   return;
    }
    varname = prog->text[prog->loc++];

    if(!strsame(prog->text[prog->loc++],"FROM"))
    {   TERMINATE("Do loop missing specifier 'from'");
    }

    from = (int)varnum(prog);
    if(prog->terminate)
    {   return;
    }

    if(!strsame(prog->text[prog->loc++],"TO"))
    {   TERMINATE("Do loop missing specifier 'TO'");
    }

    to = (int)varnum(prog);
    if(prog->terminate)
    {   return;
    }

    if(!strsame(prog->text[prog->loc++],"{"))
    {   TERMINATE("Do loop missing specifier '{'");
    }

    for(i=from, loop_location=prog->loc ;i<=to ;i++)
    {   setvariable(prog,varname,i);
        prog->loc=loop_location;
        instrctlist(prog);
        if(prog->terminate)
        {   return;
        }
    }
}

void push(program *prog, double value)
{   if(prog->stk.top>=MAXSTACK)
    {   TERMINATE("Stack is overloaded, cannot push");
    }
    prog->stk.hill[prog->stk.top]=value;
    prog->stk.top++;
}

double pop(program *prog)
{   if(prog->stk.top<=0)
    {   TERMINATE("Stack is empty, cannot pop");
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
    TERMINATE("Variable used before declaration");
    return 0;
}

void setvariable(program *prog, char* varname, double value)
{   int i;
    for(i=0; prog->varlist[i].name[0]!='\0'
            && !strsame(prog->varlist[i].name,varname) ;i++);
    strcpy(prog->varlist[i].name,varname);
    prog->varlist[i].value=value;
}

void move_draw(program *prog, double length)
{    double x0,y0,x1,y1,dx,dy;
     if(prog->parsermode == parsing)
     {   return;
     }
     x0 = prog->turtlebay->x;
     y0 = prog->turtlebay->y;
     dx =  length * cos(prog->turtlebay->angle);
     dy =  length * sin(prog->turtlebay->angle);
     x1 = x0 + dx;
     y1 = y0 + dy;
     turtle_crawling(prog->turtlebay,x0, y0, x1, y1, TURTLE_MOVE_DELAY);
}

void shift_angle(program *prog, double angle)
{    double angle_r;
     if (prog->parsermode == parsing)
     {   return;
     }
     angle_r = R_PI * angle / 180;
     prog->state.angle = prog->turtlebay->angle;
     prog->state.angle += angle_r;
     turtle_shifting(prog->turtlebay,prog->state.angle, TURTLE_MOVE_DELAY);
}
