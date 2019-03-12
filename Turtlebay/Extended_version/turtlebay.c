/*
--  Created by 王朝 on 18/12/2018.
*/

#include "turtlebay.h"

bool inputlisten(article *article, SDL_Color textColor)
{   SDL_Event event;
    bool finished = FALSE;
    TTF_Font *font = article->font;
    char c;

    while(!finished)
    {   while(SDL_PollEvent(&event))
       {    if(event.type == SDL_KEYDOWN)
            {   c = key_input(event.key.keysym.scancode);
                if(c!= NONCHAR && c != '\0')
                {   article_render(article, c, textColor, font);
                }
                if(event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
                {   article_back(article);
                }
                render_turtlebay(article->turtlebay);
                SDL_UpdateWindowSurface(article->artwindow);
                if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                {   return TRUE;
                }
                if(event.key.keysym.scancode == SDL_SCANCODE_RETURN)
                {   return FALSE;
                }
            }
       }
       render_turtlebayframe(article->turtlebay);
       article->turtlebay->COLISIONflag = FALSE;
    }

    return FALSE;
}

void init(turtlebay *turtlebay)
{   SDL_Window *window0;
    SDL_Renderer *renderer0;
    SDL_Texture *tex0,*tex1;
    SDL_Color keyrgb = {255,255,255,255};
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
    SDL_SetRenderDrawBlendMode(renderer0,SDL_BLENDMODE_NONE);

    tex0 = SDL_CreateTexture(renderer0, SDL_PIXELFORMAT_RGBA8888,
                                    SDL_TEXTUREACCESS_TARGET, WWIDTH, WHEIGHT);
    SDL_SetTextureBlendMode(tex0, SDL_BLENDMODE_NONE);
    SDL_SetTextureAlphaMod(tex0, 122);
    if(tex0 == NULL)
    {   PUTERROR("Unable to initialize SDL texture");
    }
    SDL_SetRenderTarget(renderer0, tex0);

    turtlebay->texstack.top = 0;
    turtlebay->rabbits_stack.top = 0;

    /* rabbit images loading to stack*/
    pushtex(&(turtlebay->rabbits_stack), load_image(renderer0,"pictures/rabbit03.bmp",TRUE, keyrgb),
              64.0, 76.0, -180.0/180*R_PI);
    pushtex(&(turtlebay->rabbits_stack), load_image(renderer0,"pictures/rabbit02.bmp",TRUE, keyrgb),
                        66.0, 68.0, 0);

    /*turtle image and position handling*/
    tex1 = load_image(renderer0,"pictures/002.bmp",TRUE, keyrgb);
    if(tex1==NULL)
    {   PUTERROR("turtle image loading failed");
    }
    turtlebay->cuteturtle = tex1;
    turtlebay->width = 73;
    turtlebay->height = 77;
    turtlebay->x = WWIDTH*3/4;
    turtlebay->y = WHEIGHT;
    turtlebay->angle = INITIAL_ANGLE/180*R_PI;
    turtlebay->rotate_angle = 0;

    turtlebay->blackboard = tex0;
    turtlebay->renderer = renderer0;
    turtlebay->window = window0;
    turtlebay->time0 = turtlebay->time1 = clock();
    turtlebay->hardness = HARDNESS_INITAL;

    turtlebay->rabbit0.rabbit_tex = turtlebay->rabbits_stack.hill[0];
    turtlebay->rabbit0.next=NULL;
    turtlebay->rabbit0.before=NULL;
    turtlebay->rabbit0.x = WWIDTH*1/2;
    turtlebay->rabbit0.y = -WHEIGHT;
    turtlebay->rabbit0.velocity = 0;
    turtlebay->rabbits_count = 1;
    turtlebay->rabbit0.angle = turtlebay->rabbits_stack.Textinfo[0][2];
    /* rabbit0 doesn't show*/
    turtlebay->rabbit0.width = 1;
    turtlebay->rabbit0.height = 1;
    turtlebay->COLISIONflag = FALSE;

    turtlebay->music = NULL;
    turtlebay->boom = NULL;
    turtlebay->laugh = NULL;

    turtlebay->music = Mix_LoadMUS("music/summer.wav");
    if(turtlebay->music == NULL)
    {   PUTERROR("audio file loading failed");
    }
    turtlebay->boom = Mix_LoadWAV("music/punch.wav");
    if(turtlebay->boom == NULL)
    {   PUTERROR("audio file loading failed");
    }
    turtlebay->laugh = Mix_LoadWAV("music/laugh.wav");
    if(turtlebay->laugh == NULL)
    {   PUTERROR("audio file loading failed");
    }
}

void pushtex(texstack *texstack, SDL_Texture *tex, double width, double height, double angle)
{   assert(texstack->top<MAXTEXSTACK);
    texstack->hill[texstack->top] = tex;
    texstack->Textinfo[texstack->top][0] = width;
    texstack->Textinfo[texstack->top][1] = height;
    texstack->Textinfo[texstack->top][2] = angle;
    texstack->top++;
}

void article_init(turtlebay *turtlebay, article *article, int x, int y, int width, int height, int fontheight,
  SDL_Color articlecolor, SDL_Color textColor)
{   int i;
    int num;
    SDL_Renderer *renderer0;
    SDL_Texture *tex0,*tex1;
    SDL_Window *window0;
    num = width*height/fontheight/fontheight*3;

    article->artwindow = window0 = turtlebay->window;
    article->artrenderer = renderer0 = turtlebay->renderer;
    article->artboard = tex0 = turtlebay->blackboard;

    tex1 = SDL_CreateTexture(renderer0, SDL_PIXELFORMAT_RGBA8888,
                                    SDL_TEXTUREACCESS_TARGET, WWIDTH, WHEIGHT);
    SDL_SetTextureBlendMode(tex1, SDL_BLENDMODE_ADD);
    if(tex1 == NULL)
    {   PUTERROR("Unable to initialize SDL texture");
    }

    article->textboard = tex1;
    article->turtlebay = turtlebay;
    pushtex(&(turtlebay->texstack),tex1,(double)width,(double)height,0);
    article->length = num;
    article->position = 0;
    article->line = 0;
    article->x0 = x;
    article->y0 = y;
    article->x = x;
    article->y = y;
    article->win_width = width;
    article->win_height = height;
    article->articlecolor = articlecolor;
    article->textColor = textColor;
    article->font = TTF_OpenFont(FONTNAME, fontheight);;
    if(article->font == NULL)
    {   PUTERROR("Unable to load TTF font file");
    }
    article->words = (char*)calloc(num,sizeof(char));
    article->words[0] = '\0';
    article->sizeinfo = (int**)calloc(num,sizeof(int *));
    for(i=0;i<num;i++)
    {   *(article->sizeinfo+i) = (int*) calloc(3,sizeof(int));
    }
    article_sync_char(article, "c", textColor, article->font);

    draw_rect(article->artrenderer, article->x0, article->y0,
      article->win_width+FONTHEIGHT, article->win_height, article->articlecolor);
}

void article_free(article *article)
{   int i;
    free(article->words);
    for(i=0; i<article->length; i++)
    {    free(*(article->sizeinfo+i));
    }
    free(article->sizeinfo);
}

void article_render(article *article, char c, SDL_Color textColor, TTF_Font *font)
{   char s[2];
    s[1]='\0';
    s[0]=c;

    /* change line*/
    if( article->x > (article->win_width+article->x0))
    {   article->x = article->x0;
        article->y += article->dy;
        article->line++;
    }

    if( article->y > (article->win_height+article->y0))
    {   return;
    }
    /* render */
    article_Render_Character(article, s, article->x, article->y, textColor, font);
    /* update */
    article->x += article->dx;
    article->sizeinfo[article->position][0] = article->dx;
    article->sizeinfo[article->position][1] = article->dy;
    article->sizeinfo[article->position][2] = article->line;

    article->words[article->position] = s[0];
    article->words[article->position+1] = '\0';
    article->position++;
}

void article_Render_sentence(article *article, char* s, int x, int y, SDL_Color textColor)
{    int i;
     article->x = x;
     article->y = y;
     for(i=0;s[i]!='\0';i++)
     {   article_render(article,s[i],textColor, article->font);
     }
}

void article_back(article *article)
{   SDL_Rect rect;
    SDL_Color color={0,0,0,255};
    int i;
    int formerwidth,formerheight,formerline;
    int linewidth=0;

    if(article->position==0)
    {   return;
    }

    formerwidth = article->sizeinfo[article->position-1][0];
    formerheight = article->sizeinfo[article->position-1][1];
    formerline = article->sizeinfo[article->position-1][2];

    /* change line backwards*/
    if( (article->x - formerwidth) < article->x0)
    {   for(i=0;i<article->position;i++)
        {    if(article->sizeinfo[i][2]==formerline)
             {   linewidth += article->sizeinfo[i][0];
             }
        }
        article->x = article->x0 + linewidth ;
        article->y -= article->dy;
        article->line--;
    }

    /* update */
    article->x -= formerwidth;
    article->position--;

    /* render a black rectangle*/
    SDL_SetRenderTarget(article->artrenderer,article->textboard);
    rect.x = article->x;
    rect.y = article->y;
    rect.w = formerwidth;
    rect.h = formerheight;
    SDL_SetRenderDrawColor(article->artrenderer, color.r, color.g, color.b, color.a );
    SDL_RenderFillRect(article->artrenderer, &rect);
    SDL_SetRenderTarget(article->artrenderer,article->artboard);
}

void article_sync_char(article *article,char *s,SDL_Color textColor, TTF_Font *font)
{    SDL_Surface *pictext;
     SDL_Texture *tex;
     SDL_Rect rect;

     /* Create the char picture */
     pictext =  TTF_RenderText_Solid(font,s,textColor);
     tex = SDL_CreateTextureFromSurface(article->artrenderer,pictext);
     SDL_FreeSurface(pictext);

     /* get the char size */
     SDL_QueryTexture(tex,NULL,NULL,&rect.w,&rect.h);
     article->dx = rect.w;
     article->dy = rect.h;
     SDL_DestroyTexture(tex);
}

void article_Render_Character(article *article, char* s, int x, int y, SDL_Color textColor, TTF_Font *font)
{    SDL_Surface *pictext;
     SDL_Texture *tex;
     SDL_Rect rect;
     SDL_Renderer *renderer=article->artrenderer;
     SDL_Color backcolor={0,0,0,0};

     /* Create the String picture */
     pictext =  TTF_RenderText_Shaded(font,s,textColor,backcolor);
     tex = SDL_CreateTextureFromSurface(renderer,pictext);
     SDL_FreeSurface(pictext);

     /* Locate the string */
     SDL_QueryTexture(tex,NULL,NULL,&rect.w,&rect.h);
     rect.x=x;
     rect.y=y;
     article->dx = rect.w;

     /* Render the string */
     SDL_SetRenderTarget(renderer, article->textboard);
     SDL_RenderCopy(renderer,tex,NULL,&rect);
     SDL_SetRenderTarget(renderer, article->artboard);
     SDL_DestroyTexture(tex);
}

void print_error(article *errorbar, char *s)
{    article_Render_sentence(errorbar,s,errorbar->x,errorbar->y,errorbar->textColor);
     render_turtlebay(errorbar->turtlebay);
     SDL_UpdateWindowSurface(errorbar->artwindow);
}

void clear_error(article *errorbar)
{    while(errorbar->position!=0)
     {   SDL_Delay(FRAMETIME/2);
         article_back(errorbar);
         render_turtlebay(errorbar->turtlebay);
         SDL_UpdateWindowSurface(errorbar->artwindow);
    }
}

char key_input(SDL_Scancode key)
{    char c[2];
     c[1]='\0';
     switch(key)
     {    case  SDL_SCANCODE_SPACE:
                c[0]=' ';
                break;

          case  SDL_SCANCODE_LEFTBRACKET:
                if ( SDL_GetModState() & (KMOD_LSHIFT | KMOD_RSHIFT))
                {   c[0] = '{';
                }
                else
                {   c[0]='[';
                }
                break;

          case  SDL_SCANCODE_RIGHTBRACKET:
                if ( SDL_GetModState() & (KMOD_LSHIFT | KMOD_RSHIFT))
                {   c[0] = '}';
                }
                else
                {   c[0]=']';
                }
                break;

          case  SDL_SCANCODE_SEMICOLON:
                if ( SDL_GetModState() & (KMOD_LSHIFT | KMOD_RSHIFT))
                {   c[0] = ':';
                }
                else
                {   c[0]=';';
                }
                break;
          case  SDL_SCANCODE_EQUALS:
                if ( SDL_GetModState() & (KMOD_LSHIFT | KMOD_RSHIFT))
                {   c[0] = '+';
                }
                else
                {   c[0]='=';
                }
                break;
          case  SDL_SCANCODE_PERIOD:
                if ( SDL_GetModState() & (KMOD_LSHIFT | KMOD_RSHIFT))
                {   c[0] = '>';
                }
                else
                {   c[0]='.';
                }
                break;
          case  SDL_SCANCODE_SLASH:
                if ( SDL_GetModState() & (KMOD_LSHIFT | KMOD_RSHIFT))
                {   c[0] = '?';
                }
                else
                {   c[0]='/';
                }
                break;
          case  SDL_SCANCODE_MINUS:
                if ( SDL_GetModState() & (KMOD_LSHIFT | KMOD_RSHIFT))
                {   c[0] = '_';
                }
                else
                {   c[0]='-';
                }
                break;

          case  SDL_SCANCODE_1:
                c[0]='1';
                break;
          case  SDL_SCANCODE_2:
                c[0]='2';
                break;
          case  SDL_SCANCODE_3:
                c[0]='3';
                break;
          case  SDL_SCANCODE_4:
                c[0]='4';
                break;
          case  SDL_SCANCODE_5:
                c[0]='5';
                break;
          case  SDL_SCANCODE_6:
                c[0]='6';
                break;
          case  SDL_SCANCODE_7:
                c[0]='7';
                break;
          case  SDL_SCANCODE_8:
                if ( SDL_GetModState() & (KMOD_LSHIFT | KMOD_RSHIFT))
                {   c[0] = '*';
                }
                else
                {   c[0]='8';
                }
               break;
          case  SDL_SCANCODE_9:
               c[0]='9';
               break;
          case  SDL_SCANCODE_0:
               c[0]='0';
               break;

          case  SDL_SCANCODE_A:
                c[0]='A';
                break;
          case  SDL_SCANCODE_B:
                c[0]='B';
                break;
          case  SDL_SCANCODE_C:
                c[0]='C';
                break;
          case  SDL_SCANCODE_D:
                c[0]='D';
                break;
          case  SDL_SCANCODE_E:
                c[0]='E';
                break;
          case  SDL_SCANCODE_F:
                c[0]='F';
                break;
          case  SDL_SCANCODE_G:
                c[0]='G';
                break;
          case  SDL_SCANCODE_H:
                c[0]='H';
                break;
          case  SDL_SCANCODE_I:
                c[0]='I';
                break;
          case  SDL_SCANCODE_J:
                c[0]='J';
                break;
          case  SDL_SCANCODE_K:
                c[0]='K';
                break;
          case  SDL_SCANCODE_L:
                c[0]='L';
                break;
          case  SDL_SCANCODE_M:
                c[0]='M';
                break;
          case  SDL_SCANCODE_N:
                c[0]='N';
                break;
          case  SDL_SCANCODE_O:
                c[0]='O';
                break;
          case  SDL_SCANCODE_P:
                c[0]='P';
                break;
          case  SDL_SCANCODE_Q:
                c[0]='Q';
                break;
          case  SDL_SCANCODE_R:
               c[0]='R';
               break;
          case  SDL_SCANCODE_S:
               c[0]='S';
               break;
          case  SDL_SCANCODE_T:
               c[0]='T';
               break;
          case  SDL_SCANCODE_U:
                c[0]='U';
                break;
          case  SDL_SCANCODE_V:
                c[0]='V';
                break;
          case  SDL_SCANCODE_W:
                c[0]='W';
                break;
          case  SDL_SCANCODE_X:
                c[0]='X';
                break;
          case  SDL_SCANCODE_Y:
                c[0]='Y';
                break;
          case  SDL_SCANCODE_Z:
                c[0]='Z';
                break;
          default:
                c[0]= '\0';
                break;
     }
     return c[0];
}
