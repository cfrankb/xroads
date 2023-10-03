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

/**********************************************
 *** This file contains data for characters ***
 **********************************************/

#include "config.h"

   /* Misc. */
#include <stdio.h>
#include <stdlib.h>

extern creature data[NUMENTITIES+2];
extern tile_t tile[NUMCREATURES+1][2], bullet, arrow, explosion[4];
extern tile_t blank, worm[2], spar[3];

/*************************************************************************************************/
void setup( void ){                                                      /* Setup character data */
   /* Player 1 Data */
   data[PLAYER1].color=GREEN;
   data[PLAYER1].speed=3;
   data[PLAYER1].health=5;
   data[PLAYER1].attack1=BULLETS;
   data[PLAYER1].attack2=NONE;
   data[PLAYER1].atspeed=10;
   data[PLAYER1].atype=AUTO;
   data[PLAYER1].etype=CHARACTER;
   data[PLAYER1].diff=0;
   data[PLAYER1].value=2000;
   data[PLAYER1].game=3;
   data[PLAYER1].intel=0;
   data[PLAYER1].frames=2;
   data[PLAYER1].team=0;
   data[PLAYER1].tile[0]=tile[PLAYER1][0];
   data[PLAYER1].tile[1]=tile[PLAYER1][1];
   /* Player 2 Data */
   data[PLAYER2].color=RED;
   data[PLAYER2].speed=3;
   data[PLAYER2].health=5;
   data[PLAYER2].attack1=BULLETS;
   data[PLAYER2].attack2=NONE;
   data[PLAYER2].atspeed=10;
   data[PLAYER2].atype=AUTO;
   data[PLAYER2].etype=CHARACTER;
   data[PLAYER2].diff=0;
   data[PLAYER2].value=2000;
   data[PLAYER2].game=3;
   data[PLAYER2].intel=0;
   data[PLAYER2].frames=2;
   data[PLAYER2].team=0;
   data[PLAYER2].tile[0]=tile[PLAYER1][0];
   data[PLAYER2].tile[1]=tile[PLAYER1][1];
   /* Player 3 Data */
   data[PLAYER3].color=YELLOW;
   data[PLAYER3].speed=3;
   data[PLAYER3].health=5;
   data[PLAYER3].attack1=BULLETS;
   data[PLAYER3].attack2=NONE;
   data[PLAYER3].atspeed=10;
   data[PLAYER3].atype=AUTO;
   data[PLAYER3].etype=CHARACTER;
   data[PLAYER3].diff=0;
   data[PLAYER3].value=2000;
   data[PLAYER3].game=3;
   data[PLAYER3].intel=0;
   data[PLAYER3].frames=2;
   data[PLAYER3].team=0;
   data[PLAYER3].tile[0]=tile[PLAYER1][0];
   data[PLAYER3].tile[1]=tile[PLAYER1][1];
   /* Player 4 Data */
   data[PLAYER4].color=BLUE;
   data[PLAYER4].speed=3;
   data[PLAYER4].health=5;
   data[PLAYER4].attack1=BULLETS;
   data[PLAYER4].attack2=NONE;
   data[PLAYER4].atspeed=10;
   data[PLAYER4].atype=AUTO;
   data[PLAYER4].etype=CHARACTER;
   data[PLAYER4].diff=0;
   data[PLAYER4].value=2000;
   data[PLAYER4].game=3;
   data[PLAYER4].intel=0;
   data[PLAYER4].frames=2;
   data[PLAYER4].team=0;
   data[PLAYER4].tile[0]=tile[PLAYER1][0];
   data[PLAYER4].tile[1]=tile[PLAYER1][1];
   /* Dog */
   data[DOG].color=BROWN;
   data[DOG].speed=10;
   data[DOG].health=1;
   data[DOG].attack1=MELEE;
   data[DOG].attack2=NONE;
   data[DOG].atspeed=4;
   data[DOG].atype=AUTO;
   data[DOG].etype=CHARACTER;
   data[DOG].diff=0;
   data[DOG].value=100;
   data[DOG].game=2;
   data[DOG].intel=2;
   data[DOG].frames=2;
   data[DOG].range=4;
   data[DOG].team=0;
   data[DOG].tile[0]=tile[DOG][0];
   data[DOG].tile[1]=tile[DOG][1];
   /* Flea */
   data[FLEA].color=DBLUE;
   data[FLEA].speed=10; 
   data[FLEA].health=1;
   data[FLEA].attack1=NONE;  
   data[FLEA].attack2=NONE;
   data[FLEA].atspeed=4; 
   data[FLEA].atype=AUTO; 
   data[FLEA].etype=CHARACTER; 
   data[FLEA].diff=0;
   data[FLEA].value=100;
   data[FLEA].game=2;
   data[FLEA].intel=2;
   data[FLEA].frames=2;
   data[FLEA].range=1;
   data[FLEA].team=1;
   data[FLEA].tile[0]=tile[FLEA][0];
   data[FLEA].tile[1]=tile[FLEA][1];
   /* Old Flea */
   data[OLDFLEA].color=BLUE;     
   data[OLDFLEA].speed=10;
   data[OLDFLEA].health=1;
   data[OLDFLEA].attack1=NONE;
   data[OLDFLEA].attack2=NONE;
   data[OLDFLEA].atspeed=4;
   data[OLDFLEA].atype=AUTO;
   data[OLDFLEA].etype=CHARACTER;
   data[OLDFLEA].diff=0;
   data[OLDFLEA].value=100;
   data[OLDFLEA].game=1;
   data[OLDFLEA].intel=2;
   data[OLDFLEA].frames=2;
   data[OLDFLEA].range=1; /* Fleas are just stupid anyway */
   data[OLDFLEA].team=1;
   data[OLDFLEA].tile[0]=tile[OLDFLEA][0];
   data[OLDFLEA].tile[1]=tile[OLDFLEA][1];
   /* Mutant */
   data[MUTANT].color=PINK;     
   data[MUTANT].speed=16;
   data[MUTANT].health=1;
   data[MUTANT].attack1=BULLETS;
   data[MUTANT].attack2=NONE;
   data[MUTANT].atspeed=4;
   data[MUTANT].atype=AUTO;
   data[MUTANT].etype=CHARACTER;
   data[MUTANT].diff=2;
   data[MUTANT].value=200;
   data[MUTANT].game=2;
   data[MUTANT].intel=2;
   data[MUTANT].frames=2;
   data[MUTANT].range=3;
   data[MUTANT].team=2;
   data[MUTANT].tile[0]=tile[MUTANT][0];
   data[MUTANT].tile[1]=tile[MUTANT][1];
   /* Old Mutant */
   data[OLDMUTANT].color=RED;     
   data[OLDMUTANT].speed=16;
   data[OLDMUTANT].health=1;
   data[OLDMUTANT].attack1=BULLETS;
   data[OLDMUTANT].attack2=NONE;
   data[OLDMUTANT].atspeed=4;
   data[OLDMUTANT].atype=AUTO;
   data[OLDMUTANT].etype=CHARACTER;
   data[OLDMUTANT].diff=2;
   data[OLDMUTANT].value=200;
   data[OLDMUTANT].game=1;
   data[OLDMUTANT].intel=2;
   data[OLDMUTANT].frames=2;   
   data[OLDMUTANT].range=3;
   data[OLDMUTANT].team=2;
   data[OLDMUTANT].tile[0]=tile[OLDMUTANT][0];
   data[OLDMUTANT].tile[1]=tile[OLDMUTANT][1];
   /* Green Mashed Potato */
   data[GPOTATO].color=DGREEN;     
   data[GPOTATO].speed=16;
   data[GPOTATO].health=2;
   data[GPOTATO].attack1=MELEE;
   data[GPOTATO].attack2=NONE;
   data[GPOTATO].atspeed=4;
   data[GPOTATO].atype=AUTO;
   data[GPOTATO].etype=CHARACTER;
   data[GPOTATO].diff=1;
   data[GPOTATO].value=300;
   data[GPOTATO].game=1;
   data[GPOTATO].intel=2;
   data[GPOTATO].frames=2;   
   data[GPOTATO].range=3;
   data[GPOTATO].team=3;
   data[GPOTATO].tile[0]=tile[GPOTATO][0];
   data[GPOTATO].tile[1]=tile[GPOTATO][1];
   /* Cyan Mashed Potato */
   data[CPOTATO].color=CYAN;     
   data[CPOTATO].speed=16;
   data[CPOTATO].health=2;
   data[CPOTATO].attack1=MELEE;
   data[CPOTATO].attack2=NONE;
   data[CPOTATO].atspeed=4;
   data[CPOTATO].atype=AUTO;
   data[CPOTATO].etype=CHARACTER;
   data[CPOTATO].diff=1;
   data[CPOTATO].value=400;
   data[CPOTATO].game=1;
   data[CPOTATO].intel=2;
   data[CPOTATO].frames=2;   
   data[CPOTATO].range=3;
   data[CPOTATO].team=3;
   data[CPOTATO].tile[0]=tile[CPOTATO][0];
   data[CPOTATO].tile[1]=tile[CPOTATO][1];
   /* Blue Egghead */
   data[EGGHEAD].color=LBLUE;     
   data[EGGHEAD].speed=16;
   data[EGGHEAD].health=3;
   data[EGGHEAD].attack1=MELEE;
   data[EGGHEAD].attack2=NONE;
   data[EGGHEAD].atspeed=4;
   data[EGGHEAD].etype=CHARACTER;
   data[EGGHEAD].diff=3;
   data[EGGHEAD].value=300;
   data[EGGHEAD].game=2;
   data[EGGHEAD].intel=2;
   data[EGGHEAD].frames=2;
   data[EGGHEAD].range=3;
   data[EGGHEAD].team=4;
   data[EGGHEAD].tile[0]=tile[EGGHEAD][0];
   data[EGGHEAD].tile[1]=tile[EGGHEAD][1];
   /* Orange Lion */
   data[LION].color=ORANGE;
   data[LION].speed=14;
   data[LION].health=4;   
   data[LION].attack1=MELEE;
   data[LION].attack2=NONE;
   data[LION].atspeed=4;
   data[LION].atype=AUTO;
   data[LION].etype=CHARACTER;
   data[LION].diff=3;
   data[LION].value=400;
   data[LION].game=3;
   data[LION].intel=2;
   data[LION].frames=2;
   data[LION].range=3;
   data[LION].team=5;
   data[LION].tile[0]=tile[LION][0];
   data[LION].tile[1]=tile[LION][1];
   /* Red Chomper */
   data[CHOMPER].color=RED;
   data[CHOMPER].speed=12;
   data[CHOMPER].health=4;   
   data[CHOMPER].attack1=MELEE;
   data[CHOMPER].attack2=NONE;
   data[CHOMPER].atspeed=4;
   data[CHOMPER].atype=AUTO;
   data[CHOMPER].etype=CHARACTER;
   data[CHOMPER].diff=4;
   data[CHOMPER].value=500;
   data[CHOMPER].game=2;
   data[CHOMPER].intel=2;
   data[CHOMPER].frames=2;   
   data[CHOMPER].range=4;
   data[CHOMPER].team=4;
   data[CHOMPER].tile[0]=tile[CHOMPER][0];
   data[CHOMPER].tile[1]=tile[CHOMPER][1];
   /* Yellow Lemonshark */
   data[LEMONSHARK].color=YELLOW;
   data[LEMONSHARK].speed=10;
   data[LEMONSHARK].health=5;   
   data[LEMONSHARK].attack1=MELEE;
   data[LEMONSHARK].attack2=NONE;
   data[LEMONSHARK].atspeed=4;
   data[LEMONSHARK].atype=AUTO;
   data[LEMONSHARK].etype=CHARACTER;
   data[LEMONSHARK].diff=4;
   data[LEMONSHARK].value=600;
   data[LEMONSHARK].game=3;
   data[LEMONSHARK].intel=2;
   data[LEMONSHARK].frames=2;   
   data[LEMONSHARK].range=5;
   data[LEMONSHARK].team=4;
   data[LEMONSHARK].tile[0]=tile[LEMONSHARK][0];
   data[LEMONSHARK].tile[1]=tile[LEMONSHARK][1];
   /* Brown Monkey */
   data[MONKEY].color=BROWN;
   data[MONKEY].speed=10;
   data[MONKEY].health=5;   
   data[MONKEY].attack1=BULLETS;
   data[MONKEY].attack2=NONE;
   data[MONKEY].atspeed=4;
   data[MONKEY].atype=AUTO;
   data[MONKEY].etype=CHARACTER;
   data[MONKEY].diff=3;
   data[MONKEY].value=700;
   data[MONKEY].game=3;
   data[MONKEY].intel=2;
   data[MONKEY].frames=2;   
   data[MONKEY].range=5;
   data[MONKEY].team=5;
   data[MONKEY].tile[0]=tile[MONKEY][0];
   data[MONKEY].tile[1]=tile[MONKEY][1];
   /* Pink Tagteam */
   data[TAGTEAM].color=PINK;
   data[TAGTEAM].speed=14;
   data[TAGTEAM].health=3;   
   data[TAGTEAM].attack1=MELEE;
   data[TAGTEAM].attack2=NONE;
   data[TAGTEAM].atspeed=4;
   data[TAGTEAM].atype=AUTO;
   data[TAGTEAM].etype=CHARACTER;
   data[TAGTEAM].diff=4;
   data[TAGTEAM].value=500;
   data[TAGTEAM].game=2;
   data[TAGTEAM].intel=2;
   data[TAGTEAM].frames=2;
   data[TAGTEAM].range=4;
   data[TAGTEAM].team=2;
   data[TAGTEAM].tile[0]=tile[TAGTEAM][0];
   data[TAGTEAM].tile[1]=tile[TAGTEAM][1];
   /* Pink Tagteam + Mutant Combo */
   data[TAGTEAM2].color=PINK;
   data[TAGTEAM2].speed=12;
   data[TAGTEAM2].health=4;   
   data[TAGTEAM2].attack1=BULLETS;
   data[TAGTEAM2].attack2=MELEE;
   data[TAGTEAM2].atspeed=4;
   data[TAGTEAM2].atype=AUTO;
   data[TAGTEAM2].etype=CHARACTER;
   data[TAGTEAM2].diff=10;
   data[TAGTEAM2].value=700;
   data[TAGTEAM2].game=2;
   data[TAGTEAM2].intel=2;
   data[TAGTEAM2].frames=2;   
   data[TAGTEAM2].range=5;
   data[TAGTEAM2].team=2;
   data[TAGTEAM2].tile[0]=tile[TAGTEAM2][0];
   data[TAGTEAM2].tile[1]=tile[TAGTEAM2][1];
   /* Grey Thrower */
   data[THROWER].color=LGREY;
   data[THROWER].speed=14;
   data[THROWER].health=5;
   data[THROWER].attack1=ARROWS;
   data[THROWER].attack2=NONE;
   data[THROWER].atspeed=4;
   data[THROWER].atype=AUTO;
   data[THROWER].etype=CHARACTER;
   data[THROWER].diff=6;
   data[THROWER].value=600;
   data[THROWER].game=2;
   data[THROWER].intel=3;
   data[THROWER].frames=2;   
   data[THROWER].range=4;
   data[THROWER].team=6;
   data[THROWER].tile[0]=tile[THROWER][0];
   data[THROWER].tile[1]=tile[THROWER][1];
   /* Grey Archer */
   data[ARCHER].color=DGREY;
   data[ARCHER].speed=12;
   data[ARCHER].health=5;
   data[ARCHER].attack1=ARROWS;
   data[ARCHER].attack2=NONE;
   data[ARCHER].atspeed=4;
   data[ARCHER].atype=AUTO;   
   data[ARCHER].etype=CHARACTER;   
   data[ARCHER].diff=8;
   data[ARCHER].value=800;
   data[ARCHER].game=2;
   data[ARCHER].intel=3;
   data[ARCHER].frames=2;   
   data[ARCHER].range=5;
   data[ARCHER].team=6;
   data[ARCHER].tile[0]=tile[ARCHER][0];
   data[ARCHER].tile[1]=tile[ARCHER][1];
   /* Blue Rubberhead */
   data[BRUBBERHEAD].color=LBLUE;
   data[BRUBBERHEAD].speed=16;
   data[BRUBBERHEAD].health=3;
   data[BRUBBERHEAD].attack1=WORMS;
   data[BRUBBERHEAD].attack2=NONE;
   data[BRUBBERHEAD].atspeed=10;
   data[BRUBBERHEAD].atype=AUTO;   
   data[BRUBBERHEAD].etype=CHARACTER;   
   data[BRUBBERHEAD].diff=4;
   data[BRUBBERHEAD].value=700;
   data[BRUBBERHEAD].game=1;
   data[BRUBBERHEAD].intel=3;
   data[BRUBBERHEAD].frames=2;   
   data[BRUBBERHEAD].range=2;
   data[BRUBBERHEAD].team=1;
   data[BRUBBERHEAD].attr=RUBBER;
   data[BRUBBERHEAD].tile[0]=tile[BRUBBERHEAD][0];
   data[BRUBBERHEAD].tile[1]=tile[BRUBBERHEAD][1];
   /* Purple Rubberhead */
   data[PRUBBERHEAD].color=PURPLE;
   data[PRUBBERHEAD].speed=16;
   data[PRUBBERHEAD].health=4;
   data[PRUBBERHEAD].attack1=WORMS;
   data[PRUBBERHEAD].attack2=NONE;
   data[PRUBBERHEAD].atspeed=10;
   data[PRUBBERHEAD].atype=AUTO;   
   data[PRUBBERHEAD].etype=CHARACTER;   
   data[PRUBBERHEAD].diff=5;
   data[PRUBBERHEAD].value=800;
   data[PRUBBERHEAD].game=3;
   data[PRUBBERHEAD].intel=3;
   data[PRUBBERHEAD].frames=2;
   data[PRUBBERHEAD].range=2;
   data[PRUBBERHEAD].team=1;
   data[PRUBBERHEAD].attr=RUBBER;
   data[PRUBBERHEAD].tile[0]=tile[PRUBBERHEAD][0];
   data[PRUBBERHEAD].tile[1]=tile[PRUBBERHEAD][1];
   /* Green Rubberhead */
   data[GRUBBERHEAD].color=LGREEN;
   data[GRUBBERHEAD].speed=12;
   data[GRUBBERHEAD].health=4;
   data[GRUBBERHEAD].attack1=WORMS;
   data[GRUBBERHEAD].attack2=NONE;
   data[GRUBBERHEAD].atspeed=10;
   data[GRUBBERHEAD].atype=AUTO;   
   data[GRUBBERHEAD].etype=CHARACTER;   
   data[GRUBBERHEAD].diff=7;
   data[GRUBBERHEAD].value=1000;
   data[GRUBBERHEAD].game=2;
   data[GRUBBERHEAD].intel=3;
   data[GRUBBERHEAD].frames=2;   
   data[GRUBBERHEAD].range=3;
   data[GRUBBERHEAD].team=1;
   data[GRUBBERHEAD].attr=RUBBER;
   data[GRUBBERHEAD].tile[0]=tile[GRUBBERHEAD][0];
   data[GRUBBERHEAD].tile[1]=tile[GRUBBERHEAD][1];
   /* White Skull */
   data[SKULL].color=WHITE;
   data[SKULL].speed=12;
   data[SKULL].health=5;
   data[SKULL].attack1=MELEE;
   data[SKULL].attack2=NONE;
   data[SKULL].atspeed=4;
   data[SKULL].atype=AUTO;   
   data[SKULL].etype=CHARACTER;   
   data[SKULL].diff=7;
   data[SKULL].value=1000;
   data[SKULL].game=2;
   data[SKULL].intel=3;
   data[SKULL].frames=2;   
   data[SKULL].range=6;
   data[SKULL].team=3;
   data[SKULL].attr=SPEEDEAT;
   data[SKULL].tile[0]=tile[SKULL][0];
   data[SKULL].tile[1]=tile[SKULL][1];
   /* Green Vaccuum */
   data[VACCUUM].color=GREEN;
   data[VACCUUM].speed=14;
   data[VACCUUM].health=4;
   data[VACCUUM].attack1=VACCUUM;
   data[VACCUUM].attack2=MELEE;
   data[VACCUUM].atspeed=4;
   data[VACCUUM].atype=AUTO;   
   data[VACCUUM].etype=CHARACTER;   
   data[VACCUUM].diff=7;
   data[VACCUUM].value=800;
   data[VACCUUM].game=2;
   data[VACCUUM].intel=3;
   data[VACCUUM].frames=2;   
   data[VACCUUM].range=5;
   data[VACCUUM].team=7;
   data[VACCUUM].tile[0]=tile[VACCUUM][0];
   data[VACCUUM].tile[1]=tile[VACCUUM][1];
   /* Worm */
   data[WORM].color=PURPLE;
   data[WORM].speed=16;
   data[WORM].health=1;
   data[WORM].attack1=BULLETS;
   data[WORM].attack2=NONE;
   data[WORM].atspeed=4;
   data[WORM].atype=AUTO;
   data[WORM].etype=CHARACTER;
   data[WORM].diff=0;
   data[WORM].value=200;
   data[WORM].game=3;
   data[WORM].intel=2;
   data[WORM].frames=2;
   data[WORM].range=2;
   data[WORM].team=1;
   data[WORM].tile[0]=worm[0];
   data[WORM].tile[1]=worm[1];
   /* Wall */
   /* This color check shouldn't be necessary anymore, but oh well :) */
#if !defined MONO
   do {
      data[WALL].color=(int)rnd(NUMCOLORS-1)+1;
      if(data[WALL].color==BLACK) printf("--CharData: Dammit, I chose black as a wall color. Trying again...\n");
      } while (data[WALL].color==BLACK);
#else
   data[WALL].color=WHITE;
#endif
   data[WALL].speed=0;
   data[WALL].health=-11;
   data[WALL].diff=0;
   data[WALL].value=0;
   data[WALL].game=3;
   data[WALL].intel=0;
   data[WALL].frames=1;   
   /* Bullet */
   data[BULLET].color=GREEN;
   data[BULLET].speed=1;
   data[BULLET].health=1;
   data[BULLET].attack1=NONE;
   data[BULLET].attack2=NONE;
   data[BULLET].atspeed=0;
   data[BULLET].atype=AUTO;
   data[BULLET].etype=CHARACTER;
   data[BULLET].diff=0;
   data[BULLET].value=50;
   data[BULLET].game=3;
   data[BULLET].intel=1;
   data[BULLET].frames=1;
   data[BULLET].tile[0]=bullet;
   data[BULLET].tile[1]=bullet;
   /* Arrow */
   data[ARROW].color=GREEN;
   data[ARROW].speed=1;
   data[ARROW].health=1;
   data[ARROW].attack1=NONE;
   data[ARROW].attack2=NONE;
   data[ARROW].atspeed=0;
   data[ARROW].atype=AUTO;
   data[ARROW].etype=CHARACTER;
   data[ARROW].diff=0;
   data[ARROW].value=50;
   data[ARROW].game=2;
   data[ARROW].intel=1;
   data[ARROW].frames=1;
   data[ARROW].tile[0]=arrow;
   data[ARROW].tile[1]=arrow;
   /* Explosion */
   data[EXPLOSION].color=WHITE;
   data[EXPLOSION].speed=2;
   data[EXPLOSION].health=1;
   data[EXPLOSION].attack1=NONE;
   data[EXPLOSION].attack2=NONE;
   data[EXPLOSION].atspeed=0;
   data[EXPLOSION].atype=ONCE;
   data[EXPLOSION].etype=EFFECT;
   data[EXPLOSION].diff=0;
   data[EXPLOSION].value=0;
   data[EXPLOSION].game=3;   
   data[EXPLOSION].intel=0;   
   data[EXPLOSION].frames=4;
   data[EXPLOSION].tile[0]=explosion[0];
   data[EXPLOSION].tile[1]=explosion[1];
   data[EXPLOSION].tile[2]=explosion[2];
   data[EXPLOSION].tile[3]=explosion[3];
   /* Spar */
   data[SPAR].color=WHITE;
   data[SPAR].speed=2;
   data[SPAR].health=1;
   data[SPAR].attack1=NONE;
   data[SPAR].attack2=NONE;
   data[SPAR].atspeed=0;
   data[SPAR].atype=CONTINUOUS;
   data[SPAR].etype=POWERUP;
   data[SPAR].diff=0;
   data[SPAR].value=500;
   data[SPAR].game=3;
   data[SPAR].intel=0;
   data[SPAR].frames=3;
   data[SPAR].tile[0]=spar[0];
   data[SPAR].tile[1]=spar[1];
   data[SPAR].tile[2]=spar[2];
   /* Blue Spar (Speed up) */
   data[BLUESPAR].color=BLUE;
   data[BLUESPAR].speed=2;
   data[BLUESPAR].health=1;
   data[BLUESPAR].attack1=NONE;
   data[BLUESPAR].attack2=NONE;
   data[BLUESPAR].atspeed=0;
   data[BLUESPAR].atype=CONTINUOUS;
   data[BLUESPAR].etype=POWERUP;
   data[BLUESPAR].diff=0;
   data[BLUESPAR].value=500;
   data[BLUESPAR].game=3;
   data[BLUESPAR].intel=0;
   data[BLUESPAR].frames=3;
   data[BLUESPAR].tile[0]=spar[0];
   data[BLUESPAR].tile[1]=spar[1];
   data[BLUESPAR].tile[2]=spar[2];
   /* Red Spar (Machine gun) */
   data[REDSPAR].color=RED;
   data[REDSPAR].speed=2;
   data[REDSPAR].health=1;
   data[REDSPAR].attack1=NONE;
   data[REDSPAR].attack2=NONE;
   data[REDSPAR].atspeed=0;
   data[REDSPAR].atype=CONTINUOUS;
   data[REDSPAR].etype=POWERUP;
   data[REDSPAR].diff=0;
   data[REDSPAR].value=500;
   data[REDSPAR].game=3;
   data[REDSPAR].intel=0;
   data[REDSPAR].frames=3;
   data[REDSPAR].tile[0]=spar[0];
   data[REDSPAR].tile[1]=spar[1];
   data[REDSPAR].tile[2]=spar[2];

}
/*************************************************************************************************/
void setcols(void){                                                         /* Set object colors */
   int i;
   for(i=0; i<WALL; i++) data[i].color = WHITE;
   data[PLAYER1].color = GREEN;
   data[PLAYER2].color = RED;
   data[PLAYER3].color = YELLOW;
   data[PLAYER4].color = BLUE;
   data[DOG].color = BROWN;
   data[FLEA].color = DBLUE;
   data[OLDFLEA].color = BLUE;
   data[MUTANT].color = PINK;
   data[OLDMUTANT].color = RED;
   data[GPOTATO].color = DGREEN;
   data[CPOTATO].color = CYAN;
   data[EGGHEAD].color = LBLUE;
   data[LION].color = ORANGE;
   data[LEMONSHARK].color = YELLOW;
   data[TAGTEAM].color = PINK;
   data[TAGTEAM2].color = PINK;
   data[BRUBBERHEAD].color = LBLUE;
   data[PRUBBERHEAD].color = PURPLE;
   data[GRUBBERHEAD].color = LGREEN;
   data[THROWER].color = LGREY;
   data[ARCHER].color = DGREY;
   data[MONKEY].color = BROWN;
   data[CHOMPER].color = RED;
   data[SKULL].color = WHITE;
   data[VACCUUM].color = GREEN;
   data[BLANK].color = BLACK;
}  

