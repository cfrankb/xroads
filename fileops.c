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

/********************************************************
 *** This File Contains Routines Used For File Access ***
 ********** (High score lists, map files, etc) **********
 ********************************************************/

#include "config.h"

   /* Prototypes */
#include "xroads.h"
#include "fileops.h"
#include "randommaze.h"

   /* Misc. */
#include <stdio.h>
#include <string.h>
#include <limits.h>

extern maze_t maze;
extern char xrmpath[80];

/************************************************************************/

void readmapfile( int mapnum ) {
   char mapfname[]="/map00.xrm"; /* Filename without path */
   char mapname[LINE_MAX+1];           /* Filename WITH path */
   FILE *fp;
   int i,j;
   char garbage;

   for(i=0; i<GRIDY; i++)
      for(j=0; j<GRIDX; j++)
         maze[i][j]=-2; /* Blank */

   if (mapnum<-1) {
      randommaze(-2-mapnum, &maze);
      return;
      }

   mapfname[4]=((mapnum/10)+'0'); /* Convert int to char */
   mapfname[5]=((mapnum%10)+'0'); /* Works for 0 to 99 */
   strcpy(mapname, xrmpath);

   if ((fp = fopen(strcat(mapname, mapfname), "r"))==NULL) {
      printf("--FileOps: Error opening file \'%s\'!\n", mapname);
      killme("Fatal error opening map file");
      }
#ifdef DEBUG
   printf("--FileOps: Successfully opened file \'%s\'.\n", mapname);
   printf("--FileOps: Reading file \'%s\'... ", mapname);
#endif

   for(i=0; i<GRIDY; i++) {
      for(j=0; j<((GRIDX)/2); j++) {
         if((char)fgetc(fp)=='*') maze[i][j]=-1;
         else maze[i][j]=-2;
         maze[i][GRIDX-1-j] = maze[i][j];
         }
      if((char)fgetc(fp)=='*') maze[i][GRIDX/2]=-1;
         else maze[i][GRIDX/2]=-2;
      garbage = (char)fgetc(fp);
      }
#ifdef DEBUG
   printf("Done.\n");

   printf("--FileOps: Closing \'%s\'... ", mapname);
#endif
   if(fclose(fp)!=0) {
      printf("Error closing file!\n");
      exit(1);
      }
#ifdef DEBUG
   printf("done!\n");
#endif
}
/*

(I can't remember what this was for, but I must have left it here for a
reason....)

             11111111112222222222333333333
   012345678901234567890123456789012345678
  0***************************************
  1
  2
  3
  4
  5
  6
  7
  8...

*/
