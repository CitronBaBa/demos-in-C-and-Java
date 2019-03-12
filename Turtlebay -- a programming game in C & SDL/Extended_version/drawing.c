/*
--  Created by 王朝 on 18/12/2018.
*/
#include "turtlebay.h"

void draw_rect(SDL_Renderer *renderer, int x0, int y0, int width, int height, SDL_Color color)
{    SDL_Rect rect;
     SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
     SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
     rect.x = x0;
     rect.y = y0;
     rect.w = width;
     rect.h = height;
     SDL_RenderFillRect(renderer, &rect);
     SDL_RenderPresent(renderer);
}

void render(SDL_Renderer *renderer, SDL_Texture *carpet)
{    SDL_SetTextureBlendMode(carpet, SDL_BLENDMODE_NONE);
     SDL_SetRenderTarget(renderer, NULL);
     SDL_RenderCopy(renderer, carpet, NULL, NULL);
     SDL_RenderPresent(renderer);
     SDL_SetRenderTarget(renderer, carpet);
}

void render_turtlebayframe(turtlebay *turtlebay)
{    bool run = FALSE;
     rabbit *rabbit, *temp;

     /* calculation here avoids problematic conversion between clock_t and double */
     if( (clock()- turtlebay->time1)>FRAMETIME*CLOCKS_PER_SEC/1000 )
     {   turtlebay->time1 = clock();
         run = TRUE;
         /*  rotate */
         turtlebay->angle += turtlebay->rotate_angle;
         /*  keep angle within 2 pi*/
         while(fabs(turtlebay->angle) > 2*R_PI)
         {   if(turtlebay->angle>0)
             {   turtlebay->angle -= 2*R_PI;
             }
             if(turtlebay->angle<=0)
             {   turtlebay->angle += 2*R_PI;
             }
         }
         render_turtlebay(turtlebay);
         SDL_UpdateWindowSurface(turtlebay->window);
     }
     else
     {   return;
     }

     /* add more rabbits */
     rabbit = &(turtlebay->rabbit0);
     if((clock()- turtlebay->time0) > RABBIT_MULTI_TIME*FRAMETIME*CLOCKS_PER_SEC/1000 )
     {   if(turtlebay->rabbits_count < MAXRABBITS)
         {   random_add_rabbit(turtlebay,WWIDTH/2,WWIDTH,RABBIT_SPEED_MIN,RABBIT_SPEED_MAX);
         }
         turtlebay->time0 = clock();
     }
     /* run & free rabbits*/
     while(rabbit!=NULL)
     {   if(run)
         {   rabbit->y += rabbit->velocity*FRAMETIME*turtlebay->hardness;
         }
         if(rabbit->y+rabbit->height/2 > WHEIGHT)
         {   if(rabbit->before!=NULL)
             {   rabbit->before->next = rabbit->next;
             }
             if(rabbit->next!=NULL)
             {   rabbit->next->before = rabbit->before;
             }
             temp = rabbit->next;
             free(rabbit);
             turtlebay->rabbits_count--;
             rabbit = temp;
         }
         else
         {   rabbit = rabbit->next;
         }
     }
     if(collision_judge(turtlebay))
     {   turtle_rewind(turtlebay);
     }
}

void render_turtlebay(turtlebay *turtlebay)
{    int i;
     SDL_Rect rect,rect2;
     SDL_Texture *tex0,*tex1;
     SDL_Renderer *renderer = turtlebay->renderer;
     rabbit *rabbit;

     tex0 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                  SDL_TEXTUREACCESS_TARGET, WWIDTH, WHEIGHT);
     if(tex0 == NULL)
     {   PUTERROR("Unable to initialize SDL texture");
     }
     SDL_SetTextureBlendMode(tex0, SDL_BLENDMODE_NONE);
     SDL_SetRenderTarget(renderer, tex0);

     /* render background */
     SDL_RenderCopy(renderer, turtlebay->blackboard, NULL, NULL);

     /* render articles */
     for(i=0; i<turtlebay->texstack.top;i++)
     {   tex1=turtlebay->texstack.hill[i];
         SDL_RenderCopy(renderer, tex1, NULL, NULL);
     }

     /* render rabbits*/
     rabbit = &(turtlebay->rabbit0);
     while(rabbit!=NULL)
     {   if (fabs(rabbit->width)<PRECISION && fabs(rabbit->height)<PRECISION )
         {   rect2.w = rabbit->width;
             rect2.h = rabbit->height;
         }
         else
         {   SDL_QueryTexture(rabbit->rabbit_tex,NULL,NULL,&rect2.w,&rect2.h);
         }
         rect2.x = ((int) rabbit->x) - rect2.w/2;
         rect2.y = ((int) rabbit->y) - rect2.h/2;
         SDL_RenderCopyEx(renderer, rabbit->rabbit_tex, NULL, &rect2,
             rabbit->angle/R_PI*180.0, NULL, SDL_FLIP_NONE);
         rabbit = rabbit->next;
     }

     /* render turtle*/
     rect.w = turtlebay->width;
     rect.h = turtlebay->height;
     rect.x = (int)turtlebay->x - rect.w/2;
     rect.y = (int)turtlebay->y - rect.h/2;
     SDL_RenderCopyEx(renderer,turtlebay->cuteturtle,NULL,
       &rect,  (turtlebay->angle)/R_PI*180.0-INITIAL_ANGLE, NULL, SDL_FLIP_NONE);

     render(renderer,tex0);
     SDL_SetRenderTarget(renderer, turtlebay->blackboard);
     SDL_DestroyTexture(tex0);
}

