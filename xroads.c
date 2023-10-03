/* XRoads v0.6 -- By Brad Johnson
 * Started sometime in April 1998
 * Based on "Crossroads" and "Crossroads II: Pandemonium"
 * Originally written by Steve Harter for the Commodore 64
 * and published in "Compute!'s Gazette" December 87 & 88
 * All quotations above are probably copyrights or at least
 * trademarks.
 */

/* This program is provided "as is". I accept no responsibility for any
 * problems resulting from the use (or mis-use) or this program in any
 * way. Do whatever you want with this program, as long as you do a few
 * things:
 * 1 - Make sure to give me *some* credit (all you have to do is mention
 *     my name somewhere)
 * 2 - Any use of this code, wether in it's original form or in an altered
 *     state, must be provided free of charge. That goes for binaries AND
 *     source code.
 * That's all for now... have fun!
 */

/*
   Note: 48 = '0'
         49 = '1'
         ...
         56 = '8'
         57 = '9'
*/

#include "config.h"

   /* Prototypes */
#include "xroads.h"
#include "ai.h"
#include "screens.h"
#include "fileops.h"
#include "chardata.h"

   /* X */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

   /* Misc. */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

   /* Needed for Joystick */
#ifdef USEJOYSTICK
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>
#include <fcntl.h>
#endif

   /* Signal handling */
#ifdef SIGHANDLE
#include <signal.h>
#endif

#define VERSION "v0.6b"

/* Global Variables */
Display				*display;
int				screen;
GC				gc/*, scgc*/;
static Cursor			point_cursor = 0;
unsigned long 			f_color, b_color;
Window				mainwindow/*, scwindow*/;
Colormap			colormap;
XFontStruct			*normalfont, *bigfont;
int				walltype, tempint[4], connected=0, level=1;
double				turn=0;
int				active=3, gamedelay=GAMEDELAY, cheatmode=0;
int				numplayers=1, randommazemode=0;
const int			basedelay=80;
#if defined HALFSIZE
char				normalfontname[]="-*-*-bold-r-*-*-*-*-*-*-*-50-*-*";
char				bigfontname[]="-bitstream-*-*-r-*-*-20-*-100-*-*-*-*-*";
#elif defined DOUBLESIZE
char				normalfontname[]="-*-*-bold-r-*-*-*-*-*-*-*-200-*-*";
char				bigfontname[]="-bitstream-*-*-r-*-*-80-*-100-*-*-*-*-*";
#else
char				normalfontname[]="-*-*-bold-r-*-*-*-*-*-*-*-100-*-*";
char				bigfontname[]="-bitstream-*-*-r-*-*-40-*-100-*-*-*-*-*";
#endif
Atom				wm_delete_window, wm_protocols;
Atom				protocols[1];
extern tile_t			tile[NUMCREATURES+1][2];
maze_t				maze;
extern tile_t			walls[NUMWALLS];
extern tile_t			spar[3];
entity				monst[MAXCREATURES];
entity				*player[5] = {0, &monst[0], &monst[1],
				              &monst[2], &monst[3]};
int				p1speed=5;
creature			data[NUMENTITIES+2];
XColor				reqcol[17], hwcol[17];
extern tile_t			arrow, bullet, explosion[4], blank, worm[2];
char				xrmpath[80];
char				targdisplay[80];
char 				directions[]={UP, DOWN, LEFT, RIGHT};
#ifdef USEJOYSTICK
int				joyfd; /* /dev/js0 descriptor */
struct js_event			joyev;
char				jname[128];
#ifdef NOJOYDEFAULT
int				jnum=-1;
#else
int				jnum=0; /* Player who uses joystick */
#endif /* NOJOYDEFAULT */
#endif /* USEJOYSTICK */

struct { int left, right, up, down, fire, alt; } keys[4];

/*************************************************************************************************/
/******************************************* Functions *******************************************/
/*************************************************************************************************/
int power(int x, int y){                                                    /* Simple x to the y */
   int i, answer=x;
   if(y==0) answer=1;
   else for(i=1; i<y; i++)
      answer*=x;
   return answer;
}

/*************************************************************************************************/
void randomize(void) {                 /* Initialize the random number generator using the clock */
   int stime;
   long ltime;
 
   ltime=time(NULL);
   stime=(unsigned) ltime/2;
   srand(stime);
}

/*************************************************************************************************/
void fan_clear(Window window, int width, int height){                   /* Clear screen in style */
   unsigned int f_color = BlackPixel(display, DefaultScreen(display));        /* Needs some work */
   unsigned int b_color = WhitePixel(display, DefaultScreen(display));
   int i;
#ifdef DEBUG
   printf("Fan clear called\n");
#endif
   for(i=0; i<=width; i++){
      XDrawLine(display, window, gc, i, 0, width-i, height);
      XFlush(display);
      }
   XSetBackground(display, gc, f_color);
   XSetForeground(display, gc, b_color);
   for(i=0; i<=width; i++){
      XDrawLine(display, window, gc, i, 0, width-i, height);
      XFlush(display);
      }
   XSetBackground(display, gc, b_color);
   XSetForeground(display, gc, f_color);
   for(i=height; i>=0; i--){ 
      XDrawLine(display, window, gc, 0, i, width, height-i);
      XFlush(display);
      }
   XSetBackground(display, gc, f_color);
   XSetForeground(display, gc, b_color);
   for(i=height; i>=0; i--){
      XDrawLine(display, window, gc, 0, i, width, height-i);
      XFlush(display);
      }
   XSetBackground(display, gc, b_color);
   XSetForeground(display, gc, f_color);
   XClearWindow(display, window);
}

