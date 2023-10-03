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

/**********************************************************
 *** This file contains routines for character movement ***
 *** attack types, and other such creature information  ***
 **********************************************************/

#include "config.h"

   /* Prototypes */
#include "ai.h"
#include "xroads.h"
#include "screens.h"

   /* X */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

   /* Misc. */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern tile_t tile[NUMCREATURES+1][2], blank, bullet, arrow;
extern creature data[NUMENTITIES+2];
extern entity *player[5];
extern entity monst[MAXCREATURES];
extern int active, numplayers, level;
extern double turn;
extern int maze[GRIDY][GRIDX];
extern Window mainwindow;
extern char directions[];

/*************************************************************************************************/
void destrand( entity *amonst ) {                            /* Find a place to move. (Creatures) */
   float x, y;
   int eqx, eqy;
   char dir;
   int tries=0;

      /* If in a halfstep, finish it */
   if(ntrunc(amonst->x)!=xround(amonst->x)) switch(amonst->dir) {
      case LEFT: amonst->x-=0.5; return;
      case RIGHT: amonst->x+=0.5; return;
      default: amonst->dir = directions[(int)rnd(2) + 2]; break;
      }
   if(ntrunc(amonst->y)!=xround(amonst->y)) switch(amonst->dir) {
      case UP: amonst->y-=0.5; return;
      case DOWN: amonst->y+=0.5; return;
      default: amonst->dir = directions[(int)rnd(2)]; break;
      }

      /* Come back here to try again */
   tryagain:
   tries++;
   if(tries>100) return; /* Give up after 100 tries */
   x = amonst->x;
   y = amonst->y;

      /* Choose a direction */
   if(rnd(10)>3) dir=amonst->dir;
   else dir = directions[rnd(4)];

      /* Find coordinates for new position */
   switch(dir) {
      case UP: y-=0.5; eqy=ntrunc(y); eqx=ntrunc(x); break;
      case DOWN: y+=0.5; eqy=xround(y); eqx=ntrunc(x); break;
      case LEFT: x-=0.5; eqy=ntrunc(y); eqx=ntrunc(x); break;
      case RIGHT: x+=0.5; eqy=ntrunc(y); eqx=xround(x); break;
      default: printf("Error: Not a direction\n"); return;
      }

      /* Check if that direction works */
   if(!checkdest(eqx, eqy) && data[xytotype(eqx, eqy)].etype!=POWERUP)
      goto tryagain;

      /* Check for double half-step */
   if(ntrunc(x)!=xround(x) && ntrunc(y)!=xround(y)) goto tryagain;

      /* Did we hit a powerup? */
   if(data[xytotype(eqx, eqy)].etype == POWERUP) {
       monst[maze[eqy][eqx]].health = 0;
       killmonster(&monst[maze[eqy][eqx]]);
       amonst->health++;
       if(amonst->attr & SPEEDEAT)
          amonst->speed = amonst->speed<6 ? 1 : amonst->speed-5;
       }

      /* Passes all checks */
   amonst->x = x;
   amonst->y = y;
   amonst->dir = dir;
}

