#include "parser.h"

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

    sdlobject->surface = SDL_CreateTexture(sdlobject->renderer, SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_TARGET, WWIDTH, WHEIGHT);
    if(sdlobject->surface == NULL)
    {   PUTERROR("Initialization SDL texture failed");
    }
    SDL_SetTextureAlphaMod(sdlobject->surface,SDL_BLENDMODE_ADD);
    SDL_SetTextureBlendMode(sdlobject->surface,SDL_BLENDMODE_ADD);
    SDL_SetRenderTarget(sdlobject->renderer, sdlobject->surface);
}

void Graph_SetRenderDrawColor(SDL_Implement  *sdlobject, Uint8 r, Uint8 g, Uint8 b)
{    SDL_SetRenderDrawColor(sdlobject->renderer, r, g, b, SDL_ALPHA_OPAQUE);
}

void Graph_UpdateScreen(SDL_Implement  *sdlobject, program *prog)
{   SDL_Rect rect0;
    
    /*attach photo*/
    SDL_QueryTexture(sdlobject->pic,NULL,NULL,&rect0.w,&rect0.h);
    rect0.x=prog->state.x;
    rect0.y=prog->state.y;
    SDL_RenderCopy(sdlobject->renderer, sdlobject->pic,NULL,&rect0);
    
    SDL_SetRenderTarget(sdlobject->renderer, NULL);
    SDL_RenderCopy(sdlobject->renderer, sdlobject->surface, NULL, NULL);
    
    SDL_RenderPresent(sdlobject->renderer);
    SDL_SetRenderTarget(sdlobject->renderer, sdlobject->surface);

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