/*************************************************************************************************/
void draw_tilex2 (Window window, tile_t tile, int x, int y, int color, char dir){
                                                                       /* Draw 8x8 tile as 16x16 */
   int i, j;                                                      /* Useful for high resolutions */
/*   printf("Drawing tile at %d, %d (dir %c) (col %d)\n", x, y, dir, color);*/
   XSetForeground(display, gc, hwcol[color].pixel); /* hwcol[color].pixel */
/*   drawsquare(0, 0, color);*/
   XSetForeground(display, gc, hwcol[color].pixel);
   switch(dir) {
   case RIGHT:
      for(i=0; i<=7; i++)
         for(j=0; j<=7; j++)
            if((tile[i]&power(2,j))==power(2,j))
               XFillRectangle(display, window, gc, x+(j*PIXSIZE), y+(i*PIXSIZE), PIXSIZE, PIXSIZE);
      break;
   case LEFT:
      for(i=0; i<=7; i++)
         for(j=0; j<=7; j++)
            if((tile[i]&power(2,7-j))==power(2,7-j))
               XFillRectangle(display, window, gc, x+(j*PIXSIZE), y+(i*PIXSIZE), PIXSIZE, PIXSIZE);
      break;
   case UP:
      for(i=0; i<=7; i++)
         for(j=0; j<=7; j++)
            if((tile[7-j]&power(2,7-i))==power(2,7-i))
               XFillRectangle(display, window, gc, x+(j*PIXSIZE), y+(i*PIXSIZE), PIXSIZE, PIXSIZE);
      break;
   case DOWN:
      for(i=0; i<=7; i++)
         for(j=0; j<=7; j++)
            if((tile[7-j]&power(2,i))==power(2,i))
               XFillRectangle(display, window, gc, x+(j*PIXSIZE), y+(i*PIXSIZE), PIXSIZE, PIXSIZE);
      break;
   case UP2:
      for(i=0; i<=7; i++)
         for(j=0; j<=7; j++)
            if((tile[j]&power(2,7-i))==power(2,7-i))
               XFillRectangle(display, window, gc, x+(j*PIXSIZE), y+(i*PIXSIZE), PIXSIZE, PIXSIZE);
      break;
   case DOWN2:
      for(i=0; i<=7; i++)
         for(j=0; j<=7; j++)
            if((tile[j]&power(2,i))==power(2,i))
               XFillRectangle(display, window, gc, x+(j*PIXSIZE), y+(i*PIXSIZE), PIXSIZE, PIXSIZE);
      break;
   case RIGHT2:
      for(i=0; i<=7; i++)
         for(j=0; j<=7; j++)
            if((tile[7-i]&power(2,j))==power(2,j))
               XFillRectangle(display, window, gc, x+(j*PIXSIZE), y+(i*PIXSIZE), PIXSIZE, PIXSIZE);
      break;
   case LEFT2:
      for(i=0; i<=7; i++)
         for(j=0; j<=7; j++)
            if((tile[7-i]&power(2,7-j))==power(2,7-j))
               XFillRectangle(display, window, gc, x+(j*PIXSIZE), y+(i*PIXSIZE), PIXSIZE, PIXSIZE);
      break;
   default: printf("draw_tilex2: I don't know that direction (%c)\n", dir);
            break;
   }
   XFlush(display);
}

/*************************************************************************************************/
void drawsquare( int x, int y, int color) {       /* Draw a square of color (for debug purposes) */
   int i;
   XSetForeground(display, gc, hwcol[color].pixel);
   for(i=0; i<8; i++)
      XDrawLine(display, mainwindow, gc, x, y+i, x+8, y+i);
   XFlush(display);
}

/*************************************************************************************************/
void AllocColors ( char colname[11], int i ){                       /* Allocate necessary colors */
   int status;
#ifdef DEBUG
   printf("--Colors: %s... Looking up... ", colname); 
#endif
   status = XLookupColor(display, colormap, colname, &reqcol[i], &hwcol[i]);
   if(reqcol[i].pixel!=hwcol[i].pixel) printf("settled... ");
#ifdef DEBUG
   else printf("found!... ");
#endif
   if(status==0) printf("Ooops, error looking up color!\n");
   else {
#ifdef DEBUG
      printf("allocating... ");
#endif
      status = XAllocColor(display, colormap, &hwcol[i]);
      if(status==0) printf("Ooops, error allocating color!\n");
#ifdef DEBUG
      else printf("Done!\n");
#endif
      }
}

/*************************************************************************************************/
void drawscreen( void ){                                               /* draw the whole screen */
/* Note: This function looks messed up... I'll look into it */
/* Things ARE getting redrawn, but it's not in here */
#ifdef DEBUG
   printf("Refreshing screen...\n");
#endif
   drawwalls();
   updatescore();
/*
   draw_tilex2(mainwindow, tile[PLAYER1][player[1]->frame], 
               player[1]->x*GRIDSIZE, player[1]->y*GRIDSIZE,
               player[1]->color, player[1]->dir);*/
/*   for(i=0; i<active; i++)
      if(monst[i].type==BULLET)
         draw_tilex2(mainwindow, bullet, j*GRIDSIZE, i*GRIDSIZE, monst[i].color, RIGHT);
      else if(monst[i].type==ARROW)
         draw_tilex2(mainwindow, arrow, j*GRIDSIZE, i*GRIDSIZE, monst[i].color, RIGHT);
      else if(monst[i].type!=-2)
         draw_tilex2(mainwindow, tile[monst[i].type][monst[i].frame], monst[i].x*GRIDSIZE,
                     monst[i].y*GRIDSIZE, monst[i].color, monst[i].dir);*/
#ifdef DEBUG
   printf("done.\n");
#endif
}

/*************************************************************************************************/
void drawwalls ( void ){                                        /* draw just the walls (quicker) */
   int i, j;
#ifdef DEBUG
   printf("Redrawing map... ");
#endif
   for(i=0; i<GRIDY; i++)
      for(j=0; j<GRIDX; j++)
         if(maze[i][j]==-1) /* Wall */
            draw_tilex2(mainwindow, walls[walltype], j*GRIDSIZE, i*GRIDSIZE, data[WALL].color, RIGHT);
#ifdef DEBUG
   printf("done.\n");
#endif

}

/*************************************************************************************************/
int ntrunc( float num ) {  /* Truncate a float to an int */
   if (num>0) return (int)num;
   else return (int)(num-0.5);
}

/*************************************************************************************************/
int xround( float num ) {  /* Round a float to an int */
   return (int)(num+0.5);
}

/*************************************************************************************************/
int checkdest( int x, int y ) {                          /* Check if x, y is a valid destination */
   icorrectpos(&x, &y);
   if(maze[y][x]<-1)
      return 1;
   else return 0;
}

/*************************************************************************************************/
void icorrectpos( int *x, int *y ) {                          /* Corect coordinates (wrap around) */
   if(*y>=GRIDY) *y=0;
   if(*x>=GRIDX) *x=0;
   if(*y<0) *y=GRIDY-1;
   if(*x<0) *x=GRIDX-1;
}

/*************************************************************************************************/
void correctpos( float *x, float *y) {
   if(*y>GRIDY-1) *y-=GRIDY-0.5;
   if(*x>GRIDX-1) *x-=GRIDX-0.5;
   if(*y<0.0) *y+=GRIDY-0.5;
   if(*x<0.0) *x+=GRIDX-0.5;
}

/*************************************************************************************************/
void watchentity( entity *amonst, entity *target) {      /* Turn the monster to face it's target */
   if(amonst->x > target->x) amonst->dir = LEFT;
   else if(amonst->x < target->x) amonst->dir = RIGHT;
   else if(amonst->y > target->y) amonst->dir = UP;
   else if(amonst->y < target->y) amonst->dir = DOWN;
#ifdef DEBUG
   else printf("--WatchEntity: I couldn\'t turn the monster for some reason. Sorry.\n");
#endif
}

/*************************************************************************************************/
void finddest( entity *amonst ) {                           /* Find a place to move. (Creatures) */
   switch(data[amonst->type].intel){
      case 0: return;
      case 1: deststraight(amonst); break;
      case 2: /*destrand(amonst); break;*/
      case 3:
      case 4: desthome(amonst); break;
      default: printf("Error calculating creature intelligence!\n");
               break;
      }
   return;
}