/*************************************************************************************************/
void desthome( entity *amonst ) {                              /* Home in on target's location */
   float x=amonst->x, y=amonst->y;
   int eqx, eqy, tries=0;
   char dir;
   entity *target=&monst[amonst->target];
   float dx, dy, dz;

      /* If in a halfstep, finish it */
   if(ntrunc(amonst->x)!=xround(amonst->x)) switch(amonst->dir) {
      case LEFT: amonst->x-=0.5; return;
      case RIGHT: amonst->x+=0.5; return;
      default: amonst->dir = directions[(int)rnd(2) + 2]; break;
      }
   if(ntrunc(amonst->y)!=xround(amonst->y)) switch(amonst->dir) {
      case UP: amonst->y-=0.5; return;
      case DOWN: amonst->y+=0.5; return;
      default: amonst->dir = directions[(int)rnd(2)]; break;
      }

      /* Find distance via pythagorean theroem */
   dx = abs(x - target->x);
   dy = abs(y - target->y);
   dz = sqrt(dx*dx+dy*dy);

      /* If the target is out of range, then wander */
   if(dz > amonst->range) {
      destrand(amonst);
      return;
      }

   tryagain: /* Come back here to try again */
   tries++;
   if(tries>100) {
      if(ntrunc(amonst->x)==xround(amonst->x) &&
         ntrunc(amonst->y)==xround(amonst->y))
         watchentity(amonst, target);
      return;
      }
   x = amonst->x;
   y = amonst->y;

      /* Flip a coin and choose wether to move x or y */
   switch((int)rnd(2)) {
      case 0: /* X */
         if(x == target->x) goto tryagain;
         dir = (target->x > x) ? RIGHT : LEFT; break;
      case 1: /* Y */
         if(y == target->y) goto tryagain;
         dir = (target->y > y) ? DOWN : UP; break;
      default: printf("Error, bad random number\n"); return;
      }

      /* Find the new position */
   switch(dir) {
      case UP: y = amonst->y - 0.5; break;
      case DOWN: y = amonst->y + 0.5; break;
      case LEFT: x = amonst->x - 0.5; break;
      case RIGHT: x = amonst->x + 0.5; break;
      default: printf("Error, bad direction\n"); return;
      }

      /* Check new position */
   switch(dir) {
      case UP: eqy = y - 0.5; eqx = x; break;
      case DOWN: eqy = y + 0.5; eqx = x; break;
      case LEFT: eqx = x - 0.5; eqy = y; break;
      case RIGHT: eqx = x + 0.5; eqy = y; break;
      default: printf("Error, bad direction\n"); return;
      }
   if(!checkdest(eqx, eqy) && data[xytotype(eqx, eqy)].etype!=POWERUP)
      goto tryagain;

      /* Be careful of half-steps */
   if(ntrunc(x)!=xround(x) && ntrunc(y)!=xround(y)) goto tryagain;

      /* Did we hit a powerup? */
   if(data[xytotype(eqx, eqy)].etype == POWERUP) {
       monst[maze[eqy][eqx]].health = 0;
       killmonster(&monst[maze[eqy][eqx]]);
       amonst->health++;
       if(amonst->attr & SPEEDEAT)
          amonst->speed = amonst->speed<6 ? 1 : amonst->speed-5;
       }

      /* Done working out new position, assign to character */
   amonst->x = x;
   amonst->y = y;
   amonst->dir = dir;
}

/*************************************************************************************************/
void deststraight( entity *actmonst ) {                               /* move straight (bullets) */
   /* Projectiles don't use halfsteps */
   int x=actmonst->x, y=actmonst->y, temp=0;

   switch(actmonst->dir) {
      case UP: y-=1; break;
      case DOWN: y+=1; break;
      case LEFT: x-=1; break;
      case RIGHT: x+=1; break;
      default: break;
      }
   icorrectpos(&x, &y);
   if(!checkdest(x, y) && data[xytotype(x, y)].etype==CHARACTER) {
      temp = monst[maze[y][x]].dir + actmonst->dir;
      /* Check if monsters are facing opposite directions */
      if(monst[maze[y][x]].attr & RUBBER &&
         !(temp==('w'+'s') || temp==('a'+'d')))
         switch(actmonst->dir) {
            case UP: actmonst->dir = DOWN; break;
            case DOWN: actmonst->dir = UP; break;
            case LEFT: actmonst->dir = RIGHT; break;
            case RIGHT: actmonst->dir = LEFT; break;
            }
      else {
         actmonst->health=0; /* Destroy on contact */
         monst[maze[y][x]].health-=1;
            /* Don't let the monster get mad at itself */
         if(actmonst->parent != -1 && actmonst->parent != monst[maze[y][x]].ind)
               monst[maze[y][x]].hurt = actmonst->parent;
         else
            monst[maze[y][x]].hurt = actmonst->ind;
         return;
         }
      }
   else if(!checkdest(x, y) && data[xytotype(x, y)].etype==POWERUP)
      actmonst->health=0; /* Powerups block shots */
   else {actmonst->x=x; actmonst->y=y;}
}

