/*
   Created by 王朝 on 18/12/2018.
*/

#define DRAW_SWITCH FALSE
#define PRECISION 0.0001
void test_case(program *prog, char *s);
void assert_position(program *prog, int line, double x, double y);
double absolute(double value);
bool double_compare(double x, double y);
void load_string(program *prog, char *s);

int main(int argc, char* argv[])
{   program prog;
    double x0,y0;
    if(argc!=1)
    {   fprintf(stderr,"no need to open other file\n");
        argv[0]="";
        exit(2);
    }
    srand(time(0));
    prog_init(&prog);
    prog.drawflag = DRAW_SWITCH;
    prog_sdlinit(&prog);

    /* black box testing*/
    x0 = prog.state.x;
    y0 = prog.state.y;
    test_case(&prog,"testcases/test_loop.txt");
    assert(prog.errorflag==FALSE);
    assert_position(&prog, __LINE__, x0+100, y0);

    test_case(&prog,"testcases/test_decimal_number.txt");
    assert(prog.errorflag==FALSE);
    assert_position(&prog, __LINE__, x0+16.511, y0+0);

    test_case(&prog,"testcases/test_trigonometric.txt");
    assert(prog.errorflag==FALSE);
    assert_position(&prog, __LINE__, x0+43.3013, y0-25);

    test_case(&prog,"testcases/test_back_to_origin.txt");
    assert(prog.errorflag==FALSE);
    assert_position(&prog, __LINE__, x0+0, y0+0);

    test_case(&prog,"testcases/test_variable.txt");
    assert(prog.errorflag==FALSE);
    assert_position(&prog, __LINE__, x0+0, y0-90);

    test_case(&prog,"testcases/test_square.txt");
    assert(prog.errorflag==FALSE);
    assert_position(&prog, __LINE__, x0-100, y0-100);

    test_case(&prog,"testcases/test_circle.txt");
    assert(prog.errorflag==FALSE);
    assert_position(&prog, __LINE__, x0+0, y0+0);

    printf("\nBlack box testing completed\n\n");

    /* white box testing*/
    prog.drawflag = FALSE;

    /*FUNCTION var()*/
    load_string(&prog,"A");
    var(&prog);
    assert(prog.errorflag==FALSE);
    load_string(&prog,"aaaa");
    var(&prog);
    assert(prog.errorflag==FALSE);

    load_string(&prog,"1");
    var(&prog);
    assert(prog.errorflag==TRUE);
    load_string(&prog,"a1");
    var(&prog);
    assert(prog.errorflag==TRUE);
    load_string(&prog,"*");
    var(&prog);
    assert(prog.errorflag==TRUE);

    /*FUNCTION setvariable() & findvariable()*/
    prog_init(&prog);
    setvariable(&prog, "A", 1.5);
    assert(strsame(prog.varlist[0].name,"A"));
    assert(double_compare(prog.varlist[0].value,1.5)==TRUE);
    setvariable(&prog, "A", 3.5);
    assert(double_compare(prog.varlist[0].value,3.5)==TRUE);
    setvariable(&prog, "B", 3.5);
    assert(strsame(prog.varlist[1].name,"B"));
    assert(double_compare(prog.varlist[1].value,3.5)==TRUE);

    assert(double_compare(findvariable(&prog,"A"),3.5)==TRUE);
    assert(double_compare(findvariable(&prog,"B"),3.5)==TRUE);
    findvariable(&prog,"C");
    assert(prog.errorflag==TRUE);

    /*FUNCTION shift_angle()*/
    prog_init(&prog);
    shift_angle(&prog, 180);
    assert(double_compare(prog.state.angle,R_PI)==TRUE);
    shift_angle(&prog, 180);
    assert(double_compare(prog.state.angle,2*R_PI)==TRUE);
    prog_init(&prog);
    shift_angle(&prog, 750);
    assert(double_compare(prog.state.angle,R_PI/6)==TRUE);

    /*FUNCTION move_draw()*/
    prog_init(&prog);
    prog.state.angle= R_PI/3;
    move_draw(&prog,100);
    assert(double_compare(prog.state.x,WWIDTH/2+50)==TRUE);
    assert(double_compare(prog.state.y,WHEIGHT/2+86.6025)==TRUE);


    /*FUNCTION varnum()*/
    load_string(&prog,"40");
    assert(double_compare(varnum(&prog),40)==TRUE);
    assert(prog.errorflag==FALSE);
    load_string(&prog,"1.5");
    assert(double_compare(varnum(&prog),1.5)==TRUE);
    assert(prog.errorflag==FALSE);
    load_string(&prog,"6.6666");
    assert(double_compare(varnum(&prog),6.6666)==TRUE);
    assert(prog.errorflag==FALSE);
    load_string(&prog,"1...5");
    varnum(&prog);
    assert(prog.errorflag==TRUE);

    load_string(&prog,"AB");
    setvariable(&prog, "AB", 1.22);
    assert(double_compare(varnum(&prog),1.22)==TRUE);
    assert(prog.errorflag==FALSE);
    load_string(&prog,"A");
    setvariable(&prog, "A", 288);
    assert(double_compare(varnum(&prog),288)==TRUE);
    assert(prog.errorflag==FALSE);
    load_string(&prog,"A");
    setvariable(&prog, "A", 288);
    assert(double_compare(varnum(&prog),233)==FALSE);
    assert(prog.errorflag==FALSE);

    /*FUNCTION op()*/
    load_string(&prog,"+");
    assert(op(&prog)==add);
    assert(prog.errorflag==FALSE);
    load_string(&prog,"-");
    assert(op(&prog)==sub);
    assert(prog.errorflag==FALSE);
    load_string(&prog,"/");
    assert(op(&prog)==divd);
    assert(prog.errorflag==FALSE);
    load_string(&prog,"1");
    op(&prog);
    assert(prog.errorflag==TRUE);
    load_string(&prog,"++");
    op(&prog);
    assert(prog.errorflag==TRUE);

    /*FUNCTION push() & pop()*/
    prog_init(&prog);
    push(&prog,1.5);
    assert(prog.errorflag==FALSE);
    assert(prog.stk.top==1);
    push(&prog,2.5);
    assert(prog.errorflag==FALSE);
    assert(prog.stk.top==2);

    assert(double_compare(pop(&prog),2.5)==TRUE);
    assert(prog.errorflag==FALSE);
    assert(double_compare(pop(&prog),1.5)==TRUE);
    assert(prog.errorflag==FALSE);
    pop(&prog);
    assert(prog.errorflag==TRUE);

    /*FUNCTION polish()*/
    load_string(&prog,"3 5 + ; ");
    polish(&prog);
    assert(prog.errorflag==FALSE);
    assert(double_compare(pop(&prog),8)==TRUE);

    load_string(&prog,"3 5 7 + * ; ");
    polish(&prog);
    assert(prog.errorflag==FALSE);
    assert(double_compare(pop(&prog),36)==TRUE);

    load_string(&prog,"3 6 7");
    polish(&prog);
    assert(prog.errorflag==TRUE);

    load_string(&prog,"3 6 7 ;");
    polish(&prog);
    assert(prog.errorflag==TRUE);

    load_string(&prog,"3 6 7 + ;");
    polish(&prog);
    assert(prog.errorflag==TRUE);

    load_string(&prog," + ;");
    polish(&prog);
    assert(prog.errorflag==TRUE);

    /*FUNCTION set()*/
    load_string(&prog,"A := 3.28 2 * ; ");
    set(&prog);
    assert(prog.errorflag==FALSE);
    assert(strsame(prog.varlist[0].name,"A"));
    assert(double_compare(prog.varlist[0].value,6.56)==TRUE);

    load_string(&prog,"B := 3.28 ; B := 1.28 ;");
    set(&prog);
    set(&prog);
    assert(prog.errorflag==FALSE);
    assert(strsame(prog.varlist[0].name,"B"));
    assert(double_compare(prog.varlist[0].value,1.28)==TRUE);

    /*FUNCTION doloop()*/
    load_string(&prog,"A FROM 1 TO 10 { SET B := 1 A + ; } ");
    doloop(&prog);
    assert(prog.errorflag==FALSE);
    assert(double_compare(prog.varlist[0].value,10)==TRUE);
    assert(double_compare(prog.varlist[1].value,11)==TRUE);
    load_string(&prog,"A FROM 3 TO 5 { SET B := 1 A + ; } ");
    doloop(&prog);
    assert(prog.errorflag==FALSE);
    assert(double_compare(prog.varlist[0].value,5)==TRUE);
    assert(double_compare(prog.varlist[1].value,6)==TRUE);

    load_string(&prog,"A FRO 3 TO 5 { SET B := 1 A + ; } ");
    doloop(&prog);
    assert(prog.errorflag==TRUE);
    load_string(&prog,"A FROM 3 5 { SET B := 1 A + ; } ");
    doloop(&prog);
    assert(prog.errorflag==TRUE);
    load_string(&prog,"A FROM 3 TO 5 SET B := 1 A + ; } ");
    doloop(&prog);
    assert(prog.errorflag==TRUE);

    /*FUNCTION instruction()*/
    load_string(&prog,"FD 40");
    instruction(&prog);
    assert(prog.errorflag==FALSE);
    load_string(&prog,"LT 40");
    instruction(&prog);
    assert(prog.errorflag==FALSE);
    load_string(&prog,"SET A := 1 ;");
    instruction(&prog);
    assert(prog.errorflag==FALSE);
    load_string(&prog,"DO A FROM 3 TO 5 { SET B := 1 A + ; } ");
    instruction(&prog);
    assert(prog.errorflag==FALSE);

    load_string(&prog,"FA");
    instruction(&prog);
    assert(prog.errorflag==TRUE);

    /*FUNCTION instrctlist()*/
    load_string(&prog,"FD 40 LT 40 }");
    instrctlist(&prog);
    assert(prog.errorflag==FALSE);

    load_string(&prog,"FD 40 LT 40 ");
    instrctlist(&prog);
    assert(prog.errorflag==TRUE);

    /*FUNCTION whole()*/
    load_string(&prog," { FD 40 LT 40 }");
    whole(&prog);
    assert(prog.errorflag==FALSE);

    load_string(&prog,"FD 40 LT 40 }");
    whole(&prog);
    assert(prog.errorflag==TRUE);

    printf("\nWhite box testing completed\n");

    if (DRAW_SWITCH)
    {   do
        {   Graph_Events(&(prog.sdlobject));
        }while(!prog.sdlobject.finished);
        SDL_Quit();
        atexit(SDL_Quit);
    }
    return 0;
}

