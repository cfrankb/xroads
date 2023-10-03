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
 *** This file contains info for title screens and such ***
 **********************************************************/

#include "config.h"

   /* Protoypes */
#include "screens.h"
#include "xroads.h"
#include "xrtext.h"

   /* X */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

   /* Misc. */
#include <stdio.h>
#include <stdlib.h>

#if defined HALFSIZE
#define MULT 0.5
#elif defined DOUBLESIZE
#define MULT 2
#else
#define MULT 1
#endif

extern Window mainwindow;
extern Display *display;
extern GC gc;
extern int screen, active, numplayers;
extern XColor hwcol[17];
extern XFontStruct *normalfont, *bigfont;
extern tile_t tile[NUMCREATURES+1][2];
extern creature data[NUMENTITIES+2];
extern entity monst[MAXCREATURES];
extern Atom wm_delete_window, wm_protocols, protocols[1];
extern entity *player[5];

/*************************************************************************************************/
void XText( char text[], int x, int y ){                                /* Draw text. Easier way */
   int size;
   size = strlen(text);
   XDrawString(display, mainwindow, gc, x, y, text, size);
}

/*************************************************************************************************/
int IsEnter(XEvent *event) {                                       /* Check if Enter was pressed */
   char keyb_buffer[20], keypress;
   int length;
   KeySym keysym;
   XComposeStatus composestatus;
   if(event->type == KeyPress) {
      length=XLookupString(&event->xkey, keyb_buffer, 1, &keysym, &composestatus);
      keypress=keyb_buffer[length-1];
      if(keypress == 13) return 1;
      else return 0;
      }
   else return 0;
}

/*************************************************************************************************/
void screenmsg( char *line1, char *line2 ) {                   /* Display title screens and such */
   int size, x, y, i, j;
   char dirs[4] = { UP, DOWN, LEFT, RIGHT };
   auto int check[GRIDY][GRIDX];
   auto entity creatures[50];
   XEvent qevent;
#ifdef DEBUG
   printf("Displaying title screen\n");
#endif
   for(i=0; i<GRIDY; i++)
      for(j=0; j<GRIDX; j++)
         check[i][j]=0;
   for(i=0; i<50; i++) {
      creatures[i].type = (int)rnd(NUMCREATURES);
      creatures[i].color = data[creatures[i].type].color;
      creatures[i].frame = ((int)rnd(2));
      creatures[i].dir = dirs[(int)rnd(4)];
      do {
         creatures[i].x = (int)rnd(GRIDX);
         creatures[i].y = (int)rnd(GRIDY);
         } while(check[(int)creatures[i].y][(int)creatures[i].x]!=0);
      check[(int)creatures[i].y][(int)creatures[i].x]=1;
      }
   XSetFont(display, gc, bigfont->fid);
   XSetForeground(display, gc, hwcol[BLACK].pixel);
   XFillRectangle(display, mainwindow, gc, 0, 0, WIN_X, WIN_Y);
   do {
      for(i=0; i<50; i++)
         draw_tilex2(mainwindow, tile[creatures[i].type][creatures[i].frame], creatures[i].x*GRIDSIZE,
                     creatures[i].y*GRIDSIZE, creatures[i].color, creatures[i].dir);
      XSetForeground(display, gc, hwcol[WHITE].pixel);
      XSetBackground(display, gc, hwcol[BLACK].pixel);
      size = strlen(line1);
      x = (WIN_X/2) - (size * 14 * MULT);
      y = (WIN_Y/2);
      XText(line1, x, y);
      size = strlen(line2);
      x = (WIN_X/2) - (size * 11 * MULT);
      y = (WIN_Y/2) + (50 * MULT);
      XText(line2, x, y);
      XFlush(display);
      XRText("XRoads", 0, 0, WHITE);
      XRText2("XRoads", 0, GRIDSIZE, DGREY);
      XRTextUp("XRoads", 0, WIN_Y, WHITE);
      XRTextUp2("XRoads", GRIDSIZE, WIN_Y, DGREY);
      XRTextDown("XRoads", WIN_X-GRIDSIZE, 0, WHITE);
      XRTextDown2("XRoads", WIN_X-GRIDSIZE*2, 0, DGREY);
      XRTextLeft("XRoads", WIN_X-GRIDSIZE, WIN_Y-GRIDSIZE, DGREY);
      XRTextLeft2("XRoads", WIN_X-GRIDSIZE, WIN_Y, WHITE);
      do {
         XNextEvent(display, &qevent);
         } while(qevent.type==Expose && qevent.xexpose.count!=0);
      } while((qevent.type != ClientMessage) && (qevent.type != ButtonRelease) && !IsEnter(&qevent));
   if(qevent.type == ClientMessage)
      if(qevent.xclient.message_type == wm_protocols &&
         qevent.xclient.data.l[0] == wm_delete_window)
         killme("Window Manager killed me");
   XClearWindow(display, mainwindow);
#ifdef DEBUG
   printf("ScreenMsg finished.\n");
#endif
}

