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

/******************************************************************/
/* Here is the code for the random maze generator by Michele Bini */
/******************** (Great work Michele!) ***********************/

#include "config.h"
#include <stdio.h>
#include <stdlib.h>

static int
wrap(int x, int max)
{
  int r=x;
 redo:
  if (r < 0) {
    r += max;
    goto redo;
  } else while (r >= max) {
     r-=max;
  }
  return r;
}

static void
maze_set(maze_t *maze, int x, int y, char c)
{
  x = wrap(x, GRIDX);
  y = wrap(y, GRIDY);
  (*maze)[y][x] = (c?-1:-2);
}

static char
maze_get(maze_t *maze, int x, int y)
{
  x = wrap(x, GRIDX);
  y = wrap(y, GRIDY);
  return (*maze)[y][x]==-1;
}

static int
maze_free(maze_t *maze, int x, int y)
{
  if (maze_get(maze, x, y)) return 0;
  {
    int up, down, left, right, ul, ur, dl, dr, sum;
    up=!!maze_get(maze, x, y-1);
    down=!!maze_get(maze, x, y+1);
    left=!!maze_get(maze, x-1, y);
    right=!!maze_get(maze, x+1, y);
    ul=!!maze_get(maze, x-1, y-1);
    dl=!!maze_get(maze, x-1, y+1);
    ur=!!maze_get(maze, x+1, y-1);
    dr=!!maze_get(maze, x+1, y+1);
    if (left+right+up+down>1) return 0;
    sum=ul+dl+ur+dr;
    if (sum==0) {
	    return 1;
    }
    if (sum>1) return 0;
    if ((ul) && (!(up || left))) return 0;
    if ((ur) && (!(up || right))) return 0;
    if ((dl) && (!(down || left))) return 0;
    if ((dr) && (!(down || right))) return 0;
    return 
      (!((up && (dl || dr)) ||
	 (down && (ul || ur)) ||
	 (left && (dr || ur)) ||
	 (right && (dl || ul))));
  }
}

static int maze_growable(maze_t*maze, int x, int y) {
  if (maze_get(maze, x, y)) return 0;
  {
    int up, down, left, right, ul, ur, dl, dr, sum;
    up=!!maze_get(maze, x, y-1);
    down=!!maze_get(maze, x, y+1);
    left=!!maze_get(maze, x-1, y);
    right=!!maze_get(maze, x+1, y);
    ul=!!maze_get(maze, x-1, y-1);
    dl=!!maze_get(maze, x-1, y+1);
    ur=!!maze_get(maze, x+1, y-1);
    dr=!!maze_get(maze, x+1, y+1);
    if (left+right+up+down!=1) return 0;
    sum=ul+dl+ur+dr;
    if (sum==0) {
	    return 1;
    }
    if (sum>1) return 0;
    if ((ul) && (!(up || left))) return 0;
    if ((ur) && (!(up || right))) return 0;
    if ((dl) && (!(down || left))) return 0;
    if ((dr) && (!(down || right))) return 0;
    return 
      (!((up && (dl || dr)) ||
	 (down && (ul || ur)) ||
	 (left && (dr || ur)) ||
	 (right && (dl || ul))));
  }
}

static int maze_growable2(maze_t*maze, int x, int y) {
  if (maze_get(maze, x, y)) return 0;
  {
    int up, down, left, right, ul, ur, dl, dr, sum;
    up=!!maze_get(maze, x, y-1);
    down=!!maze_get(maze, x, y+1);
    left=!!maze_get(maze, x-1, y);
    right=!!maze_get(maze, x+1, y);
    ul=!!maze_get(maze, x-1, y-1);
    dl=!!maze_get(maze, x-1, y+1);
    ur=!!maze_get(maze, x+1, y-1);
    dr=!!maze_get(maze, x+1, y+1);
    if (left+right+up+down!=1) return 0;
    sum=ul+dl+ur+dr;
    if (sum==0) {
	    return 1;
    }
    if (sum>2) return 0;
    if ((ul) && (!(up || left))) return 0;
    if ((ur) && (!(up || right))) return 0;
    if ((dl) && (!(down || left))) return 0;
    if ((dr) && (!(down || right))) return 0;
    return 
      (!((up && (dl || dr)) ||
	 (down && (ul || ur)) ||
	 (left && (dr || ur)) ||
	 (right && (dl || ul))));
  }
}

static void maze_grow(maze_t*, int, int, int);

static void
maze_trace_path(maze_t *maze, int sx, int sy, int depth)
{
  if (maze_growable2(maze, sx, sy)) {
	  maze_grow(maze, sx, sy, depth);
  }
}

static void
maze_grow(maze_t *maze, int sx, int sy, int depth)
{
  int c, oldx, oldy, dir;
  maze_set(maze, sx, sy, 1);
  c=4;
  oldx=sx; oldy=sy; 
  while (depth>0) {
    dir=rand()%4;
    sx=oldx; sy=oldy;
    switch(dir) {
      case 0:
	sx += 1;
	break;
      case 1:
	sx -= 1;
	break;
      case 2:
	sy += 1;
	break;
      case 3:
	sy -= 1;
	break;
    }
    maze_trace_path(maze, sx, sy, depth);
    depth--;
  }
}



