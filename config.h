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

/********************************************************************
 * There's not much here to change. Don't change anything below the *
 ******* warning unless you REALLY know what you're doing. **********
 ********************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

/* Comment this out to disable the joystick code. This code currently only
   works on Linux, and only uses one joystick */

#define USEJOYSTICK

/* Don't enable the joystick by default. Otherwise, it defaults to
   controlling player 1. Uncomment this if your joystick doesn't work
   properly under XRoads, but you still want joystick support compiled in.
   (Same effect as using 'xroads -j 0') */

#define NOJOYDEFAULT

/* For those of you with crappy analog joysticks (like me), set this
   number high enough so that joystick "noise" won't get translated into
   motion (higher means less sensitive) */

#define JS_SENS 200

/* Uncomment this to use space to fire instead of Return (This was changed
   so that 2 player on one keyboard would be easier) */

/* #define OLDKEYS */

/* How many monsters to start off each maze with */

#define STARTMONSTERS 15

/* Spars needed to pass each level */

#define SPARSPERLEVEL 5

/* Amount to slow the game down by default. This value can be changed
   using command line parameters */

#define GAMEDELAY 10000

/* Path to XRM (XRoads Map) files. This can be changed using command
   line parameters too. Make sure to put a / on the end, or it won't
   work correctly (This is best defined in Makefile, here we just make
   sure it's actually set to something) */

#ifndef XRMPATH
#define XRMPATH "./"
#endif

/* Comment this out to use the P key to pause the game. If you used this
   key as one of your motion keys, then leave then uncomment it and use
   the "Pause" key on your keyboard to pause the game. In case you don't
   have a Pause key (I don't know maybe some people have weird keyboards),
   then make sure this is commented out or you won't be able to pause */

/*#define NOPTOPAUSE*/

/* Use either of the next two lines to change the size of the window.
   Halfsize makes everything smaller in case you're running on a smaller
   display, and doublesize makes everything bigger in case you've got an
   80" monitor or something. Leave them both commented out for the normal
   screen size */

/*#define HALFSIZE*/
/*#define DOUBLESIZE*/

/* Comment this out to disable the little explosions that show everytime
   something is shot or damaged */

#define XPLOSIONS

/* Uncomment this to remove spars from the game. It's a useless option,
   but you never know... someone might want it */

/*#define NOSPARS*/

/* This is supposed to make the game run in monochrome for those who don't
   have enough colors. It doesn't work, so don't use it (It used to work,
   but I don't really see much point in getting it working again */

/* #define MONO */

/* SIGHANDLE tells XRoads to exit nicely when recieving various
   termination signals. This way you don't lose keyrepeats when XRoads
   dies. If it won't compile for whatever reason, then comment it out */

#define SIGHANDLE

/* This will print out tons and tons of debugging information. Don't use
   it, even if you are working on the source code (I wrote the game and I
   don't even use it */

/*#define DEBUG*/

/* Game controls */
/* See /usr/X11R6/include/X11/keysymdef.h for key names */
/* I would prefer to use the right control/alt for fire/alt, but stupid
   window managers will steal them from you. P_ALT doesn't do anything
   yet, but will someday */

#define P1UP XK_Up
#define P1DOWN XK_Down
#define P1LEFT XK_Left
#define P1RIGHT XK_Right
#ifdef OLDKEYS
#define P1FIRE XK_space
#define P1ALT XK_Return
#else
#define P1FIRE XK_Return
#define P1ALT XK_backslash
#endif

#define P2UP XK_w
#define P2DOWN XK_s
#define P2LEFT XK_a
#define P2RIGHT XK_d
#define P2FIRE XK_Tab
#define P2ALT XK_Caps_Lock

#define P3UP XK_i
#define P3DOWN XK_k
#define P3LEFT XK_j
#define P3RIGHT XK_l
#define P3FIRE XK_space
#define P3ALT XK_n

#define P4UP XK_KP_Up
#define P4DOWN XK_KP_Begin /* Note: This is 5 on the keypad */
#define P4LEFT XK_KP_Left
#define P4RIGHT XK_KP_Right
#define P4FIRE XK_KP_Enter
#define P4ALT XK_KP_Add

#define RESTARTKEY XK_1
#define QUITKEY XK_Escape

/*******************************************************************/
/* No user-servicable parts! Most of this will screw up if changed */
/*******************************************************************/

/* A Few Useful Function-Macros */
#define delay(x)		{usleep(x*1000);}
#define rnd(x)				(rand()%(x))

