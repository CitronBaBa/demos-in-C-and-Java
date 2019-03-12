/*
Created by 王朝 on 18/12/2018.
*/
#include <SDL.h>
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
#define PUTERROR(WARNING) {fprintf(stderr,"Error:'%s' occurred in %s line %d\n",\
WARNING,__FILE__,__LINE__); exit(2);}
#define strsame(A,B) (strcmp(A, B)==0)
#define WWIDTH 1000
#define WHEIGHT 800
#define SDL_8BITCOLOUR 256
#define R_PI 3.14159265358979323846

enum operand {add,sub,mul,divd};
typedef enum operand operand;
enum bool {FALSE, TRUE};
typedef enum bool bool;

struct stack
{   double hill[MAXSTACK];
    int top;
};
typedef struct stack stack;

struct variable
{   double value;
    char name[MAXWORD];
};
typedef struct variable variable;

struct position
{   double x;
    double y;
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
    bool errorflag;
    bool drawflag;
};
typedef struct program program;

void prog_init(program *prog);
void prog_sdlinit(program *prog);
void prog_readwords(program *prog, FILE *fp);
void whole(program *prog);
void instrctlist(program *prog);
void instruction(program *prog);
double varnum(program *prog);
void var(program *prog);
operand op(program *prog);
void set(program *prog);
void polish(program *prog);
void doloop(program *prog);
void push(program *prog, double value);
double pop(program *prog);
void setvariable(program *prog, char* varname, double value);
double findvariable(program *prog, char* varname);
void move_draw(program *prog, double length);
void shift_angle(program *prog, double angle);
void Graph_init(SDL_Implement* sdlobject);
void Graph_SetRenderDrawColor(SDL_Implement  *sdlobject, Uint8 r, Uint8 g, Uint8 b);
void Graph_UpdateScreen(SDL_Implement  *sdlobject);
void Graph_Events(SDL_Implement  *sdlobject);
