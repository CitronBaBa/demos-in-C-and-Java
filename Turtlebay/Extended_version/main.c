#include "turtlebay.h"

int main(int argc, char *argv[])
{   article article,topbar,errorbar;
    turtlebay turtlebay;
    SDL_Color textColor = {155,200,110,255};
    SDL_Color topColor= {150,120,128,255};
    SDL_Color errorColor= {144,40,70,255};
    SDL_Color articlecolor = {0,0,0,0};
    SDL_Color topbarcolor = {0,0,0,0};
    SDL_Color errorbarcolor = {0,0,0,0};
    SDL_Color colorkey = {255,255,255,255};
    SDL_Event event;
    int i,j,backspace,tokenindex;
    bool finished = FALSE;
    char programstr[MAXPROG][MAXWORD];
    program prog;
    srand(time(0));

    if(argc!=1)
    {   PUTERROR("No need to include other file");
        /* argc,argv have to remain in the main function for SDL_main to replace them
           on windows platform */
        argv[0]="";
    }

    if(SDL_Init(SDL_INIT_VIDEO)!=0)
    {   PUTERROR("Unable to intirilize the SDL Graph");
    }
    if( TTF_Init() == -1 )
    {   PUTERROR("Cannot init ttf font file");
    }
    if( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1 )
    {    PUTERROR("audio initial failed");
    }

    init(&turtlebay);
    prog.renderer = turtlebay.renderer;
    prog.window = turtlebay.window;
    prog.blackboard = turtlebay.blackboard;
    prog.turtlebay = &turtlebay;
    prog_init(&prog);
    prog.errorbar = &errorbar;

    /* initial drawings*/
    if( Mix_PlayMusic(turtlebay.music, -1) == -1 )
    {    PUTERROR("MUSIC is not playing");
    }
    draw_image(turtlebay.renderer, "pictures/beach.jpg", 0, 0, 1280, 720, FALSE, colorkey);
    draw_image(turtlebay.renderer, "pictures/001.jpg", 540, 500, 420, 200, TRUE, colorkey);
    article_init(&turtlebay, &article,60,210,450,510,FONTHEIGHT,articlecolor, textColor);
    article_init(&turtlebay, &topbar,60,135,450,70,FONTHEIGHT,topbarcolor, topColor);
    article_init(&turtlebay, &errorbar,60,620,450,70,FONTHEIGHT,errorbarcolor, errorColor);

    /* this event poll here is to prevent graphic error on Mac platform, the reason is unknown*/
    SDL_PollEvent(&event);
    article_Render_sentence(&topbar, "Please enter your turtle program:          Example: { ROTATE 10 } ", topbar.x0, topbar.y0, topColor);
    render_turtlebay(&turtlebay);
    turtle_crawling(&turtlebay,turtlebay.x,turtlebay.y,WWIDTH*3/4,WHEIGHT/2,TURTLE_MOVE_DELAY/3);
    SDL_UpdateWindowSurface(turtlebay.window);

    while(!finished)
    {   turtlebay.COLISIONflag = FALSE;
        finished = inputlisten(&article, textColor);
        if(finished == FALSE)
        {   /* clear up*/
            backspace = article.position;

            /*recording text, prepare for parsing*/
            for(i=0,j=0,tokenindex=0;i<backspace+1;i=i+j+1)
            {   for(j=0;article.words[i+j]!=' ' && article.words[i+j]!='\0';j++)
                {   programstr[tokenindex][j] = article.words[i+j];
                }
                programstr[tokenindex][j]='\0';
                if(j!=0)
                {   tokenindex++;
                }
            }
            programstr[tokenindex][0]='\0';
            for(i=0;programstr[i][0]!='\0';i++)
            {   strcpy(prog.text[i],programstr[i]);
            }
            prog.text[i][0]='\0';

            /* interpreting the recorded program*/
            parser_operation(&prog);
            if(prog.terminate == FALSE)
            {   for(i=0;i<backspace;i++)
                {    article_back(&article);
                }
                render_turtlebay(&turtlebay);
                SDL_UpdateWindowSurface(article.artwindow);
            }
            prog.terminate = FALSE;
        }
    }

    article_free(&article);
    article_free(&topbar);
    article_free(&errorbar);
    free_rabbits(&(turtlebay.rabbit0));
    Mix_FreeMusic(turtlebay.music);
    Mix_FreeChunk(turtlebay.boom);
    Mix_FreeChunk(turtlebay.laugh);
    Mix_CloseAudio();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    atexit(SDL_Quit);
    return 0;
}