static void drawborder(maze_t *maze, int density) {
  int c;
  for (c=0; c<GRIDX; c++) {
    if ((rand()%100)<density) {
      maze_set(maze, c, 0, 1);
      maze_set(maze, c, GRIDY-1, 1);
    }
  }
  for (c=1; c<GRIDX-1; c++) {
    if ((rand()%100)<density) {
      maze_set(maze, 0, c, 1);
      maze_set(maze, GRIDX-1, c, 1);
    }
  }
}

static void complexmaze(maze_t *maze, int complexity) {
  maze_grow(maze, rand()%GRIDX, rand()%GRIDY, complexity);
}

static void reasonablemaze(maze_t *maze) {
  int n=4+rand()%4, i;
  for (i=0; i<n; i++) {
    int x=rand()%GRIDX;
    int y=rand()%GRIDY;
    if (maze_free(maze, x, y))
      maze_grow(maze, x, y, 500/n);
  }
}

static void grainymaze(maze_t *maze, int density) {
  int n=density*(GRIDX*GRIDY)/100, i;
  for (i=0; i<n; i++) {
    int x=rand()%GRIDX;
    int y=rand()%GRIDY;
    if (maze_free(maze, x, y))
      maze_set(maze, x, y, 1);
  }
}

static void growmaze(maze_t *maze, int density) {
  int n=density*(GRIDX*GRIDY)/100, i;
  for (i=0; i<n; i++) {
    int x=rand()%GRIDX;
    int y=rand()%GRIDY;
    if (maze_growable(maze, x, y))
      maze_set(maze, x, y, 1);
  }
}

static void simmetricalmaze(maze_t *maze, int granulosity, int density, int x_simm, int y_simm) {
  int n=granulosity*(GRIDX*GRIDY)/2/100, i;
  for (i=0; i<n; i++) {
    int x=rand()%GRIDX;
    int y=rand()%GRIDY;
    int x2=x_simm?(GRIDX-1-x):x;
    int y2=y_simm?(GRIDY-1-y):y;
    if (maze_free(maze, x, y)) {
      maze_set(maze, x, y, 1);
      if (maze_free(maze, x2, y2)) maze_set(maze, x2, y2, 1);
    }
  }
  n=density*(GRIDX*GRIDY)/100;
  for (i=0; i<n; i++) {
    int x=rand()%GRIDX;
    int y=rand()%GRIDY;
    int x2=x_simm?(GRIDX-1-x):x;
    int y2=y_simm?(GRIDY-1-y):y;
    if (maze_growable(maze, x, y)) {
      maze_set(maze, x, y, 1);
      if (maze_growable(maze, x2, y2)) maze_set(maze, x2, y2, 1);
    }
  }
}


static void bisimmetricalmaze(maze_t *maze, int granulosity, int density) {
  int n=granulosity*(GRIDX*GRIDY)/100, i;
  for (i=0; i<n; i++) {
    int x=rand()%GRIDX;
    int y=rand()%GRIDY;
    int x2=(GRIDX-1-x);
    int y2=(GRIDY-1-y);
    if (maze_free(maze, x, y)) {
      maze_set(maze, x, y, 1);
      if (maze_free(maze, x, y2)) maze_set(maze, x, y2, 1);
      if (maze_free(maze, x2, y)) maze_set(maze, x2, y, 1);
      if (maze_free(maze, x2, y2)) maze_set(maze, x2, y2, 1);
    }
  }
  n=density*(GRIDX*GRIDY)/100;
  for (i=0; i<n; i++) {
    int x=rand()%(GRIDX+1)/2;
    int y=rand()%(GRIDY+1)/2;
    int x2=(GRIDX-1-x);
    int y2=(GRIDY-1-y);
    if (maze_growable(maze, x, y)) {
      if (maze_growable(maze, x, y)) maze_set(maze, x, y, 1);
      if (maze_growable(maze, x, y2)) maze_set(maze, x, y2, 1);
      if (maze_growable(maze, x2, y)) maze_set(maze, x2, y, 1);
      if (maze_growable(maze, x2, y2)) maze_set(maze, x2, y2, 1);
    }
  }
}

static void nicemaze(maze_t *maze, int granulosity, int density) {
  grainymaze(maze, granulosity);
  growmaze(maze, density);
}

void randommaze(int misc, maze_t *maze) {
  if (misc==0) {
    misc=rand();
    printf("Maze seed: %d\n", misc);
  }
  srand(misc);
  misc=rand()%10;
  if (rand()&1) drawborder(maze, 60+rand()%10);
  switch (misc) {
  case 0: complexmaze(maze, 100+rand()%100); break;
  case 1: reasonablemaze(maze); break;
  case 2: nicemaze(maze, 4, 300); break;
  case 3: case 4: simmetricalmaze(maze, 4, 300, rand()&1, rand()&1); break;
  case 5: case 6: bisimmetricalmaze(maze, 4, 300); break;
  case 7: case 8: case 9: simmetricalmaze(maze, 4, 300, 1, 1); break;
    /*case 10: grainymaze(maze, rand()%200); break; */
  }
}
