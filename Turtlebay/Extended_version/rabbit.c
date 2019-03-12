/*
--  Created by 王朝 on 22/1/2019.
*/
#include "turtlebay.h"

rabbit* add_rabbit(rabbit *oldrabbit, SDL_Texture *tex, double x, double y,
  double width, double height, double angle, double velocity)
{    rabbit *newrabbit;
     newrabbit = oldrabbit->next;
     while (newrabbit!=NULL)
     {   oldrabbit = newrabbit;
         newrabbit = oldrabbit->next;
     }
     newrabbit = oldrabbit->next = (rabbit*)calloc(1,sizeof(rabbit));
     if(oldrabbit->next==NULL)
     {   PUTERROR("NEW rabbit not found\n");
     }
     newrabbit->before = oldrabbit;
     newrabbit->next = NULL;
     newrabbit->x = x;
     newrabbit->y = y;
     newrabbit->velocity = velocity;
     newrabbit->rabbit_tex = tex;
     newrabbit->angle = angle;
     newrabbit->width = width;
     newrabbit->height = height;
     return newrabbit;
}

rabbit* add_rab_fromstack(rabbit *oldrabbit, texstack *stack, int i, double x, double y, double velocity)
{    double width, height, angle;
     width = stack->Textinfo[i][0];
     height = stack->Textinfo[i][1];
     angle = stack->Textinfo[i][2];
     return add_rabbit(oldrabbit,stack->hill[i],x,y,width,height,angle,velocity);
}

void free_rabbits(rabbit *rabbit0)
{    rabbit *forward, *backward, *temp;
     forward = rabbit0->next;
     backward = rabbit0->before;
     temp = forward;

     while(forward!=NULL)
     {   forward = forward->next;
         free(temp);
         temp = forward;
     }

     temp = backward;
     while(backward!=NULL)
     {   backward = backward->before;
         free(temp);
         temp = backward;
     }
     rabbit0->next=NULL;
     rabbit0->before=NULL;
     /*the first rabbit is not malloced, no need to free*/
}

rabbit* random_add_rabbit(turtlebay *turtlebay,double x0, double x1, double vmin, double vmax)
{   int numtex,texindex;
    double x,coefficient,velocity,y=RABBIT_Y_POSITION;

    numtex = turtlebay->rabbits_stack.top;
    texindex = rand()%numtex;
    coefficient = (float)rand()/RAND_MAX;
    x = coefficient*(x1-x0)+x0;
    coefficient = (float)rand()/RAND_MAX;
    velocity = coefficient*(vmax-vmin)+vmin;
    turtlebay->rabbits_count++;

    return add_rab_fromstack(&(turtlebay->rabbit0),&(turtlebay->rabbits_stack),
            texindex, x, y, velocity);
}

bool collision_judge(turtlebay *turtlebay)
{   double x1,y1,x2,y2,distance,size1,size2;
    rabbit *rabbit;
    x1 = turtlebay->x;
    y1 = turtlebay->y;
    size1 = (turtlebay->width+turtlebay->height)/2*SIZE_AJUST;
    rabbit = turtlebay->rabbit0.next;

    /* boundary judge */
    if(x1>WWIDTH || x1<WWIDTH/2 || y1>WHEIGHT || y1<0)
    {   return TRUE;
    }

    while(rabbit!=NULL)
    {   size2 = (rabbit->width+rabbit->height)/2*SIZE_AJUST;
        x2 = rabbit->x;
        y2 = rabbit->y;
        distance = fabs(x1-x2)+fabs(y1-y2);
        if( distance<(size1+size2) )
        {   return TRUE;
        }
        rabbit = rabbit->next;
    }

    return FALSE;
}

void turtle_rewind(turtlebay *turtlebay)
{   int i;
    double width=turtlebay->width,height=turtlebay->height;
    if (Mix_PlayChannel(-1, turtlebay->laugh, 0)==-1)
    {   PUTERROR("laugh sound is not played");
    }
    if (Mix_PlayChannel(-1, turtlebay->boom, 0)==-1)
    {   PUTERROR("boom sound is not played");
    }

    /* blink the turtle*/
    for (i=0;i<BLINK_TIME;i++)
    {   if(i%2==0)
        {   turtlebay->width = 0;
            turtlebay->height = 0;
            render_turtlebay(turtlebay);
        }
        else
        {   turtlebay->width = width;
            turtlebay->height = height;
            render_turtlebay(turtlebay);
        }
        SDL_Delay(BLINK_INTER*FRAMETIME);
    }
    turtlebay->width = width;
    turtlebay->height = height;

    free_rabbits(&(turtlebay->rabbit0));
    turtlebay->rabbits_count = 1;
    turtlebay->x = WWIDTH*3/4;
    turtlebay->y = WHEIGHT/2;
    turtlebay->angle = INITIAL_ANGLE/180*R_PI;
    turtlebay->rotate_angle = 0;
    turtlebay->COLISIONflag = TRUE;
}
