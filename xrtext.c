#include <stdio.h>
#include <X11/Xlib.h>
#include <string.h>

#include "config.h"
#include "xroads.h"
#include "xrtext.h"

#include "letters.h"

extern tile_t trueblank;
extern Window mainwindow;

/* Here is the whole C64 character set, from ascii 32 to 126 */

tile_t *xrascii[] = {

   /* ASCII 32 to 47; Various punctuation */

   &space_bits, &bang_bits, &double_quote_bits, &number_bits,
   &dollar_bits, &percent_bits, &ampersand_bits, &quote_bits, 
   &l_paren_bits, &r_paren_bits, &asterisk_bits, &plus_bits, &comma_bits,
   &minus_bits, &period_bits, &slash_bits,

   /* ASCII 48 to 57; Numbers */

   &zero_bits, &one_bits, &two_bits, &three_bits, &four_bits, &five_bits,
   &six_bits, &seven_bits, &eight_bits, &nine_bits,

   /* ASCII 58 to 64; More punctuation */

   &colon_bits, &semi_colon_bits, &less_than_bits, &equals_bits,
   &greater_than_bits, &question_bits, &at_bits,

   /* ASCII 65 to 90; Uppercase letters */

   &A_bits, &B_bits, &C_bits, &D_bits, &E_bits, &F_bits, &G_bits, &H_bits,
   &I_bits, &J_bits, &K_bits, &L_bits, &M_bits, &N_bits, &O_bits, &P_bits,
   &Q_bits, &R_bits, &S_bits, &T_bits, &U_bits, &V_bits, &W_bits, &X_bits,
   &Y_bits, &Z_bits,

   /* ASCII 91 to 96; Even more punctuation */

   &l_bracket_bits, &backslash_bits, &r_bracket_bits, &carat_bits,
   &underscore_bits, &left_quote_bits,

   /* ASCII 97 to 122; Lowercase letters */

   &a_bits, &b_bits, &c_bits, &d_bits, &e_bits, &f_bits, &g_bits, &h_bits,
   &i_bits, &j_bits, &k_bits, &l_bits, &m_bits, &n_bits, &o_bits, &p_bits,
   &q_bits, &r_bits, &s_bits, &t_bits, &u_bits, &v_bits, &w_bits, &x_bits,
   &y_bits, &z_bits,

   /* ASCII 123 to 126; Yet MORE punctuation, ASCII is stupid */

   &l_curly_bits, &pipe_bits, &r_curly_bits, &tilde_bits

   };

/**************************************************************************/
/* Regular Text */
void XRText (char *text, int x, int y, int color) {
   int i;
   for(i = 0; i < strlen(text); i++)
      draw_tilex2(mainwindow, *xrascii[text[i]-32], x+i*GRIDSIZE, y,
                  color, RIGHT);
}

void XRText2 (char *text, int x, int y, int color) {
   int i;
   for(i = 0; i < strlen(text); i++)
      draw_tilex2(mainwindow, *xrascii[text[i]-32], x+i*GRIDSIZE, y,
                  color, RIGHT2);
}

/**************************************************************************/
/* Text drawn upwards */
void XRTextUp (char *text, int x, int y, int color) {
   int i;
   for(i = 0; i < strlen(text); i++)
      draw_tilex2(mainwindow, *xrascii[text[i]-32], x, y-i*GRIDSIZE,
                  color, UP2);
}

void XRTextUp2 (char *text, int x, int y, int color) {
   int i;
   for(i = 0; i < strlen(text); i++)
      draw_tilex2(mainwindow, *xrascii[text[i]-32], x, y-i*GRIDSIZE,
                  color, UP);
}

/**************************************************************************/
/* Text drawn downwards */
void XRTextDown (char *text, int x, int y, int color) {
   int i;
   for(i = 0; i < strlen(text); i++)
      draw_tilex2(mainwindow, *xrascii[text[i]-32], x, y+i*GRIDSIZE,
                  color, DOWN);
}

void XRTextDown2 (char *text, int x, int y, int color) {
   int i;
   for(i = 0; i < strlen(text); i++)
      draw_tilex2(mainwindow, *xrascii[text[i]-32], x, y+i*GRIDSIZE,
                  color, DOWN2);
}

/**************************************************************************/
/* Text drawn left */
void XRTextLeft (char *text, int x, int y, int color) {
   int i;
   for(i = 0; i < strlen(text); i++)
      draw_tilex2(mainwindow, *xrascii[text[i]-32], x-i*GRIDSIZE, y,
                  color, LEFT);
}

void XRTextLeft2 (char *text, int x, int y, int color) {
   int i;
   for(i = 0; i < strlen(text); i++)
      draw_tilex2(mainwindow, *xrascii[text[i]-32], x-i*GRIDSIZE, y,
                  color, LEFT2);
}

