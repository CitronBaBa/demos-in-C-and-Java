
#include "turtlebay.h"

int main(int argc, char *argv[])
{
    article article;
    turtlebay turtlebay;
    TTF_Font *font;
    SDL_Color textColor = { 125, 122, 233 };
    int i,j,ii,backspace,tokenindex;
    bool finished = FALSE;
    char programstr[MAXPROG][MAXWORD];
    program prog;

    srand(time(0));

    if(SDL_Init(SDL_INIT_VIDEO)!=0)
    {   PUTERROR("Unable to intirilize the SDL Graph");
    }
    prog.loc=0;
    prog.stk.top=0;
    prog.state.x=WWIDTH*3/4;
    prog.state.y=WHEIGHT/2;
    prog.state.angle=0;

    for(i=0;i<MAXPROG;i++)
    {   prog.text[i][0]='\0';
    }
    for(i=0;i<MAXVAR;i++)
    {   prog.varlist[i].name[0]='\0';
    }
    i=0;


    /*Initialize SDL_ttf*/
    if( TTF_Init() == -1 )
    {   PUTERROR("Cannot init ttf font file");
    }
    /* load font data */
    font = TTF_OpenFont( "OpenSans-Bold.ttf", FONTHEIGHT );

    init(&turtlebay);
    turtlebay.article = &article;
    prog.window = article.artwindow = turtlebay.window;
    prog.renderer = article.artrenderer = turtlebay.renderer;
    article_sync_char(&article, "c", textColor, font);
    article_set(&article,100,100,ARTWIDTH,ARTHEIGHT);


    while(!finished)
    {   finished = inputlisten(&article, textColor, font);
        if(finished == FALSE)
        {   backspace = article.position;
            for(i=0;i<backspace;i++)
            {    article_back(&article);
            }
            printf("%d is posi \n", article.position);
            SDL_RenderPresent(article.artrenderer);
            SDL_UpdateWindowSurface(article.artwindow);
            /*recording text*/
            for(i=0,j=0,tokenindex=0;i<backspace+1;i=i+j+1)
            {   for(j=0;article.words[i+j]!=' ' && article.words[i+j]!='\0';j++)
                {   programstr[tokenindex][j] = article.words[i+j];
                }
                programstr[tokenindex][j]='\0';
                printf("%s\n",programstr[tokenindex]);
                tokenindex++;
            }
            printf("\n");
            programstr[tokenindex][0]='\0';
            for(i=0;programstr[i][0]!='\0';i++)
            {   strcpy(prog.text[i],programstr[i]);
            }
            prog.text[i][0]='\0';
            whole(&prog);
            /* roll back*/
            prog.loc = 0;
        }
    }

    //whole(&prog);

    SDL_Quit();
    atexit(SDL_Quit);
    return 0;
}

bool inputlisten(article *article, SDL_Color textColor,TTF_Font *font)
{   SDL_Event event;
    bool finished = FALSE;
    char c;
    int i;
    while(!finished)
    {   while(SDL_PollEvent(&event))
       {    if(event.type == SDL_Quit)
            {   finished = TRUE;
            }
            if(event.type == SDL_KEYDOWN)
            {   c = key_input(article, event.key.keysym.scancode, textColor, font);
                if(c!= NONCHAR)
                {   article->words[article->position-1] = c;
                    article->words[article->position]='\0';
                }
                if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                {   return TRUE;
                }
                if(event.key.keysym.scancode == SDL_SCANCODE_RETURN)
                {   return FALSE;
                }
                SDL_Delay(50);
            }
       }
    }

    return FALSE;

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
    SDL_SetRenderDrawBlendMode(renderer0,SDL_BLENDMODE_NONE);

    turtlebay->renderer = renderer0;
    turtlebay->window = window0;
}

void article_set(article *article, int x, int y, int width, int height)
{   article->position = 0;
    article->line = 0;
    article->x0 = x;
    article->y0 = y;
    article->x = x;
    article->y = y;
    article->win_width = width;
    article->win_height = height;
}

void article_render(article *article, char *s, SDL_Color textColor, TTF_Font *font)
{   /* change line*/

    if( article->x > (article->win_width+article->x0))
    {   article->x = article->x0;
        article->y += article->dy;
        article->line++;
    }

    if( article->y > (article->win_height+article->y0))
    {   return;
    }
    /* render */
    Render_Character(article, s, article->x, article->y, textColor, font);
    /* update */
    article->x += strlen(s)*(article->dx);
    article->sizeinfo[article->position][0] = article->dx;
    article->sizeinfo[article->position][1] = article->dy;
    article->sizeinfo[article->position][2] = article->line;
    article->position++;
}

void article_back(article *article)
{   SDL_Rect rect;
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
    SDL_SetRenderDrawColor(article->artrenderer, 0, 0, 0, SDL_ALPHA_OPAQUE );
    rect.x = article->x;
    rect.y = article->y;
    rect.w = formerwidth;
    rect.h = formerheight;
    SDL_RenderFillRect(article->artrenderer, &rect);
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
     article->dx = rect.w*1.1;
     article->dy = rect.h;
     printf("the width SYNC %d\n",article->dx);
     printf("the height SYNC %d\n",article->dy);
     SDL_DestroyTexture(tex);
}

void Render_Character(article *article, char* s, int x, int y, SDL_Color textColor, TTF_Font *font)
{    SDL_Surface *pictext;
     SDL_Texture *tex;
     SDL_Rect rect;
     SDL_Renderer *renderer=article->artrenderer;
     SDL_Color backcolor={255,255,255};
     /* Create the String picture */
     pictext =  TTF_RenderText_Shaded(font,s,textColor,backcolor);
     tex = SDL_CreateTextureFromSurface(renderer,pictext);
     SDL_FreeSurface(pictext);

     /* Locate the string */
     SDL_QueryTexture(tex,NULL,NULL,&rect.w,&rect.h);
     rect.x=x;
     rect.y=y;
     article->dx = rect.w;
     //article->dy = rect.h;

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
                c[0]=NONCHAR;
                article_back(article);
                break;
          case  SDL_SCANCODE_LEFTBRACKET:
                c[0]='{';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_RIGHTBRACKET:
                c[0]='}';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_SEMICOLON:
                c[0]=':';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_EQUALS:
                c[0]='=';
                article_render(article, c, textColor, font);
                break;
          case  SDL_SCANCODE_COMMA:
                c[0]=';';
                article_render(article, c, textColor, font);
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
          default:
                c[0]=NONCHAR;
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