void test_case(program *prog, char *s)
{   FILE *fp;
    if(!(fp=fopen(s,"r")))
    {   fprintf(stderr,"Can not open file %s\n",s);
        exit(2);
    }
    /* clean wordslist in the program*/
    prog_init(prog);

    /* execute test program*/
    prog_readwords(prog, fp);
    whole(prog);
}

void assert_position(program *prog, int line, double x, double y)
{   double x0,y0;
    x0 = prog->state.x;
    y0 = prog->state.y;
    if( !double_compare(x,x0) || !double_compare(y,y0))
    {   fprintf(stderr,"position-assertion fails at line %d in test.c \n", line);
        exit(2);
    }
}

double absolute(double value)
{   if(value<0)
    {   return (-1)*value;
    }
    return value;
}

bool double_compare(double x, double y)
{   if(absolute(x-y) <= PRECISION)
    {   return TRUE;
    }
    return FALSE;
}

void load_string(program *prog, char *s)
{   int i,j,tokenindex,len;
    len = strlen(s);

    /* clear wordlist */
    prog_init(prog);

    /* load tokens*/
    for(i=0,tokenindex=0; i<len && tokenindex<MAXPROG; i=i+j+1)
    {   for(j=0 ;s[i+j]!=' ' && s[i+j]!='\0' ;j++)
        {   prog->text[tokenindex][j] = s[i+j];
            if(j>=MAXWORD)
            {   fprintf(stderr,"one token exceeds the length limitation\n");
                exit(2);
            }
        }
        prog->text[tokenindex][j]='\0';
        if(j!=0)
        {   tokenindex++;
        }
    }
}