/*************************************************************************************************/
void creatureturn( entity *amonst, int number ) {                  /* Generic turn for a monster */
   float oldx=amonst->x, oldy=amonst->y;
   entity *bmonst;
   int targ = amonst->target, count=0;
   if((unsigned long int)turn%(amonst->speed)==0){
#ifdef DEBUG
      printf("Looks like it's time for this monster to move (turn %d / %d / %d). F%d\n",
               (unsigned long int)turn, 8-(amonst->speed*2), amonst->speed, amonst->frame);
#endif
      if(amonst->atspeed > 0) amonst->atspeed-=1;
      draw_tilex2(mainwindow, data[amonst->type].tile[amonst->frame],
                  oldx*GRIDSIZE, oldy*GRIDSIZE, BLACK, amonst->dir);

         /* Find a suitable target */
      findtarg(amonst, False);
      targ = amonst->target;
      if(amonst->team == monst[targ].team) findtarg(amonst, True);

         /* Human & Tagteam join */
      if(amonst->type == MUTANT && monst[targ].type == TAGTEAM &&
         (abs(amonst->x - monst[targ].x) <= 1 && abs(amonst->y -
         monst[targ].y) <= 1)) {
         amonst->health = 0;
         killmonster(amonst);
         draw_tilex2(mainwindow, data[monst[targ].type].tile[monst[targ].frame],
                  monst[targ].x*GRIDSIZE, monst[targ].y*GRIDSIZE, BLACK,
                  monst[targ].dir);
         monst[targ].type = TAGTEAM2;         
         monst[targ].atspeed = data[TAGTEAM2].atspeed;
         monst[targ].speed = data[TAGTEAM2].speed;
         monst[targ].range = data[TAGTEAM2].range;
         monst[targ].hurt = -1;
         monst[targ].attr = data[TAGTEAM2].attr | ACTIVE;
         findtarg(&monst[targ], True);
         return;
         }

         /* Get revenge on whoever attacks me */
      if(amonst->hurt != -1 && amonst->team != monst[amonst->hurt].team)
         amonst->target = amonst->hurt;

         /* If the monsters target is unsuitable, go back to a player */
      while ((monst[amonst->target].health <= 0 ||
         monst[amonst->target].type == -1 ||
         data[monst[amonst->target].type].etype == POWERUP) &&
         count < 100) {
            findtarg(amonst, False);
            count++;
            }

         /* It's more fun to watch mosters kill each other if they're
            actually good at it */
/*
      if(amonst->hurt >= 4) amonst->range = 20;
      else amonst->range = data[amonst->type].range;
*/

         /* Fire a bullet at the target if it has a chance of hitting */
      if((data[amonst->type].attack1==ARROWS ||
          data[amonst->type].attack1==BULLETS ||
          data[amonst->type].attack1==WORMS) &&
          clearshot(amonst, &monst[targ]) &&
          amonst->team != monst[targ].team &&
          data[monst[targ].type].etype != POWERUP) {
         watchentity(amonst, &monst[targ]);
         spawnbullet(amonst);
         }

         /* Bite the target */
      if(data[amonst->type].attack1==MELEE &&
         amonst->team != monst[targ].team)
         if( (amonst->x==monst[targ].x && abs(amonst->y - monst[targ].y) <= 1)
          || (amonst->y==monst[targ].y && abs(amonst->x - monst[targ].x) <= 1)) {
            if(data[monst[targ].type].etype != POWERUP) {
               watchentity(amonst, &monst[targ]);
               biteattack(amonst, &monst[targ]);
               }
            }

         /* VacAttack */
      if(data[amonst->type].attack1==VACCUUM && (xround(amonst->x) ==
         ntrunc(amonst->x) && xround(amonst->y) == ntrunc(amonst->y)) &&
         clearshot(amonst, &monst[targ]) && amonst->team!=monst[targ].team
         && data[monst[targ].type].etype != POWERUP) {
         watchentity(amonst, &monst[targ]);
         vacattack(amonst, &monst[targ]);
         }
      else {
         finddest(amonst);
            /* Check for pulls */
         if(amonst->attr & PULL_U) { amonst->y-=1; amonst->attr &= ~PULL_U; }
         if(amonst->attr & PULL_D) { amonst->y+=1; amonst->attr &= ~PULL_D; }
         if(amonst->attr & PULL_L) { amonst->x-=1; amonst->attr &= ~PULL_L; }
         if(amonst->attr & PULL_R) { amonst->x+=1; amonst->attr &= ~PULL_R; }
         correctpos(&(amonst->x), &(amonst->y));
         }

         /* Animate the character */
      if(amonst->frame >= data[amonst->type].frames-1) {
         if(data[amonst->type].etype!=EFFECT)
            amonst->frame = 0;
         else
            amonst->health = -10;
         }
      else amonst->frame++;

      if(data[amonst->type].etype!=EFFECT) {
         maze[xround(oldy)][xround(oldx)]=-2; /* Blank out old space */
         maze[ntrunc(oldy)][ntrunc(oldx)]=-2;
         maze[xround(amonst->y)][xround(amonst->x)]=number;
         maze[ntrunc(amonst->y)][ntrunc(amonst->x)]=number;
         }

         /* If there was something underneath the effect, redraw it */
      if(maze[ntrunc(amonst->y)][ntrunc(amonst->x)]!=-2) {
         bmonst=&monst[maze[ntrunc(amonst->y)][ntrunc(amonst->x)]];
         draw_tilex2(mainwindow, data[bmonst->type].tile[bmonst->frame], (bmonst->x)*GRIDSIZE,
                     (bmonst->y)*GRIDSIZE, bmonst->color, bmonst->dir);
         }
      if(maze[xround(amonst->y)][xround(amonst->x)]!=-2) {
         bmonst=&monst[maze[xround(amonst->y)][xround(amonst->x)]];
         draw_tilex2(mainwindow, data[bmonst->type].tile[bmonst->frame], (bmonst->x)*GRIDSIZE,
                     (bmonst->y)*GRIDSIZE, bmonst->color, bmonst->dir);
         }

      draw_tilex2(mainwindow, data[amonst->type].tile[amonst->frame],
                  (amonst->x)*GRIDSIZE, (amonst->y)*GRIDSIZE,
                  amonst->color, amonst->dir);

         /* If none of the other functions caught it, kill the monster */
      if((amonst->type == -1)||(amonst->health <= 0)) {
         killmonster(amonst);
         return;
         }

#ifdef DEBUG
      printf("Monster: %d, (%d, %d) int: %d col: %d Wall: %d\n", amonst->type, amonst->x, amonst->y,
             data[amonst->type].intel, data[amonst->type].color, data[WALL].color);
#endif
      }
}

