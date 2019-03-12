/*
Created by 王朝 on 18/12/2018.
*/

int main(int argc, char* argv[])
{   program prog;
    FILE *fp;
    SDL_Implement* sdlobject=&(prog.sdlobject);

    if(argc!=2)
    {   fprintf(stderr,"Please enter one file name\n");
        exit(2);
    }
    if(!(fp=fopen(argv[1],"r")))
    {   fprintf(stderr,"Can not open file %s\n",argv[1]);
        exit(2);
    }
    srand(time(0));
    prog_init(&prog);
    prog.drawflag = TRUE;
    prog_sdlinit(&prog);
    prog_readwords(&prog, fp);

    whole(&prog);

    do
    {   Graph_Events(sdlobject);
    }while(!sdlobject->finished);

    SDL_Quit();
    atexit(SDL_Quit);
    return 0;
}