void draw_image(SDL_Renderer *renderer, char *picname,int x, int y,
 int width, int height, bool colorkeying, SDL_Color keyrgb)
{    SDL_Texture *tex;
     SDL_Rect rect;
     tex=load_image(renderer, picname, colorkeying, keyrgb);
     if(tex==NULL)
     {   PUTERROR("turtle image loading failed");
     }
     rect.x = x;
     rect.y = y;
     SDL_QueryTexture(tex,NULL,NULL,&rect.w,&rect.h);
     if (width != 0)
     {   rect.w = width;
     }
     if (height != 0)
     {   rect.h = height;
     }
     SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
     SDL_RenderCopy(renderer, tex, NULL, &rect);
     SDL_RenderPresent(renderer);
     SDL_DestroyTexture(tex);
}

SDL_Texture* load_image(SDL_Renderer *renderer, char *picname, bool colorkeying, SDL_Color keyrgb)
{    SDL_Surface *pic;
     SDL_Texture *tex;
     pic = IMG_Load(picname);
     if(pic == NULL)
     {   PUTERROR("Cannot open picture file");
     }
     if (colorkeying == TRUE)
     {   SDL_SetColorKey(pic, SDL_TRUE, SDL_MapRGB(pic->format, keyrgb.r, keyrgb.g, keyrgb.b));
     }
     tex = SDL_CreateTextureFromSurface(renderer,pic);
     SDL_FreeSurface(pic);
     return tex;
}

void turtle_shifting(turtlebay *turtlebay, double angle, Uint32 movedelay)
{    double angle0 = turtlebay->angle;
     double interval = ANGLEINTERVAL/180.0 *R_PI;
     double delta;
     clock_t time;
     int num,i;
     num = abs((int)((angle-angle0)/interval));
     if (num==0)
     {  num=1;
     }
     delta = (angle-angle0)/num;

     for(i=0;i<num && !turtlebay->COLISIONflag;i++)
     {   turtlebay->angle += delta;
         /* choose clock delay, because SDL_delay may conflict on mac platform*/
         time = clock();
         while( (clock()-time)<movedelay*CLOCKS_PER_SEC/1000)
         {   render_turtlebayframe(turtlebay);
         }
         render_turtlebayframe(turtlebay);
     }
}

void turtle_crawling(turtlebay *turtlebay, double x0, double y0, double x, double y, Uint32 movedelay)
{    int numy = abs((int)((y-y0)/INTERVAL));
     int numx = abs((int)((x-x0)/INTERVAL));
     int num,i;
     clock_t time;
     double dx,dy;

     /* if use euclid distance here might make things smoother*/
     num = (numy+numx)/2;
     if(num==0)
     {   num=1;
     }
     dx = (x-x0)/num;
     dy = (y-y0)/num;

     for(i=0;i<num && !turtlebay->COLISIONflag;i++)
     {   turtlebay->x = x0 = x0 + dx;
         turtlebay->y = y0 = y0 + dy;
         /* choose clock delay, because SDL_delay may conflict on mac platform */
         time = clock();
         while( (clock()-time)<movedelay*CLOCKS_PER_SEC/1000)
         {   render_turtlebayframe(turtlebay);
         }
         render_turtlebayframe(turtlebay);
     }
}