/*************************************************************************************************/
void keyhandle (KeySym key, int pressed) {  /* Watch keypresses and update pressedkeys settings */
   switch(key) {
      case P1UP : keys[0].up = pressed; break;
      case P1DOWN : keys[0].down = pressed; break;
      case P1RIGHT : keys[0].right = pressed; break;
      case P1LEFT : keys[0].left = pressed; break;
      case P1FIRE : keys[0].fire = pressed; break;
      case P1ALT : keys[0].alt = pressed; break;

      case P2UP : keys[1].up = pressed; break;
      case P2DOWN : keys[1].down = pressed; break;
      case P2LEFT : keys[1].left = pressed; break;
      case P2RIGHT : keys[1].right = pressed; break;
      case P2FIRE : keys[1].fire = pressed; break;
      case P2ALT : keys[1].alt = pressed; break;

      case P3UP : keys[2].up = pressed; break;
      case P3DOWN : keys[2].down = pressed; break;
      case P3LEFT : keys[2].left = pressed; break;
      case P3RIGHT : keys[2].right = pressed; break;
      case P3FIRE : keys[2].fire = pressed; break;
      case P3ALT : keys[2].alt = pressed; break;

      case P4UP : keys[3].up = pressed; break;
      case P4DOWN : keys[3].down = pressed; break;
      case P4LEFT : keys[3].left = pressed; break;
      case P4RIGHT : keys[3].right = pressed; break;
      case P4FIRE : keys[3].fire = pressed; break;
      case P4ALT : keys[3].alt = pressed; break;

      case XK_r :
      case XK_R : if(pressed) drawscreen(); break;
#ifndef NOPTOPAUSE
      case XK_p :
      case XK_P :
#endif
      case XK_Pause : if(pressed) pausegame(); break;
      case RESTARTKEY : if(pressed) initmap(randommazemode?-2:-1); break;
      case QUITKEY : if(pressed) killme("Game ended by user (Quit key hit)");
      case XK_F11 : data[PLAYER1].attack1=ARROWS; break;
      case XK_F12 : data[PLAYER1].attack1=BULLETS; break;
      default : if(pressed) printf("This key is not used (%s)\n", XKeysymToString(key));
      }
}

/*************************************************************************************************/
void playerturn ( entity *player ) {              /* Interpret players keypresses */
   float newx=player->x, newy=player->y;
   int eqx=player->x, eqy=player->y;
   int pnum = player->ind;
   char okay=0, olddir=player->dir;

      /* All four players info are held in different positions in data[]
         Player 1 is at 0, but 2, 3, and 4 are at 31, 32, and 33 */
   int datapos;
   if(player->ind > 0) datapos = player->ind + 30;
   else datapos = 0;

      /* If the player is not active, then out he goes */
   if(!(player->attr & ACTIVE)) return;

#ifdef DEBUG
   printf("Character moving... ");
#endif

   if(keys[pnum].fire) spawnbullet(player);

      /* If a key is pressed, animate */
   if(keys[pnum].up||keys[pnum].down||keys[pnum].left||keys[pnum].right)
      player->frame = (player->frame==0) ? 1 : 0;

/*
   if(keys[pnum].up) { player->dir=UP; newy-=0.5; }
   else if(keys[pnum].down) { player->dir=DOWN; newy+=0.5; }
   else if(keys[pnum].right) { player->dir=RIGHT; newx+=0.5; }
   else if(keys[pnum].left) { player->dir=LEFT; newx-=0.5; }
*/

   if(keys[pnum].up) player->dir=UP;
   else if(keys[pnum].down) player->dir=DOWN;
   else if(keys[pnum].right) player->dir=RIGHT;
   else if(keys[pnum].left) player->dir=LEFT;

   havedir:

   if(keys[pnum].up||keys[pnum].down||keys[pnum].left||keys[pnum].right)
      switch(player->dir) {
         case UP: newy-=0.5; break;
         case DOWN: newy+=0.5; break;
         case LEFT: newx-=0.5; break;
         case RIGHT: newx+=0.5; break;
         }

      /* Check for pulls */
   if(player->attr & PULL_U) { newy-=1; player->attr &= ~PULL_U; }
   else if(player->attr & PULL_D) { newy+=1; player->attr &= ~PULL_D; }
   else if(player->attr & PULL_L) { newx-=1; player->attr &= ~PULL_L; }
   else if(player->attr & PULL_R) { newx+=1; player->attr &= ~PULL_R; }

      /* Only one half-step at a time */
   if(xround(newx)!=ntrunc(newx) && xround(newy)!=ntrunc(newy)) {
#ifdef DEBUG
      printf("Damn, in two half-steps at a time...\n");
#endif
      switch(olddir) {
         case UP: newy=player->y-0.5; newx=player->x; break;
         case DOWN: newy=player->y+0.5; newx=player->x; break;
         case LEFT: newx=player->x-0.5; newy=player->y; break;
         case RIGHT: newx=player->x+0.5; newy=player->y; break;
         default: printf("Something screwed up.\n"); break;
         }
      player->dir = olddir;
      }

      /* If we're in a half-step, eqx and eqy are the space we're moving
         into */
   if(xround(newx)!=ntrunc(newx)) {
      eqy = ntrunc(newy);
      if(newx > player->x)
         eqx = xround(newx);
      else if(newx < player->x)
         eqx = ntrunc(newx);
      }
   else if(xround(newy)!=ntrunc(newy)) {
      eqx = ntrunc(newx);
      if(newy > player->y)
         eqy = xround(newy);
      else if(newy < player->y)
         eqy = ntrunc(newy);
      }
   else {
      eqx = ntrunc(newx);
      eqy = ntrunc(newy);
      }
/*
   switch(player->dir) {
      case LEFT: eqx = ntrunc(newx); break;
      case RIGHT: eqx = xround(newx); break;
      case UP: eqy = ntrunc(newy); break;
      case DOWN: eqy = xround(newy); break;
      default: break;
      }
*/
   icorrectpos(&eqx, &eqy);

   correctpos(&newx, &newy);

#ifdef DEBUG
   if(newx!=player->x || newy!=player->y) {
   printf("Destination (%0.2f, %0.2f)...\n", newx, newy);
   printf("Equiv (%d, %d)\n", eqx, eqy);
   printf("@ %d, %d : %d\n", eqx, eqy, maze[eqy][eqx]);
   printf("newx: %0.2f: round %d, ntrunc %d\n", newx, xround(newx), ntrunc(newx));
   printf("newy: %0.2f: round %d, ntrunc %d\n", newy, xround(newy), ntrunc(newy));
   printf("Checkdest: %d\n", checkdest(eqx, eqy));
   printf("-----\n");
   }
#endif

   okay = checkdest(eqx, eqy);
   if(maze[eqy][eqx] == player->ind) okay = 1;

         /* Okay, if the destination was okay, check for powerups */
   if(data[xytotype(eqx, eqy)].etype==POWERUP) {
      if(player->health<9) player->health++;
      player->spars++;
      if(monst[maze[eqy][eqx]].type==BLUESPAR)
         data[datapos].speed=1;
      if(monst[maze[eqy][eqx]].type==REDSPAR)
         data[datapos].atspeed=5;
      monst[maze[eqy][eqx]].health=-10;
      monst[maze[eqy][eqx]].hurt=player->ind;
      killmonster(&monst[maze[eqy][eqx]]);
      }
   else if(maze[eqy][eqx] == -1) {
#ifdef DEBUG
      printf("Wall in the way\n");
#endif
      newx=player->x;
      newy=player->y;
      if(player->dir != olddir) {
         player->dir = olddir;
         goto havedir;
         }
      }
   else if(!okay) {
#ifdef DEBUG
      printf("something in the way. (%d or %d)\n",
             maze[ntrunc(newy)][ntrunc(newx)],
             maze[xround(newy)][xround(newy)]);
#endif
      newx=player->x;
      newy=player->y;
      }

#ifdef DEBUG
   else printf("success.\n");
#endif

     /* Destination has been set, now all we have to do is move there */
   maze[xround(player->y)][xround(player->x)]=-2;
   maze[ntrunc(player->y)][ntrunc(player->x)]=-2;
   draw_tilex2(mainwindow, blank, player->x*GRIDSIZE, player->y*GRIDSIZE,
               BLACK, RIGHT);

      /* Draw player */
   player->x=newx; player->y=newy;
   maze[xround(player->y)][xround(player->x)]=player->ind;
   maze[ntrunc(player->y)][ntrunc(player->x)]=player->ind;
   draw_tilex2(mainwindow, data[datapos].tile[player->frame],
               player->x*GRIDSIZE, player->y*GRIDSIZE,
               player->color, player->dir);
}