/*************************************************************************************************/
int makemonst( int mtype, char dir, int x, int y, int color, int parent) {
                                               /* Add a new monster */
   int i=4;

   while (mtype==PLAYER1 || mtype==PLAYER2 || mtype==PLAYER3 ||
          mtype==PLAYER4 || mtype==-1)
      mtype = (int)rnd(NUMCREATURES-2)+1;

   while(data[mtype].diff > level)
      mtype = (int)rnd(NUMCREATURES-2)+1;

   if(mtype==SPAR) {
      if(rnd(30)==8)
         mtype=BLUESPAR;
      else if(rnd(20)==8)
         mtype=REDSPAR;
      }
   while((monst[i].type!=-1)&&(i<=active)) i++;
   if(i>=MAXCREATURES) {
      printf("--MakeMonst: Creature stack overrun! Fatal Error!\n");
      exit(1);
      }
   monst[i].type=mtype;
   if((x==-1)&&(y==-1)) /* Random placement */
      do {
         x=(int)rnd(GRIDX);
         y=(int)rnd(GRIDY);
         } while(!checkdest(x,y));
   if(color==-1) color=data[mtype].color;
   /* Set up the creature's starting values */
   monst[i].x=x;
   monst[i].y=y;
   monst[i].frame=0;
   monst[i].dir=dir;
   monst[i].atspeed=0;
   monst[i].health=data[monst[i].type].health;
   monst[i].speed=data[monst[i].type].speed;
   monst[i].status=0;  /* Also unimplemented */
   monst[i].timeout=0; /* Guess what? */
   monst[i].range=data[monst[i].type].range;
   monst[i].color=color;
   /* Keep track of this creature's origin for scoring purposes */
   monst[i].parent=parent;
   monst[i].ind=i;
   monst[i].hurt=-1;
   monst[i].team=data[mtype].team;
   monst[i].attr=data[mtype].attr | ACTIVE;
   /* Attack parents enemies, mostly for rubberheads */
   if(parent != -1)
      monst[i].target = monst[parent].target;
   else
      monst[i].target=rnd(numplayers); /* Defaults to a random player */
#ifdef DEBUG
   printf("Monster Spawned! ( @ %d, %d... id: %d, type: %d)\n", x, y, i, mtype);
#endif
   if(monst[i].type!=EXPLOSION) /* Explosions shouldn't block anything */
      maze[(int)monst[i].y][(int)monst[i].x]=i; /* Hold entity id in array */
   draw_tilex2(mainwindow, data[mtype].tile[0], x*GRIDSIZE, y*GRIDSIZE, color, dir);
   if(i>=active) active+=2;
   if(active>MAXCREATURES) {
      printf("Oh crap! I made too many creatures! Better exit now before the segfault.\n");
      exit(1);
      }
   return i; /* The index of the monster that was created */
}

