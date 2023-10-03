#ifndef XROADS_H
#define XROADS_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

/* xroads.c */
void randomize(void);
void fan_clear(Window, int, int);
void draw_tilex2(Window, tile_t, int, int, int, char);
int power(int, int);
void AllocColors( char[], int );
void drawscreen( void );
void drawwalls( void );
void setcols( void );
int checkdest( int, int );
void icorrectpos( int *, int * );
void correctpos( float *, float * );
void creatureturn( entity *, int );
void finddest( entity *);
void keyhandle ( KeySym, Bool );
void playerturn( entity * );
void getcols( void );
void drawsquare( int, int, int );
void initmap( int );
void watchentity( entity *, entity * );
void init( int, char *[]);
void showhelp( void );
void showkeys( void );
void clientmessage( XClientMessageEvent * );
int xytotype( int, int );
void killme( char * );
void xconnect( int, char *[] );
int ntrunc( float );
int xround( float );
#ifdef SIGHANDLE
void sigcatch( int );
#endif

#endif /* XROADS_H */
