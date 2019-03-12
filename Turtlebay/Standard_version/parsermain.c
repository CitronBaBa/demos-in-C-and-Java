/*
 Created by 王朝 on 18/12/2018.

*/

int main(int argc, char* argv[])
{   program prog;
    FILE *fp;

    if(argc!=2)
    {   fprintf(stderr,"Please enter one file name\n");
        exit(2);
    }
    if(!(fp=fopen(argv[1],"r")))
    {   fprintf(stderr,"Can not open file %s\n",argv[1]);
        exit(2);
    }

    prog_init(&prog);
    prog_readwords(&prog, fp);

    /* Parsing */
    whole(&prog);

    return 0;
}