/* Constants */
#ifndef GAMEDELAY
#define GAMEDELAY 1
#endif
#define GRIDX 39 /* Grid X and Y maxes. (Numbering starts at zero) */
#define GRIDY 23
#if defined HALFSIZE
#define GRIDSIZE 8
#define SCBOXSIZE 9
#elif defined DOUBLESIZE
#define GRIDSIZE 32
#define SCBOXSIZE 33
#else
#define GRIDSIZE 16
#define SCBOXSIZE 17
#endif
#define WIN_X (GRIDSIZE*GRIDX) /* 624 */
#define WIN_Y (GRIDSIZE*GRIDY) /* 480 */
#define NUMWALLS 20
#if !defined MONO
#define NUMCOLORS 17
#else
#define NUMCOLORS 1
#endif
#define MAXCREATURES 100
#define PIXSIZE (GRIDSIZE/8) /* Pixel size (for larger tiles) */
#define NUMMAPS 19

/* Directions */
#define LEFT 'a'
#define RIGHT 'd'
#define UP 'w'
#define DOWN 's'

/* Upside down versions of the above */
#define LEFT2 'h'
#define UP2 'k'
#define DOWN2 'j'
#define RIGHT2 'l'

/* Character Types */
#define PLAYER1 0
#define DOG 1
#define FLEA 2
#define OLDFLEA 3
#define MUTANT 4
#define OLDMUTANT 5
#define GPOTATO 6
#define CPOTATO 7
#define EGGHEAD 8
#define LION 9
#define CHOMPER 10
#define LEMONSHARK 11
#define MONKEY 12
#define TAGTEAM 13
#define TAGTEAM2 14
#define THROWER 15
#define ARCHER 16
#define BRUBBERHEAD 17
#define PRUBBERHEAD 18
#define GRUBBERHEAD 19
#define SKULL 20
#define VACCUUM 21
#define WORM 22
#define BLANK 23
#define SPAR 24
#define WALL 25
#define BULLET 26
#define ARROW 27
#define EXPLOSION 28
#define BLUESPAR 29
#define REDSPAR 30
#define PLAYER2 31  /* I put these at the end so that they don't mess up the */
#define PLAYER3 32  /* order of the main tiledata array, these numbers are   */
#define PLAYER4 33  /* likely to change if another monster/item is added     */

#define NUMCREATURES 22
#define NUMENTITIES 34

/* Colors by number */
#define BLACK 0
#define WHITE 1
#define PINK 2
#define RED 3
#define LGREEN 4
#define GREEN 5
#define DGREEN 6
#define LBLUE 7
#define BLUE 8
#define DBLUE 9
#define CYAN 10
#define YELLOW 11
#define LGREY 12
#define DGREY 13
#define PURPLE 14
#define ORANGE 15
#define BROWN 16

/* Attributes pertaining to entities */
#define ACTIVE		1   /* The monster is alive */
#define PULL_L		2   /* Being pulled to the left */
#define PULL_R		4   /* ... right */
#define PULL_U		8   /* ... up */
#define PULL_D		16  /* ... down */
#define SPEEDEAT	32  /* Gain speed from eating spars */
#define RUBBER		64  /* I'm rubber, and you're glue... */

enum attacks {NONE, MELEE, BULLETS, ARROWS, WORMS, VACCATTACK};
enum animtype {CONTINUOUS=1, ONCE, AUTO};
enum enttype {HUMAN, CHARACTER, EFFECT, POWERUP};

   /* Generic data for a creature */
typedef struct {
        int color;
        int speed;
        int health; /* Initial health */
        enum attacks attack1; /* Attack types */
        enum attacks attack2;
        int atspeed; /* Attack speed */
        int diff; /* difficulty */
        int frames; /* frames worth of animation */
        enum animtype atype; /* Animation type. See constants above */
        enum enttype etype; /* Entity type. See constants above */
        int value; /* Point value for scoring */
        int game; /* Game association. 1=Croads1, 2=Croads2, 3=Both, 4=New? */
        int intel; /* Intelligence (0 for bullets etc) */
        int range; /* Range for homing AI */
	int team; /* Monster alliance stuff */
        int attr; /* Default monster attributes */
	void *tile[5]; /* Pointers to the tile data for each character */
        } creature;

   /* Specific data for an active entity (Stuff that can change) */
typedef struct {
        float x, y; /* Coordinates */
        int frame; /* Current frame of animation */
        int type; /* What creature */
        char dir; /* Direction it's heading */
        int speed; /* Current speed (may change during game) */
        int health;
        long int score;
	int spars; /* Number of spars carried (only useful for humans) */
        int atspeed; 
        int status; /* Active, dead, invulnerable, etc, etc */
        int timeout; /* Powerup time left */
        int range; /* Range for homing AI */
	int color; /* This is turning into the same structure, isn't it? :P */
	int parent; /* Created by monst[parent], or NULL */
	int ind; /* The creature's index in monst[] */
	int hurt; /* Index of last character to attack this one */
	int target; /* The index of the creatures next target */
        int team; /* Allow monsters to change sides, pets, etc */
	long int attr; /* Attributes */
        } entity;  

   /* maze_t is a map, and tile_t is a character tile */
typedef int maze_t[GRIDY][GRIDX];
typedef unsigned char tile_t[8];

#endif
