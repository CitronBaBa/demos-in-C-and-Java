
#include "turtlebay.h"

int main(int argc, char *argv[])
{   article article;
    turtlebay turtlebay;
    TTF_Font *font;
    SDL_Color textColor = { 125, 122, 233 };
    int i;
    bool finished = FALSE;
    SDL_Surface *pic;
    SDL_Texture *tex1;
    SDL_Event event;

    if(argc!=2)
    {   fprintf(stderr,"Please enter one picture name \n");
    }

    if(SDL_Init(SDL_INIT_VIDEO)!=0)
    {   PUTERROR("Unable to intirilize the SDL Graph");
    }

    /*Initialize SDL_ttf*/
    if( TTF_Init() == -1 )
    {   PUTERROR("Cannot init ttf font file");
    }

    /* load font data */
    font = TTF_OpenFont( "OpenSans-Bold.ttf", FONTHEIGHT );

    init(&turtlebay);
    turtlebay.article = &article;
    article.artwindow = turtlebay.window;
    article.artrenderer = turtlebay.renderer;
    article_sync_char(&article, textColor, font);
    article_set(&article,100,100,500,300);

    while(!finished)
    {   while(SDL_PollEvent(&event))
       {    if(event.type == SDL_Quit)
            {   finished = TRUE;
            }
            if(event.type == SDL_KEYDOWN)
            {   key_input(&article, event.key.keysym.scancode, textColor, font);
                if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                {   finished = TRUE;
                }
                SDL_Delay(50);
            }
       }
    }
    free(article.words);
    SDL_Quit();
    atexit(SDL_Quit);
    return 0;
}