/*************************************************************************************************/
void getcols( void ) {                                                /* Allocate all the colors */
#ifndef MONO
   AllocColors("black", BLACK);
   AllocColors("white", WHITE);
   AllocColors("pink2", PINK);
   AllocColors("red", RED);
   AllocColors("light green", LGREEN);
   AllocColors("lime green", GREEN);
   AllocColors("forest green", DGREEN);
   AllocColors("dodger blue", LBLUE);
   AllocColors("blue", BLUE);
   AllocColors("dark blue", DBLUE);
   AllocColors("cyan", CYAN);
   AllocColors("yellow", YELLOW);
   AllocColors("dark grey", LGREY);
   AllocColors("dark slate grey", DGREY);
   AllocColors("magenta", PURPLE);
   AllocColors("orange2", ORANGE);
   AllocColors("saddle brown", BROWN);
#endif
#ifdef MONO
   int i;
   AllocColors("black", BLACK);
   for(i=WHITE; i<=BROWN; i++)
      AllocColors("white", i);
#endif
}

/*************************************************************************************************/
void initmap( int mapnum ){                   /* Load a map and set up some default start values */
   int i;
   char levelname[20], levelnum[5];
   for(i=0; i<4; i++) {
      keys[i].up = False;
      keys[i].down = False;
      keys[i].left = False;
      keys[i].right = False;
      keys[i].fire = False;
      keys[i].alt = False;
      }

#ifdef DEBUG
   printf("Setting up maze...\n");
#endif
   XSync(display, True);
   if(mapnum==-1) mapnum=(int)rnd(NUMMAPS);
   strcpy(levelname, "Level ");
   sprintf(levelnum, "%d", level);
   strcat(levelname, levelnum);
   screenmsg(levelname, "");
   XClearWindow(display, mainwindow);
   for(i=0; i<MAXCREATURES; i++) monst[i].type=-1;

   readmapfile(mapnum);

   for(i=1; i<=4; i++) {
      player[i]->attr = 0;
      player[i]->health = 0;
      }

      /* Green Player */
   player[1]->x=1; player[1]->y=GRIDY-2;
   player[1]->type=PLAYER1;
   player[1]->dir=RIGHT;
   player[1]->color=data[PLAYER1].color;

      /* Red Player */
   player[2]->x=GRIDX-2; player[2]->y=GRIDY-2;
   player[2]->type=PLAYER2;
   player[2]->dir=LEFT;
   player[2]->color=data[PLAYER2].color;
 
      /* Yellow Player */
   player[3]->x=1; player[3]->y=1;
   player[3]->type=PLAYER3;
   player[3]->dir=RIGHT;
   player[3]->color=data[PLAYER3].color;
 
      /* Blue Player */
   player[4]->x=GRIDX-2; player[4]->y=1;
   player[4]->type=PLAYER4;
   player[4]->dir=LEFT;
   player[4]->color=data[PLAYER4].color;

      /* Common to each player */
   for(i=1; i<=numplayers; i++) {
      player[i]->spars = 0;
      player[i]->frame = 0;
      player[i]->atspeed = 0;
      player[i]->ind = i-1;
      player[i]->attr = ACTIVE;
      player[i]->health = data[PLAYER1].health;
      maze[(int)player[i]->y][(int)player[i]->x]=player[i]->ind; /* Player */
      }
 
  for(i=0; i<(STARTMONSTERS); i++)
     makemonst((int)rnd(NUMCREATURES-1)+1, RIGHT, -1, -1, -1, -1);
#ifndef NOSPARS
   if(SPARSPERLEVEL<=5) {
      makemonst(SPAR, RIGHT, -1, -1, -1, -1);
      makemonst(SPAR, RIGHT, -1, -1, -1, -1);
      }
   else
      for(i=0; i<(int)(SPARSPERLEVEL/5); i++)
         makemonst(SPAR, RIGHT, -1, -1, -1, -1);
#endif

/* setcols() was a kludge to fix a bug, I hope it isn't needed anymore */
/*   setcols();*/
   walltype=(int)rnd(NUMWALLS);
   do {
      data[WALL].color=(int)rnd(NUMCOLORS);
      } while(data[WALL].color==BLACK);

#ifdef DEBUG
   printf("Done setting up\nDrawing... \n");
#endif
   drawscreen();
   XSetBackground(display, gc, f_color);
   XSetForeground(display, gc, b_color);
   XFlush(display);
}

