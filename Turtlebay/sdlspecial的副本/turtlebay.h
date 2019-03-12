#define PUTERROR(WARNING) {fprintf(stderr,"Error:'%s' occurred in %s line %d\n",WARNING,__FILE__,__LINE__); exit(2); }
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <SDL_image.h>

typedef unsigned short fntrow;

#define ARTWIDTH 400
#define ARTHEIGHT 300


struct article
{   SDL_Renderer *artrenderer;
    SDL_Window *artwindow;
    char words[ARTWIDTH*ARTHEIGHT];
    int sizeinfo[ARTWIDTH*ARTHEIGHT][3];
    int line;
    int position;
    int x0;
    int y0;
    int x;
    int y;
    int dx;
    int dy;
    int win_width;
    int win_height;
};
typedef struct article article;

struct turtlebay
{   article *article;
    SDL_Window *window;
    SDL_Renderer *renderer;
};
typedef struct turtlebay turtlebay;

#define FONTHEIGHT 44
#define WWIDTH 1000
#define WHEIGHT 800
#define SDL_8BITCOLOUR 256
#define R_PI 3.14159265358979323846
#define NONCHAR '@'
enum bool {FALSE,TRUE};
typedef enum bool bool;

void init(turtlebay *turtlebay);
void article_set(article *article, int x, int y, int width, int height);
void article_render(article *article, char *s, SDL_Color textColor, TTF_Font *font);
void article_back(article *article);
void article_sync_char(article *article, char *s, SDL_Color textColor, TTF_Font *font);
void Render_Character(article *article, char* s, int x, int y,SDL_Color textColor,TTF_Font *font);
char key_input(article *article, SDL_Scancode key, SDL_Color textColor, TTF_Font *font);
bool inputlisten(article *article, SDL_Color textcolor,TTF_Font *font);


/*----------------------parsing----------------*/
/*----------------------parsing----------------*/
/*----------------------parsing----------------*/
/*----------------------parsing----------------*/
#define MAXPROG 100
#define MAXWORD 100
#define MAXSTACK 100
#define MAXVAR 100
#define strsame(A,B) (strcmp(A, B)==0)


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
    SDL_Renderer *renderer;
    SDL_Window *window;
};
typedef struct program program;

enum operand {add,sub,mul,divd};
typedef enum operand operand;

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
