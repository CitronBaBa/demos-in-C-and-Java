/*
  Created by 王朝 on 18/12/2018.
*/
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAXPROG 100
#define MAXWORD 100
#define PUTERROR(WARNING) {fprintf(stderr,"Error:'%s' occurred in %s line %d\n",\
WARNING,__FILE__,__LINE__); exit(2); }
#define strsame(A,B) (strcmp(A, B)==0)

struct program
{   char text[MAXPROG][MAXWORD];
    int loc;
};
typedef struct program program;

enum operand {add,sub,mul,divd};
typedef enum operand operand;
enum bool {FALSE, TRUE};
typedef enum bool bool;

void prog_init(program *prog);
void prog_readwords(program *prog, FILE *fp);
void whole(program *prog);
void instrctlist(program *prog);
void instruction(program *prog);
void varnum(program *prog);
void var(program *prog);
operand op(program *prog);
void set(program *prog);
void polish(program *prog);
void doloop(program *prog);
