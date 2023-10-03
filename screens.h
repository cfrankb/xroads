#ifndef SCREENS_H
#define SCREENS_H

#include <X11/Xlib.h>

/* screens.c */
void titlescreen( void );
void XText( char [], int, int );
int IsEnter( XEvent * );
void screenmsg( char *, char *);
void screentxt( char *, char *);
void pausegame( void );
void updatescore( void );

#endif /* SCREENS_H */
