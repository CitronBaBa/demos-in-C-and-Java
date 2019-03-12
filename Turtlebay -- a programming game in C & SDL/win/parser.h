/*//
//  parser.h
//  Turtle-project
//
//  Created by 王朝 on 18/12/2018.
//  Copyright © 2018 王朝. All rights reserved.
//*/
#include <windows.h>
#include "SDL.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <time.h>

#define MAXPROG 100
#define MAXWORD 100
#define MAXSTACK 100
#define MAXVAR 100
#define PUTERROR(WARNING) {fprintf(stderr,"Error:'%s' occurred in %s line %d\n",WARNING,__FILE__,__LINE__); exit(2); }
#define strsame(A,B) (strcmp(A, B)==0)

#define WWIDTH 800
#define WHEIGHT 600
#define SDL_8BITCOLOUR 256
#define R_PI 3.14159265358979323846

struct stack
{   int hill[MAXSTACK];
    int top;
};
typedef struct stack stack;

struct variable
{   int value;
    char name[MAXWORD];
};
typedef struct variable variable;

struct position
{   int x;
    int y;
    double angle;
};
typedef struct position position;

struct SDL_Implement
{  SDL_bool finished;
   SDL_Window *window;
   SDL_Renderer *renderer;
   SDL_Texture *surface;
};
typedef struct SDL_Implement SDL_Implement;

struct program
{   char text[MAXPROG][MAXWORD];
    variable varlist[MAXVAR];
    stack stk;
    int loc;
    position state;
    SDL_Implement sdlobject;
};
typedef struct program program;

enum operand {add,sub,mul,divd};
typedef enum operand operand;



void whole(program *prog);
void instrctlist(program *prog);
void instruction(program *prog);
int varnum(program *prog);
void var(program *prog);
operand op(program *prog);
void set(program *prog);
void polish(program *prog);
void doloop(program *prog);


void push(program *prog, int value);
int pop(program *prog);
void setvariable(program *prog, char* varname, int value);
int findvariable(program *prog, char* varname);

void move_draw(program *prog, int length);
void shift_angle(program *prog, int angle);
void Graph_init(SDL_Implement* sdlobject);
void Graph_SetRenderDrawColor(SDL_Implement  *sdlobject, Uint8 r, Uint8 g, Uint8 b);
void Graph_UpdateScreen(SDL_Implement  *sdlobject);
void Graph_Events(SDL_Implement  *sdlobject);
