/*
--  parser.c
--  Turtle-project
--
--  Created by 王朝 on 18/12/2018.
--  Copyright © 2018 王朝. All rights reserved.
*/

#include "parser.h"

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
    
    /* where we record exit instruction*/
    Graph_Events(&(prog->sdlobject));
    if(prog->sdlobject.finished)
    {   exit(0);
    }
    
    if(strsame(s,"FD"))
    {    prog->loc++;
         move_draw(prog, varnum(prog));
         return;
    }
    if(strsame(s,"LT"))
    {    prog->loc++;
         shift_angle(prog, varnum(prog));
         return;
    }
    if(strsame(s,"RT"))
    {    prog->loc++;
         shift_angle(prog, (-1)*varnum(prog));
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
        {    if(c<='9' && c>='0' && decimal==FALSE)
             {   sum = sum*10 + (c-'0');
             }
             else if(c=='.' && decimal== FALSE)
             {   decimal = TRUE;
             }
             else if(c>='0' && c<='9' && decimal==TRUE )
             {   ii++;
             }
             else
             {   PUTERROR("Please enter a number after the instruction");
             }
        }
        for(;ii>0;ii--)
        {   sum /= 10;
        }
    }
    else
    {   var(prog);
        sum = findvariable(prog, prog->text[prog->loc]);
    }

    printf("the number is %lf \n",sum);
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
    setvariable(prog,varname,pop(prog));
}

void polish(program *prog)
{   char c=prog->text[prog->loc][0];
    double value_a, value_b;
    operand operate;
    if(c==';')
    {   prog->loc++;
        printf("arith result is %lf\n",prog->stk.hill[0]);
        return;
    }

    if(c=='+' || c=='-'|| c=='*'|| c=='/')
    {   operate=op(prog);
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
    polish(prog);

    return;
}

void doloop(program *prog)
{   char *varname;
    int from,to,i;
    int loop_location;

    var(prog);
    varname = prog->text[prog->loc++];

    if(!strsame(prog->text[prog->loc++],"FROM"))
    {   PUTERROR("Do loop missing specifier 'from'");
    }

    from = (int)varnum(prog);

    if(!strsame(prog->text[prog->loc++],"TO"))
    {   PUTERROR("Do loop missing specifier 'TO'");
    }

    to = (int)varnum(prog);

    if(!strsame(prog->text[prog->loc++],"{"))
    {   PUTERROR("Do loop missing specifier '{'");
    }

    for(i=from, loop_location=prog->loc ;i<=to ;i++)
    {   setvariable(prog,varname,i);
        prog->loc=loop_location;
        instrctlist(prog);
    }
}


void push(program *prog, double value)
{   assert(prog->stk.top<MAXSTACK);
    prog->stk.hill[prog->stk.top]=value;
    prog->stk.top++;
}

double pop(program *prog)
{   assert(prog->stk.top>0);
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
}

void setvariable(program *prog, char* varname, double value)
{   int i;
    for(i=0; prog->varlist[i].name[0]!='\0'
            && !strsame(prog->varlist[i].name,varname) ;i++);
    strcpy(prog->varlist[i].name,varname);
    prog->varlist[i].value=value;
    printf("variable %d : %s = %lf\n",i,prog->varlist[i].name,prog->varlist[i].value);
}

void move_draw(program *prog, double length)
{    int x0,y0,x1,y1,dx,dy;
     x0 = prog->state.x;
     y0 = prog->state.y;
     dx = (int)( length * cos(prog->state.angle));
     dy = (int)( length * sin(prog->state.angle));
     x1 = x0 + dx;
     y1 = y0 + dy;
     prog->state.x=x1;
     prog->state.y=y1;

     SDL_Delay(25);
     SDL_RenderDrawLine(prog->sdlobject.renderer, x0, y0, x1, y1);
     Graph_UpdateScreen(&(prog->sdlobject),prog);

}

void shift_angle(program *prog, double angle)
{    double angle_r;
     angle_r = R_PI * angle / 180;
     prog->state.angle += angle_r;
     while(prog->state.angle > 2*R_PI)
     {   prog->state.angle -= 2*R_PI;
     }
}