/*************************************************************************************************/
void killmonster( entity *amonst ) {                /* Remove a monster from the game */
   entity *bmonst;
   int mtype = amonst->type;
   if((amonst->type != -1) && (amonst->health > 0))
      printf("I was asked to kill a monster that shouldn't be killed!\n");
      if(amonst->health <= 0) {
         if(data[amonst->type].etype != EFFECT) {
            maze[xround(amonst->y)][xround(amonst->x)] = -2; /* Blank */
            maze[ntrunc(amonst->y)][ntrunc(amonst->x)] = -2; /* Blank */
            }
         draw_tilex2(mainwindow, data[amonst->type].tile[amonst->frame],
                     amonst->x*GRIDSIZE, amonst->y*GRIDSIZE, BLACK,
                     amonst->dir);
         }
         /* If there was something underneath the effect, redraw it */
      if(maze[ntrunc(amonst->y)][ntrunc(amonst->x)]!=-2) {
         bmonst=&monst[maze[ntrunc(amonst->y)][ntrunc(amonst->x)]];
         draw_tilex2(mainwindow, data[bmonst->type].tile[bmonst->frame],
                     bmonst->x*GRIDSIZE, bmonst->y*GRIDSIZE,
                     bmonst->color, bmonst->dir);
         }
      if(maze[xround(amonst->y)][xround(amonst->x)]!=-2) {
         bmonst=&monst[maze[xround(amonst->y)][xround(amonst->x)]];
         draw_tilex2(mainwindow, data[bmonst->type].tile[bmonst->frame],
                     bmonst->x*GRIDSIZE, bmonst->y*GRIDSIZE,
                     bmonst->color, bmonst->dir);
         }
   amonst->health = 0;
#ifdef XPLOSIONS
   if(amonst->type != EXPLOSION && data[amonst->type].etype != POWERUP)
      makemonst(EXPLOSION, directions[rnd(4)], amonst->x, amonst->y,
amonst->color, -1);
#endif
/*
   if(data[amonst->type].etype == CHARACTER && amonst->type != BULLET
      && amonst->type != ARROW)
      monst[amonst->hurt].score+=100;
   else if(data[amonst->type].etype == POWERUP)
      monst[amonst->hurt].score+=200;
*/
   if(amonst->hurt >= 0)
      monst[amonst->hurt].score += data[mtype].value;
   updatescore();
   if(data[amonst->type].etype == POWERUP)
      makemonst(SPAR, RIGHT, -1, -1, -1, -1);
   /* Don't respawn a creature if the dying creature was a player/projectile/worm/effect/etc */
   else if((amonst->type < NUMCREATURES) && (amonst->type >= 0))
      makemonst((int)rnd(NUMCREATURES-1)+1, RIGHT, -1, -1, -1, -1);
   amonst->type = -1;
   amonst->color = BLACK;
#ifdef DEBUG
   printf("Monster killed.\n");
#endif
}

/*************************************************************************************************/
void biteattack( entity *amonst, entity *bmonst ) {                 /* Bite/kick/punch/flail/etc */
   if(abs(amonst->x-bmonst->x) > 1 || abs(amonst->y-bmonst->y) > 1) {
      printf("--BiteAttack: Character too far away to try and bite\n");
      return;
      }
   bmonst->health--;
   bmonst->hurt = amonst->ind;
   makemonst(EXPLOSION, directions[rnd(4)], bmonst->x, bmonst->y, bmonst->color, -1);
}

/*************************************************************************************************/
void spawnbullet( entity *amonst ) {                                 /* Fire a bullet/worm/arrow */
   float fx=amonst->x, fy=amonst->y;
   int x, y;
   int bullettype;
   if(data[amonst->type].attack1==ARROWS) bullettype = ARROW;
   else if(data[amonst->type].attack1==WORMS) bullettype = WORM;
   else bullettype = BULLET;
#ifdef DEBUG
   printf("Spawn bullet called from character @ (%f, %f) (%c)\n", fx, fy, amonst->dir);
#endif
   if(amonst->atspeed == 0) {
      amonst->atspeed = data[amonst->type].atspeed;
      switch(amonst->dir) {
         case LEFT: fx-=1; x=ntrunc(fx); y=ntrunc(fy); break;
         case RIGHT: fx+=1; x=xround(fx); y=ntrunc(fy); break;
         case UP: fy-=1; y=ntrunc(fy); x=ntrunc(fx); break;
         case DOWN: fy+=1; y=xround(fy); x=ntrunc(fx); break;
         }
      icorrectpos(&x, &y);
      if(!checkdest(x, y)) {
         if(maze[y][x]!=-1 && data[xytotype(x, y)].etype!=POWERUP)
            makemonst(EXPLOSION, directions[rnd(4)], x, y, amonst->color, -1);
         if(data[xytotype(x, y)].etype==CHARACTER) {
            monst[maze[y][x]].health-=1;
            monst[maze[y][x]].hurt = amonst->ind;
            }
         return;
         }
      else {
#ifdef DEBUG
         printf("Bullet Spawned @ (%d, %d)\n", x, y);
#endif
         makemonst(bullettype, amonst->dir, x, y, amonst->color, amonst->ind);
         }
      }
#ifdef DEBUG
   else printf("Can\'t shoot yet!\n");
#endif
}

