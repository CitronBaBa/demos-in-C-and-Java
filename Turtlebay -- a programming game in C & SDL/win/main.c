/*
//  main.c
//  Turtle-project
//
//  Created by 王朝 on 18/12/2018.
//  Copyright © 2018 王朝. All rights reserved.

    解决小数问题；
    判断字长；
    解决停顿问题；
    解决退出问题；
    makrfile机制；
    地址~；
*/

#include "parser.h"

int main (int argc, char* args[])
{   program prog;
    FILE *fp;
    int i;
    char *filename="test.txt";
    SDL_Implement* sdlobject=&(prog.sdlobject);
    srand(time(0));
    Graph_init(sdlobject);
    Graph_SetRenderDrawColor(sdlobject, rand()%SDL_8BITCOLOUR,
                                        rand()%SDL_8BITCOLOUR,
                                        rand()%SDL_8BITCOLOUR);
    prog.loc=0;
    prog.stk.top=0;
    prog.state.x=WWIDTH/2;
    prog.state.y=WHEIGHT/2;
    prog.state.angle=0;

    for(i=0;i<MAXPROG;i++)
    {   prog.text[i][0]='\0';
    }
    for(i=0;i<MAXVAR;i++)
    {   prog.varlist[i].name[0]='\0';
    }
    i=0;

    if(!(fp=fopen(filename,"r")))
    {   fprintf(stderr,"Can not open file %s\n",filename);
        exit(2);
    }

    /* DO REMBER TO CHECK WORD LEN; */
    while(fscanf(fp,"%s",prog.text[i++])==1 && i<MAXPROG);
    assert(i<MAXPROG);

    /* Parsing */
    whole(&prog);
    SDL_Delay(2500);
    SDL_Quit();
    atexit(SDL_Quit);
    return 0;
}