/*************************************************************************************************/
void screentxt( char *line1, char *line2 ) {                   /* Display title screens and such */
   int size, x, y, i;
   XEvent qevent;
#ifdef DEBUG
   printf("Displaying title screen\n");
#endif
   XSetFont(display, gc, bigfont->fid);
   do {
      drawscreen();
      updatescore();
      for(i=0; i<active; i++)
         if(monst[i].type!=-1 && (i>3 || i<numplayers))
            draw_tilex2(mainwindow, data[monst[i].type].tile[monst[i].frame],
            (monst[i].x)*GRIDSIZE, (monst[i].y)*GRIDSIZE, monst[i].color,
            monst[i].dir);
      XSetFont(display, gc, bigfont->fid);
      XSetForeground(display, gc, hwcol[WHITE].pixel);
      XSetBackground(display, gc, hwcol[BLACK].pixel);
      size = strlen(line1);
      x = (WIN_X/2) - (size * 14 * MULT);
      y = (WIN_Y/2);
      XText(line1, x, y);
      size = strlen(line2);
      x = (WIN_X/2) - (size * 11 * MULT);
      y = (WIN_Y/2) + (50 * MULT);
      XText(line2, x, y);
      XFlush(display);
      do {
         XNextEvent(display, &qevent);
         } while(qevent.type==Expose && qevent.xexpose.count!=0);
      } while((qevent.type != ClientMessage) && (qevent.type != KeyPress));
   if(qevent.type == ClientMessage)
      if(qevent.xclient.message_type == wm_protocols &&
         qevent.xclient.data.l[0] == wm_delete_window)
         killme("Window Manager killed me");
   XClearWindow(display, mainwindow);
#ifdef DEBUG
   printf("ScreenTxt finished.\n");
#endif
}

/*************************************************************************************************/
void updatescore( void ) {
   char scorep[4][80];
   int i;

   XSetForeground(display, gc, hwcol[BLACK].pixel);
   XFillRectangle(display, mainwindow, gc, 0, WIN_Y, WIN_X, SCBOXSIZE*numplayers);
   XSetBackground(display, gc, hwcol[BLACK].pixel);
   XSetForeground(display, gc, hwcol[WHITE].pixel);
   XSetFont(display, gc, normalfont->fid); 
   XFillRectangle(display, mainwindow, gc, 0, WIN_Y+1, WIN_X, 1);
      
   draw_tilex2(mainwindow, data[PLAYER1].tile[0], 0, WIN_Y+2,
               data[PLAYER1].color, RIGHT);
   draw_tilex2(mainwindow, data[PLAYER2].tile[0], 0, WIN_Y+SCBOXSIZE+2,
               data[PLAYER2].color, RIGHT);  
   draw_tilex2(mainwindow, data[PLAYER3].tile[0], 0, WIN_Y+(SCBOXSIZE*2)+2,
               data[PLAYER3].color, RIGHT);  
   draw_tilex2(mainwindow, data[PLAYER4].tile[0], 0, WIN_Y+(SCBOXSIZE*3)+2,
               data[PLAYER4].color, RIGHT);

   XSetBackground(display, gc, hwcol[BLACK].pixel);
   XSetForeground(display, gc, hwcol[WHITE].pixel);
   
   for(i=1; i<=numplayers; i++) {
      sprintf(scorep[i-1], ": %ld points, %d health, %d more to go...",
        player[i]->score, player[i]->health, SPARSPERLEVEL-player[i]->spars);
/*
      XText(scorep[i-1], GRIDSIZE, WIN_Y+(SCBOXSIZE*i)-2);
*/
      XRText(scorep[i-1], GRIDSIZE, WIN_Y+(SCBOXSIZE*(i-1))+2, WHITE);
      }
   
   XFlush(display);
}
