#define PUTERROR(WARNING) {fprintf(stderr,"Error:'%s' occurred in %s line %d\n",WARNING,__FILE__,__LINE__); exit(2); }
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <time.h>

#define WWIDTH 1000
#define WHEIGHT 800
#define SDL_8BITCOLOUR 256
#define R_PI 3.14159265358979323846

struct SDL_Implement
{  SDL_bool finished;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *background;
    SDL_Texture *pic;
    SDL_Surface *windowsurf;
};
typedef struct SDL_Implement SDL_Implement;
enum bool {TRUE,FALSE};
typedef enum bool bool;

void Graph_init(SDL_Implement* sdlobject);
void Graph_SetRenderDrawColor(SDL_Implement  *sdlobject, Uint8 r, Uint8 g, Uint8 b);
void Graph_UpdateScreen(SDL_Implement  *sdlobject);
void Graph_Events(SDL_Implement  *sdlobject);

int main(int argc, char *argv[])
{
    SDL_Implement sdlobject;
    SDL_Surface *pic;

    if(argc!=2)
    {   fprintf(stderr,"Please enter one picture name \n");
    }

    Graph_init(&sdlobject);

    pic = SDL_LoadBMP(argv[1]);
    if( pic == NULL)
    {   PUTERROR("Picture file can not be open\n");
    }
    Graph_SetRenderDrawColor(&sdlobject, 122, 22,222);

    sdlobject.pic = SDL_CreateTextureFromSurface(sdlobject.renderer,pic);
    SDL_FreeSurface(pic);

    SDL_SetTextureBlendMode(sdlobject.pic,SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(sdlobject.pic, 122);


    SDL_RenderDrawLine(sdlobject.renderer,0,0,800,800);
    Graph_UpdateScreen(&sdlobject);
    SDL_Delay(4000);


    SDL_Quit();
    atexit(SDL_Quit);
    return 0;
}


void Graph_init(SDL_Implement* sdlobject)
{   if(SDL_Init(SDL_INIT_VIDEO)!=0)
    {   PUTERROR("Unable to intirilize the SDL Graph");
    }
    sdlobject->finished=0;
    sdlobject->window = SDL_CreateWindow("SDL Window",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          WWIDTH, WHEIGHT,
                          SDL_WINDOW_SHOWN);

    if(sdlobject->window == NULL)
    {   PUTERROR("Window creation failed");
    }

    sdlobject->renderer = SDL_CreateRenderer(sdlobject->window, -1,
                   SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    if(sdlobject->renderer == NULL)
    {   PUTERROR("SDL Renderer creation failed");
    }
    SDL_SetRenderDrawBlendMode(sdlobject->renderer,SDL_BLENDMODE_BLEND);

    sdlobject->background = SDL_CreateTexture(sdlobject->renderer, SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_TARGET, WWIDTH, WHEIGHT);

    /* get window surface*/
    sdlobject->windowsurf = SDL_GetWindowSurface(sdlobject->window);

    sdlobject->background = SDL_CreateTexture(sdlobject->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WWIDTH, WHEIGHT);
    if(sdlobject->background == NULL)
    {   PUTERROR("Initialization SDL texture failed");
    }
    SDL_SetRenderTarget(sdlobject->renderer, sdlobject->background);
}

void Graph_SetRenderDrawColor(SDL_Implement  *sdlobject, Uint8 r, Uint8 g, Uint8 b)
{    SDL_SetRenderDrawColor(sdlobject->renderer, r, g, b, SDL_ALPHA_OPAQUE);
}

void Graph_UpdateScreen(SDL_Implement  *sdlobject)
{
    SDL_SetRenderTarget(sdlobject->renderer, NULL);
    SDL_RenderCopy(sdlobject->renderer, sdlobject->background, NULL, NULL);
    SDL_RenderPresent(sdlobject->renderer);
    SDL_SetRenderTarget(sdlobject->renderer, sdlobject->background);
}

void Graph_Events(SDL_Implement  *sdlobject)
{
   SDL_Event event;
   while(SDL_PollEvent(&event))
   {
       switch (event.type){
          case SDL_QUIT:
          case SDL_MOUSEBUTTONDOWN:
          case SDL_KEYDOWN:
             sdlobject->finished = 1;
       }
    }
}