/*************************************************************************************************/
void init(int argc, char *argv[]){                                         /* Parse command line */
/* Wow, this function could use a little work... Oh well, it does what it
   needs to, so I'm not too concerned */
   int i;
   printf("XRoads %s - By Brad Johnson (foxtrot@autobahn.mb.ca)\n", VERSION);
   printf("http://www.geocities.com/SiliconValley/Way/1795/xroads.html\n");
   strcpy(targdisplay, "NONE");

   for(i=1; i<argc; i++) {
      if(!strcmp(argv[i], "-help") || !strcmp(argv[i], "--help")) {
         showhelp();
         exit(1);
         }
      if(!strcmp(argv[i], "-keys")) {
         showkeys();
         exit(1);
         }
      if(!strcmp(argv[i], "-version") || !strcmp(argv[i], "--version")) {
         exit(1);
         }   
    }
    printf("\"xroads -help\" for command line parameters\n");
    for (i=1; i<argc; i++) {
      if(!strcmp(argv[i], "-display") || !strcmp(argv[i], "--display")) {
         printf("Using display: %s\n", argv[i+1]);
         strcpy(targdisplay, argv[i+1]);
         }
      if(!strcmp(argv[i], "-xrm")) {
         printf("XRM path set to %s\n", argv[i+1]);
         strcpy(xrmpath, argv[i+1]);  
         if(xrmpath[strlen(xrmpath)-1] != '/')
            strcat(xrmpath, "/");
         }
      if(!strcmp(argv[i], "-p")) {
         numplayers = atoi(argv[i+1]);
         printf("Enabling %d player mode\n", numplayers);
         if(numplayers>4 || numplayers<1) {
            printf("Wait! I can't do that! Back to single player...\n");
            numplayers=1;
            }
         }
#ifdef USEJOYSTICK
      if(!strcmp(argv[i], "-j")) {
         jnum = atoi(argv[i+1])-1;
         }
#endif
      if(!strcmp(argv[i], "-delay")) {
         printf("Delay set to %d\n", atoi(argv[i+1]));
         gamedelay=atoi(argv[i+1]);
         }
      if(!strcmp(argv[i], "-hyper")) {
         printf("Hyper Mode enabled!\n");
         gamedelay=0;
         }
      if(!strcmp(argv[i], "-cheat")) {
         printf("Cheat Mode enabled!\n");
         cheatmode=1;
         }
      if (!strcmp(argv[i], "-maze")) {
         printf("Random Maze Generator enabled!\n");
         randommazemode=1;
      }
   }
}

/*************************************************************************************************/
void showhelp( void ) {                           /* Print out a list of command line parameters */
   printf("\n\nUsage: xroads [-display disp] [-delay #] [-xrm /path/to/files/]\n");
   printf("              [-hyper] [-cheat] [-help]\n\n");
   printf("-display disp : Use disp as the target display\n");
   printf("-delay #      : Use # as a value to delay by\n");
   printf("-xrm /path/   : Tell XRoads the location of your XRM files\n");
   printf("-p #          : Play with # players (1 to 4)\n");
#ifdef USEJOYSTICK
   printf("-j #          : Control player number # with joystick\n");
   printf("-j 0          : Disable the joystick\n");
#endif
   printf("-hyper        : Hyper Mode\n");
   printf("-cheat        : Unlimited health\n");
   printf("-maze         : Generate random mazes\n");
   printf("-help         : Show this message\n");
   printf("-keys         : Show keyboard configuration\n");
   printf("-version      : Show version number and exit\n");
   printf("\n");
   printf("Example: \"xroads -display localhost:0.1 -xrm /usr/local/xroads/\"\n\n");
}

/*************************************************************************************************/
void showkeys( void ) {
   printf("Green:         Up : %s\n", XKeysymToString(P1UP));
   printf("             Down : %s\n", XKeysymToString(P1DOWN));
   printf("             Left : %s\n", XKeysymToString(P1LEFT));
   printf("            Right : %s\n", XKeysymToString(P1RIGHT));
   printf("             Fire : %s\n\n", XKeysymToString(P1FIRE));

   printf("Red:           Up : %s\n", XKeysymToString(P2UP));
   printf("             Down : %s\n", XKeysymToString(P2DOWN));
   printf("             Left : %s\n", XKeysymToString(P2LEFT));
   printf("            Right : %s\n", XKeysymToString(P2RIGHT));
   printf("             Fire : %s\n\n", XKeysymToString(P2FIRE));

   printf("Yellow:        Up : %s\n", XKeysymToString(P3UP));
   printf("             Down : %s\n", XKeysymToString(P3DOWN));
   printf("             Left : %s\n", XKeysymToString(P3LEFT));
   printf("            Right : %s\n", XKeysymToString(P3RIGHT));
   printf("             Fire : %s\n\n", XKeysymToString(P3FIRE));

   printf("Blue :         Up : %s\n", XKeysymToString(P4UP));
   printf("             Down : %s\n", XKeysymToString(P4DOWN));
   printf("             Left : %s\n", XKeysymToString(P4LEFT));
   printf("            Right : %s\n", XKeysymToString(P4RIGHT));
   printf("             Fire : %s\n\n", XKeysymToString(P4FIRE));

   printf("          Restart : %s\n", XKeysymToString(RESTARTKEY));
   printf("       Pause Game : Pause\n");
   printf("    Redraw screen : R\n");
   printf("        Quit game : %s\n", XKeysymToString(QUITKEY));
}   

/*************************************************************************************************/
void clientmessage( XClientMessageEvent *messageevent ) {   /* Process Client Messages (useless) */
   int i;
   printf("What the..!?!? I got a message from somewhere!\n");
   printf("The message is: ");
   printf("(%d-bit data) ", messageevent->format);
   if(messageevent->format==8)
      printf("%s", messageevent->data.b);
   else if(messageevent->format==16)
      for(i=0; i<10; i++)
         printf("%d, ", messageevent->data.s[i]);
   else if(messageevent->format==32)
      for(i=0; i<5; i++)
         printf("%d, ", messageevent->data.s[i]);
   else printf("Ooops! I can't understand the message.");
   printf("\n");
}

/*************************************************************************************************/
int xytotype( int x, int y ) {        /* Get monster type from X-Y position (to avoid segfaults) */
   icorrectpos(&x, &y);
   if(maze[y][x] < 0) return 0;
   else return monst[maze[y][x]].type;
}

/*************************************************************************************************/
void killme( char *reason ) {                                                /* Exit the program */
#ifdef DEBUG
   printf("I was told to exit because: %s\n", reason);
#endif
   /* Don't try to disconnect if the connection didn't even work */
   if (connected == 1) {
      XAutoRepeatOn(display);
      XSync(display, True);
      XFreeGC(display, gc);
      XDestroyWindow(display, mainwindow);
      XCloseDisplay(display);
      }
/*
   printf("Bye!\n");
*/
   exit(1);
}

