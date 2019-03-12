/*
--  Created by 王朝 on 18/12/2018.
*/
#define PUTERROR(WARNING) {fprintf(stderr,"Error:'%s' occurred in %s line %d\n", \
WARNING,__FILE__,__LINE__); exit(2); }
#define TERMINATE(WARNING) {fprintf(stdout,"Error:'%s' occurred in %s line %d\n",\
WARNING,__FILE__,__LINE__); \
clear_error(prog->errorbar);print_error(prog->errorbar,"Last error: ");\
prog->terminate = TRUE; print_error(prog->errorbar,WARNING);}

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <SDL_image.h>

typedef unsigned short fntrow;
#define FONTNAME "font/Candarab.ttf"
#define FONTHEIGHT 27
#define WWIDTH 1280
#define WHEIGHT 720
#define SDL_8BITCOLOUR 256
#define R_PI 3.14159265358979323846
#define NONCHAR '&'
#define MAXTEXSTACK 100
#define INTERVAL 2.0
#define ANGLEINTERVAL 5.0
#define INITIAL_ANGLE -90.0
#define RABBIT_Y_POSITION 0
#define RABBIT_SPEED_MIN 0.005
#define RABBIT_SPEED_MAX 0.015
#define SIZE_AJUST 1/3
#define FRAMETIME 10.0
#define TURTLE_MOVE_DELAY 30
#define BLINK_TIME 25
#define BLINK_INTER 3
#define RABBIT_MULTI_TIME 200
#define MAXRABBITS 20
#define HARDNESS_INITAL 1
#define PRECISION 0.0001

enum bool {FALSE,TRUE};
typedef enum bool bool;

struct rabbit
{   SDL_Texture* rabbit_tex;
    double x;
    double y;
    double velocity;
    double angle;
    double height;
    double width;
    struct rabbit *before;
    struct rabbit *next;
};
typedef struct rabbit rabbit;

struct texstack
{   SDL_Texture* hill[MAXTEXSTACK];
    double Textinfo[MAXTEXSTACK][3];
    int top;
};
typedef struct texstack texstack;

struct turtlebay
{   SDL_Texture *blackboard;
    SDL_Window *window;
    SDL_Renderer *renderer;
    texstack texstack;
    texstack rabbits_stack;
    rabbit rabbit0;
    int rabbits_count;
    double x;
    double y;
    int width;
    int height;
    double angle;
    double rotate_angle;
    SDL_Texture *cuteturtle;
    bool COLISIONflag;
    clock_t time0;
    clock_t time1;
    double hardness;
    Mix_Music *music;
    Mix_Chunk *boom;
    Mix_Chunk *laugh;
};
typedef struct turtlebay turtlebay;

struct article
{   SDL_Renderer *artrenderer;
    SDL_Window *artwindow;
    SDL_Texture *artboard;
    SDL_Texture *textboard;
    turtlebay *turtlebay;
    char *words;
    int **sizeinfo;
    SDL_Color articlecolor;
    SDL_Color textColor;
    int length;
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
    TTF_Font *font;
};
typedef struct article article;

void init(turtlebay *turtlebay);
void pushtex(texstack *texstack, SDL_Texture *tex, double width, double height, double angle);
void article_init(turtlebay *turtlebay, article *article, int x, int y, int width, int height, int fontheight,
  SDL_Color articlecolor, SDL_Color textColor);
void article_render(article *article, char s, SDL_Color textColor, TTF_Font *font);
void article_back(article *article);
void article_sync_char(article *article, char *s, SDL_Color textColor, TTF_Font *font);
void article_Render_Character(article *article, char* s, int x, int y,SDL_Color textColor,TTF_Font *font);
void article_Render_sentence(article *article, char* s, int x, int y, SDL_Color textColor);
void article_free(article *article);
char key_input(SDL_Scancode key);
bool inputlisten(article *article, SDL_Color textcolor);
void render(SDL_Renderer *renderer, SDL_Texture *carpet);
void render_turtlebay(turtlebay *turtlebay);
void draw_rect(SDL_Renderer *renderer, int x0, int y0, int width, int height, SDL_Color color);
void print_error(article *errorbar, char *s);
void clear_error(article *errorbar);
void draw_image(SDL_Renderer *renderer, char *picname, int x, int y, int wdith, int height, bool colorkeying, SDL_Color keyrgb);
SDL_Texture* load_image(SDL_Renderer *renderer, char *picname, bool colorkeying, SDL_Color keyrgb);
void turtle_crawling(turtlebay *turtlebay, double x0, double y0, double x, double y, Uint32 interval);
void turtle_shifting(turtlebay *turtlebay, double angle,  Uint32 interval);
void render_turtlebayframe(turtlebay *turtlebay);

void free_rabbits(rabbit *rabbit);
rabbit* add_rabbit(rabbit *oldrabbit, SDL_Texture *tex, double x, double y,
   double width, double height, double angle, double velocity);
rabbit* add_rab_fromstack(rabbit *oldrabbit, texstack *stack, int i, double x,
   double y, double velocity);
rabbit* random_add_rabbit(turtlebay *turtlebay,double x0, double x1, double vmin, double vmax);
bool collision_judge(turtlebay *turtlebay);
void turtle_rewind(turtlebay *turtlebay);

/*----------------------parsing----------------*/
/*----------------------parsing----------------*/
/*----------------------parsing----------------*/

#define MAXPROG 100
#define MAXWORD 100
#define MAXSTACK 100
#define MAXVAR 100
#define strsame(A,B) (strcmp(A, B)==0)

enum operand {add,sub,mul,divd};
typedef enum operand operand;
enum parserMODE {parsing, interpreting};
typedef enum parserMODE parserMODE;

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


struct program
{   char text[MAXPROG][MAXWORD];
    variable varlist[MAXVAR];
    stack stk;
    int loc;
    position state;
    bool terminate;
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Texture *blackboard;
    turtlebay *turtlebay;
    parserMODE parsermode;
    article *errorbar;
};
typedef struct program program;

void prog_init(program *prog);
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

void parser_operation(program *prog);
void move_draw(program *prog, double length);
void shift_angle(program *prog, double angle);