void init(turtlebay *turtlebay)
{   SDL_Window *window0;
    SDL_Renderer *renderer0;
    window0 = SDL_CreateWindow("SDL Window",
                  SDL_WINDOWPOS_UNDEFINED,
                  SDL_WINDOWPOS_UNDEFINED,
                  WWIDTH, WHEIGHT,
                  SDL_WINDOW_SHOWN);
    if(window0 == NULL)
    {   PUTERROR("Article Window creation failed");
    }

    renderer0 = SDL_CreateRenderer(window0, -1,
                   SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    if(renderer0 == NULL)
    {   PUTERROR("Article Renderer creation failed");
    }
    SDL_SetRenderDrawBlendMode(renderer0,SDL_BLENDMODE_BLEND);

    turtlebay->renderer = renderer0;
    turtlebay->window = window0;
}

void article_set(article *article, int x, int y, int width, int height)
{   article->position = 0;
    article->x0 = x;
    article->y0 = y;
    article->x = x;
    article->y = y;
    article->win_width = width;
    article->win_height = height;
}

void article_render(article *article, char *s, SDL_Color textColor, TTF_Font *font)
{   /* change line*/
    if( (article->x+(article->dx)*strlen(s)) > (article->win_width+article->x0))
    {   article->x = article->x0;
        article->y += article->dy;
    }
    if( article->y > (article->win_height+article->y0))
    {   return;
    }
    /* render */
    Render_Character(article, article->artrenderer, s, article->x, article->y, textColor, font);
    /* update */
    article->x += strlen(s)*(article->dx);
}

void article_back(article *article, int len)
{   SDL_Rect rect;
    /* change line backwards*/
    if( (article->x - (article->dx)*len) < article->x0)
    {   article->x = article->x0 + article->win_width - article->win_width%article->dx ;
        article->y -= article->dy;
    }
    if(article->y < article->y0)
    {   return;
    }
    /* update */
    article->x -= len*(article->dx);

    /* render a black rectangle*/
    SDL_SetRenderDrawColor(article->artrenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    rect.x = article->x;
    rect.y = article->y;
    rect.w = article->dx*len;
    rect.h = article->dy;
    SDL_RenderFillRect(article->artrenderer, &rect);
}

void article_sync_char(article *article,SDL_Color textColor, TTF_Font *font)
{    SDL_Surface *pictext;
     SDL_Texture *tex;
     SDL_Rect rect;

     /* Create the char picture */
     pictext =  TTF_RenderText_Solid(font,"a",textColor);
     tex = SDL_CreateTextureFromSurface(article->artrenderer,pictext);
     SDL_FreeSurface(pictext);

     /* get the char size */
     SDL_QueryTexture(tex,NULL,NULL,&rect.w,&rect.h);
     article->dx = rect.w*1.3;
     article->dy = rect.h*1.3;
     printf("the width SYNC %d\n",article->dx);
     printf("the height SYNC %d\n",article->dy);
     SDL_DestroyTexture(tex);
}

void Render_Character(article *article, char* s, int x, int y, SDL_Color textColor, TTF_Font *font)
{    SDL_Surface *pictext;
     SDL_Texture *tex;
     SDL_Rect rect;
     SDL_Renderer renderer=article->artrenderer;

     /* Create the String picture */
     pictext =  TTF_RenderText_Solid(font,s,textColor);
     tex = SDL_CreateTextureFromSurface(renderer,pictext);
     SDL_FreeSurface(pictext);

     /* Locate the string */
     SDL_QueryTexture(tex,NULL,NULL,&rect.w,&rect.h);
     rect.x=x;
     rect.y=y;

     /* Render the string */
     SDL_RenderCopy(renderer,tex,NULL,&rect);
     SDL_DestroyTexture(tex);
}

char key_input(article *article, SDL_Scancode key, SDL_Color textColor, TTF_Font *font)
{    char c[2];
     c[1]='\0';
     switch(key)
     {    case  SDL_SCANCODE_SPACE:
                c[0]=' ';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_BACKSPACE:
          /*nasty*/
                c[0]=';';
                article_back(article,1);
                break;

          case  SDL_SCANCODE_1:
                c[0]='1';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_2:
                c[0]='2';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_3:
                c[0]='3';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_4:
                c[0]='4';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_5:
                c[0]='5';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_6:
                c[0]='6';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_7:
                c[0]='7';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_8:
               c[0]='8';
               article_render(article, c, textColor, font);
               break;
          case  SDL_SCANCODE_9:
               c[0]='9';
               article_render(article, c, textColor, font);
               break;
          case  SDL_SCANCODE_0:
               c[0]='0';
               article_render(article, c, textColor, font);
               break;

          case  SDL_SCANCODE_A:
                c[0]='A';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_B:
                c[0]='B';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_C:
                c[0]='C';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_D:
                c[0]='D';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_E:
                c[0]='E';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_F:
                c[0]='F';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_G:
                c[0]='G';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_H:
                c[0]='H';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_I:
                c[0]='I';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_J:
                c[0]='J';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_K:
                c[0]='K';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_L:
                c[0]='L';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_M:
                c[0]='M';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_N:
                c[0]='N';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_O:
                c[0]='O';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_P:
                c[0]='P';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_Q:
                c[0]='Q';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_R:
               c[0]='R';
               article_render(article, c, textColor, font);
               break;
          case  SDL_SCANCODE_S:
               c[0]='S';
               article_render(article, c, textColor, font);
               break;
          case  SDL_SCANCODE_T:
               c[0]='T';
               article_render(article, c, textColor, font);
               break;
          case  SDL_SCANCODE_U:
                c[0]='U';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_V:
                c[0]='V';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_W:
                c[0]='W';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_X:
                c[0]='X';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_Y:
                c[0]='Y';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_Z:
                c[0]='Z';
                article_render(article, c, textColor, font);
                break;

     }
     SDL_RenderPresent(article->artrenderer);
     SDL_UpdateWindowSurface(article->artwindow);
     return c[0];
}


/*
    for(i=0;i<33;i++)
    {    article_render(&article, "F", textColor, font);
         article_render(&article, "2", textColor, font);
         article_render(&article, "S", textColor, font);
         article_render(&article, "T", textColor, font);
         SDL_RenderPresent(article.artrenderer);
         SDL_UpdateWindowSurface(article.artwindow);
         SDL_Delay(50);
    }
    for(i=0;i<77;i++)
    {    article_back(turtlebay.article,1);
         SDL_RenderPresent(article.artrenderer);
         SDL_UpdateWindowSurface(article.artwindow);
         SDL_Delay(50);
    }
*/
/*
    window0 = SDL_CreateWindow("SDL Window",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          WWIDTH, WHEIGHT,
                          SDL_WINDOW_SHOWN);
    if(window0 == NULL)
    {   PUTERROR("Window creation failed");
    }
    renderer0 = SDL_CreateRenderer(window0, -1,
                   SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    if(renderer0 == NULL)
    {   PUTERROR("SDL Renderer creation failed");
    }
    SDL_SetRenderDrawBlendMode(renderer0,SDL_BLENDMODE_BLEND);
    tex0 = SDL_CreateTexture(renderer0, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WWIDTH, WHEIGHT);



    pic = SDL_LoadBMP(argv[1]);
    if( pic == NULL)
    {   PUTERROR("Picture file can not be open\n");
    }


    tex1 = SDL_CreateTextureFromSurface(renderer0,pic);

    SDL_FreeSurface(pic);

    SDL_SetTextureBlendMode(tex1,SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(tex1, 255);


    SDL_QueryTexture(tex1,NULL,NULL,&rect0.w,&rect0.h);
    rect0.x=333;
    rect0.y=100;


    SDL_SetRenderDrawColor(renderer0, 222, 222, 222, SDL_ALPHA_OPAQUE);

    SDL_RenderCopy(renderer0,tex1,NULL,&rect0);

    SDL_RenderDrawLine(renderer0, 22,222, 333, 333);
    SDL_RenderDrawLine(renderer0, 0, 555, 333, 333);
    SDL_RenderDrawLine(renderer0, 0, 666, 333, 333);
    SDL_RenderDrawLine(renderer0, 0, 134, 333, 333);
    SDL_RenderDrawLine(renderer0, 0, 22, 633, 633);

    SDL_RenderPresent(renderer0);
    SDL_UpdateWindowSurface(window0);  */