/*************************************************************************************************/
void xconnect(int argc, char *argv[]) {  /* Generic X Server connection code (so I can reuse it) */

   char				title[]="XRoads";
   Window			parent;
   XSizeHints			hint/*, schint*/;
   int				depth;
   unsigned long int		event_mask;
   static char			data[8] = { 0x01 };
   Pixmap			bitmap;
   XColor			black = { 0 };

#ifdef DEBUG
   printf("Connecting to X server...\n");
#endif
   if(strcmp(targdisplay, "NONE") == 0)
      display = XOpenDisplay((char *)NULL);
   else
      display = XOpenDisplay(targdisplay);
   if(display==NULL) {
      printf("Failed to open display! Check the DISPLAY environment variable\n");
      killme("Fatal error opening display");
      }
   else connected = 1; /* Successful connection established */
   screen = DefaultScreen(display);
   XAutoRepeatOff(display);

#ifdef DEBUG
   printf("Connection established. Retrieving information...\n");
   printf("Resolution: %dx%d\n", DisplayWidth(display, screen),
                                 DisplayHeight(display, screen));
   printf("Allocating colors... ");
#endif
   colormap = DefaultColormap(display, screen);
   depth = DefaultDepth(display, screen);
#ifdef DEBUG
   printf("%d bpp\n", depth);
   printf("Attempting to allocate all %d colors...\n", NUMCOLORS);
#endif
   getcols();
   setcols();
   f_color = WhitePixel(display, screen);
   b_color = BlackPixel(display, screen);
   parent=RootWindow(display, screen);
   hint.x=100;
   hint.y=100;
   hint.width=WIN_X;
   hint.height=WIN_Y+SCBOXSIZE*numplayers;
   hint.max_width=WIN_X;
   hint.min_width=WIN_X;
   hint.max_height=WIN_Y+SCBOXSIZE*numplayers;
   hint.min_height=WIN_Y+SCBOXSIZE*numplayers;
   hint.flags=PPosition | PSize | PMinSize | PMaxSize;
#ifdef DEBUG
   printf("Window info set, creating...");
#endif
   mainwindow = XCreateSimpleWindow(display, parent, hint.x, hint.y,
                hint.width, hint.height, 5, f_color, b_color);
   XSetStandardProperties(display, mainwindow, title, title, None, argv,
                          argc, &hint);

     /* Turn on WM_DELETE_WINDOW; "Borrowed" from XSokoban */
  wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", 0);
  wm_protocols = XInternAtom(display, "WM_PROTOCOLS", 0);
  protocols[0] = wm_delete_window;
  XSetWMProtocols(display, mainwindow, protocols, 1);

#ifdef DEBUG
   printf(" gc\'ing...");     
#endif
   gc = XCreateGC(display, mainwindow, 0, 0);
   setcols();
   XSetBackground(display, gc, b_color);
   XSetForeground(display, gc, f_color);
#ifdef DEBUG
   printf(" displaying...\n");
#endif
   XMapWindow(display, mainwindow);
   XFlush(display);
#ifdef DEBUG
   printf("Attempting to load font: %s\n", normalfontname);
#endif 
   normalfont = XLoadQueryFont(display, normalfontname);
   if(normalfont == (XFontStruct *)NULL) {
      printf("Error loading font! (%s)\n", normalfontname);
      killme("Fatal error loading font");
      }
#ifdef DEBUG
   printf("Attempting to load font %s\n", bigfontname);
#endif
   bigfont = XLoadQueryFont(display, bigfontname);
   if(normalfont == (XFontStruct *)NULL) {
      printf("Error loading font! (%s)\n", bigfontname);
      killme("Fatal error loading font");
      }
   XSetFont(display, gc, normalfont->fid);
#ifdef DEBUG
   printf("Displayed. Setting up Events...\n");
#endif

/* Set up events to respond to during gameplay */
/* Needs too respond to: Keyboard, Button clicks, Exposures and client messages */

   event_mask = ButtonPressMask | ButtonReleaseMask | ExposureMask |
                KeyPressMask | SubstructureNotifyMask | KeyReleaseMask;
   XSelectInput(display, mainwindow, event_mask);

      /* Get rid of cursor, "Borrowed" from Snes9X */
   bitmap = XCreateBitmapFromData(display, mainwindow, data, 8, 8);
   point_cursor = XCreatePixmapCursor(display, bitmap, bitmap,
                  &black, &black, 0, 0);
   XDefineCursor(display, mainwindow, point_cursor);
}

/*************************************************************************************************/
void pausegame( void ) {
   int i;
   for(i=0; i<4; i++) {
      keys[i].up = False;
      keys[i].down = False;
      keys[i].left = False;
      keys[i].right = False;
      keys[i].fire = False;
      keys[i].alt = False;
      }
   screentxt("Paused.", "");
   drawscreen();
}

#ifdef USEJOYSTICK
/*************************************************************************************************/
void readjoy( void ) {  /* Read joystick */
   while(read(joyfd, &joyev, sizeof(struct js_event))!=-1)
      switch(joyev.type & ~JS_EVENT_INIT) {
         case JS_EVENT_AXIS:
            switch(joyev.number) {
               case 0:
                  if(joyev.value > JS_SENS) {
                     keys[jnum].right = 1;
                     keys[jnum].left = 0;
                     }
                  else if(joyev.value < -JS_SENS) {
                     keys[jnum].right = 0;
                     keys[jnum].left = 1;
                     }
                  else {
                     keys[jnum].right = 0;
                     keys[jnum].left = 0;
                     }
                  break; /* case 0 */
               case 1:
                  if(joyev.value > JS_SENS) {
                     keys[jnum].down = 1;
                     keys[jnum].up = 0;
                     }
                  else if(joyev.value < -JS_SENS) {
                     keys[jnum].down = 0;
                     keys[jnum].up = 1;
                     }
                  else {
                     keys[jnum].down = 0;
                     keys[jnum].up = 0;
                     }
                  break; /* case 1 */
               default: break; /* Unused Axis */
               } /* switch(joyev.number) */
            break; /* case JS_EVENT_AXIS */
         case JS_EVENT_BUTTON:
            switch(joyev.number) {
               case 0:
                  if(joyev.value) keys[jnum].fire = 1;
                  else keys[jnum].fire = 0; break;
               case 1:
                  if(joyev.value) keys[jnum].alt = 1;
                  else keys[jnum].alt = 0; break;
               default: break; /* Unused Button */
               } /* switch(joyev.number) */
            break; /* case JS_EVENT_BUTTON */
         default:
            printf("joystick: Unknown event\n");
            break;
         } /* switch(joyev.type & ~JS_EVENT_INIT) */
}
#endif

#ifdef SIGHANDLE
/*************************************************************************************************/
void sigcatch( int sig ) {
   printf("Ack! Signal %d! I'm melting!!\n", sig);
      /* Clean up X connection */
   if (connected == 1) {
      XAutoRepeatOn(display);
      XSync(display, True);
      XFreeGC(display, gc);
      XDestroyWindow(display, mainwindow);
      XCloseDisplay(display);
      }
      /* Re-raise the signal so that we exit naturally */
   signal(sig, SIG_DFL);
   raise(sig);
}
#endif