/*************************************************************************************************/
void vacattack( entity *amonst, entity *target) {
#ifdef DEBUG
   printf("Sucking #%d\n", target->ind);
#endif
   if(abs(target->x - amonst->x) + abs(target->y - amonst->y) <= 1) {
      target->health = 0;
#ifdef DEBUG
      printf("Sucked!\n");
#endif
      return;
      }
   if(target->x > amonst->x)
      target->attr |= PULL_L;
   else if(target->x < amonst->x)
      target->attr |= PULL_R;
   else if(target->y > amonst->y)
      target->attr |= PULL_U;
   else if(target->y < amonst->y)
      target->attr |= PULL_D;

      /* Though it doesn't do any damage, it should anger monsters */
   target->hurt = amonst->ind;
}

/*************************************************************************************************/
int clearshot( entity *amonst, entity *target) {
      /* Check if amonst has a clear shot at target */
   int x1 = (int) amonst->x,
       y1 = (int) amonst->y,
       x2 = (int) target->x,
       y2 = (int) target->y;
   int i;

      /* If they're touching, then there's a clear shot */
   if(abs(target->x - amonst->x) + abs(target->y - amonst->y) <= 1)
      return 1;

      /* Not lined up */
   if(!(amonst->x == target->x || amonst->y == target->y)) return 0;

      /* Halfsteps are banned */
   if(xround(amonst->x) != ntrunc(amonst->x)) return 0;
   if(xround(amonst->y) != ntrunc(amonst->y)) return 0;

      /* Force x1, y1 to be smaller pair */
   if(x1 > x2) {
      x1 = (int) target->x;
      x2 = (int) amonst->x;
      }
   if(y1 > y2) {
      y1 = (int) target->y;
      y2 = (int) amonst->y;
      }

      /* Check for walls */
   if(y1 == y2) {
      for(i = x1; i < x2; i++)
         if(maze[y1][i] == -1) return 0;
      }
   else if(x1 == x2) {
      for(i = y1; i < y2; i++)
         if(maze[i][x1] == -1) return 0;
      }

      /* If we haven't returned yet, there's a clear path */
   return 1;

}

/*************************************************************************************************/
void findtarg( entity *amonst, int noteam ) {

/* Use the noteam parameter to tell a monster if it is okay to target a
teammate. 'True' means to not allow it */

   int i, temp=amonst->target; /* Default to a player */
   float tx, ty, tz, t2z = 100;
   float ax = amonst->x, ay = amonst->y;

   for(i=0; i<MAXCREATURES; i++) {
         /* Disqualify a target */
      if(monst[i].type == -1) continue;
      if(monst[i].ind == amonst->ind) continue;
      if(clearshot(amonst, &monst[i]) == 0) continue;
      if(monst[i].health<=0) continue;
      if(data[monst[i].type].etype == EFFECT) continue;
      if((amonst->attr & ACTIVE) == 0) continue;
      if(noteam && amonst->team == monst[i].team) continue;

      tx = monst[i].x;
      ty = monst[i].y;

         /* Else, treat as a potential target */
      tz = sqrt((ax-tx)*(ax-tx)+(ay-ty)*(ay-ty));
      if(tz < t2z) {
         t2z = tz;
         temp = i;
         }
      }

   amonst->target = temp;
}