/*************************************************************************************************/
/****************************************   Main Program   ***************************************/ 
/*************************************************************************************************/
int main(int argc, char *argv[])
{
   XEvent			event;
   int				i;

   randomize();
   strcpy(xrmpath, XRMPATH); /* Set default path */
   init(argc, argv);

   xconnect(argc, argv); /* Set up connection and create window */

   /* Open Joystick */
#ifdef USEJOYSTICK
   if(jnum != -1) {
      if((joyfd = open("/dev/js0", O_RDONLY | O_NONBLOCK)) == -1)
         printf("Could not open /dev/js0\n");
      else {
         if(ioctl(joyfd, JSIOCGNAME(sizeof(jname)), &jname) < 0)
            strncpy(jname, "Unknown", sizeof(jname));
         printf("Joystick: %s\n", jname);
         if(jnum<-1 || jnum>numplayers) jnum = 0;
         printf("Joystick enabled for player %d\n", jnum+1);
         }
      }
#endif

   /* Define Signal Handlers */
#ifdef SIGHANDLE
      /* If parent has already decided we should ignore a signal,
         then we will honour that */
   if(signal(SIGSEGV, sigcatch) == SIG_IGN) signal(SIGSEGV, SIG_IGN);
   if(signal(SIGBUS, sigcatch) == SIG_IGN) signal(SIGBUS, SIG_IGN);
   if(signal(SIGTERM, sigcatch) == SIG_IGN) signal(SIGTERM, SIG_IGN);
   if(signal(SIGINT, sigcatch) == SIG_IGN) signal(SIGINT, SIG_IGN);
   if(signal(SIGHUP, sigcatch) == SIG_IGN) signal(SIGHUP, SIG_IGN);
#endif

#ifdef DEBUG
   printf("Pausing for Window...\n");
#endif
   delay(200);

startgame: /*** Come back here to play again ***/

   level = 1;
   for(i=1; i<=numplayers; i++)
      player[i]->score = 0;

#ifdef DEBUG
   printf("About to run the title screen...\n");
#endif
   screenmsg("Welcome to XRoads!", "Press \'Enter\' to start.");
   /*titlescreen();*/

#ifdef DEBUG
   printf("Setting up Character Data... \n");
#endif
   setup();

   /* Load up a map to start with */
   initmap(randommazemode?-2:-1);

#ifdef DEBUG
   printf("Entering main game loop...\n");
#endif

/***********************************/
/* Main Game Loop Starts With This */
/***********************************/

mainloop:
   turn++;
   if(turn>2000000000) turn=0;
   for(i=1; i<=numplayers; i++) {
      if(player[i]->atspeed>0) player[i]->atspeed-=1;
      if(player[i]->speed>0) player[i]->speed-=1;
      }

      /* Handle X events */
   if(XPending(display)) {
#ifdef DEBUG
      printf("Event caught! Running through Event handler...\n");
#endif
      goto handleevent; /* Branch to Event Handler */
      }

#ifdef USEJOYSTICK
   if(jnum != -1) readjoy();
#endif

      /* Players take their turns */
   for(i=1; i<=numplayers; i++)
      if(((player[i]->speed == 0)||(gamedelay==0))&&(player[i]->health>0)) {
         playerturn(player[i]);
         switch(i) {
            case 1: player[i]->speed = data[PLAYER1].speed; break;
            case 2: player[i]->speed = data[PLAYER2].speed; break;
            case 3: player[i]->speed = data[PLAYER3].speed; break;
            case 4: player[i]->speed = data[PLAYER4].speed; break;
            }
         }

   if(gamedelay!=0) usleep(gamedelay);

      /* Monsters take their turns */
   for(i=4; i<active; i++)
      if(monst[i].type!=-1)
         creatureturn(&monst[i], i);

   if(cheatmode) for(i=1; i<=numplayers; i++) player[i]->health=10;

      /* Check for dead players */
   for(i=1; i<=numplayers; i++)
      if(player[i]->health<=0) {
         player[i]->health = 0;
         if(player[i]->attr & ACTIVE) {
            player[i]->attr ^= ACTIVE;
            maze[xround(player[i]->y)][xround(player[i]->x)] = -2;
            maze[ntrunc(player[i]->y)][ntrunc(player[i]->x)] = -2;  
            draw_tilex2(mainwindow, blank, player[i]->x*GRIDSIZE,
                        player[i]->y*GRIDSIZE, BLACK, RIGHT);
            monst[player[i]->hurt].score += data[player[i]->type].value;
#ifdef XPLOSIONS
            makemonst(EXPLOSION, directions[rnd(4)], player[i]->x,
                      player[i]->y, player[i]->color, -1);
#endif
            }
         }

      /* Check for level exit */
   if(player[1]->spars>=SPARSPERLEVEL || player[2]->spars>=SPARSPERLEVEL ||
      player[3]->spars>=SPARSPERLEVEL || player[4]->spars>=SPARSPERLEVEL) {
      for(i=1; i<=numplayers; i++) {
         tempint[i-1] = player[i]->health; /* Carry health to next map */
         player[i]->score+=500;
         }
      level++;
      initmap(randommazemode?-2:-1);
      for(i=1; i<=numplayers; i++)
         player[i]->health = tempint[i-1];
      updatescore();
      }
   if(player[1]->health + player[2]->health + player[3]->health +
      player[4]->health == 0)
      goto playerdied;
   else
      goto mainloop; /* Branch back to main look for another cycle */

/*************************/
/* End of Main Game Loop */
/*************************/

/* Event Handler */

handleevent:
   XNextEvent(display, &event);
      switch (event.type){
         case ButtonPress: 
#ifdef DEBUG
                           printf("Mouse button pressed.\n");
#endif
                           break;
         case ButtonRelease:
#ifdef DEBUG
                             printf("Mouse button released. Exiting.\n");
#endif
                             /*killme("Game ended by user (Mouse click)");*/
                             break;
         case ClientMessage: if (event.xclient.message_type == wm_protocols &&
                                 event.xclient.data.l[0] == wm_delete_window)
                                killme("Window Manager killed me");
                             clientmessage(&event.xclient);
                             break;
         case Expose: if(event.xexpose.count==0) drawscreen();
                      break;
         case KeyPress:
                       /*Get the keycode from the event and convert it to a keysym and pass to
                         the keyhandler*/
                     keyhandle(XKeycodeToKeysym(display, event.xkey.keycode, 0), True);
                     break;
         case KeyRelease:
                       /*Get the keycode from the event and convert it to a keysym and pass to
                         the keyhandler*/
                     keyhandle(XKeycodeToKeysym(display, event.xkey.keycode, 0), False);
                     break;
         default: printf("An unknown event happened... Where did THAT come from?!\n");
                  break;
         }
   goto mainloop; /* Branch back to main loop for next cycle */

/* Player Died */

playerdied:
#ifdef DEBUG
   printf("You have died!\n");
#endif
   updatescore();
   screenmsg("Game Over.", "");
   /*gameover();*/
   goto startgame;

/* End Of Program. Clean Up And Exit */

#ifdef DEBUG
   printf("\nClient exiting! Cleaning up first....");
#endif
   killme("Game over");
}
