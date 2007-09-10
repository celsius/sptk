/*
  ----------------------------------------------------------------
 Speech Signal Processing Toolkit (SPTK): version 3.0
    SPTK Working Group

     Department of Computer Science
     Nagoya Institute of Technology
    and
    Interdisciplinary Graduate School of Science and Engineering
     Tokyo Institute of Technology
        Copyright (c) 1984-2000
   All Rights Reserved.

  Permission is hereby granted, free of charge, to use and
  distribute this software and its documentation without
  restriction, including without limitation the rights to use,
  copy, modify, merge, publish, distribute, sublicense, and/or
  sell copies of this work, and to permit persons to whom this
  work is furnished to do so, subject to the following conditions:

    1. The code must retain the above copyright notice, this list
       of conditions and the following disclaimer.

    2. Any modifications must be clearly marked as such.

  NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSITITUTE OF TECHNOLOGY,
  SPTK WORKING GROUP, AND THE CONTRIBUTORS TO THIS WORK DISCLAIM
  ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT
  SHALL NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSITITUTE OF
  TECHNOLOGY, SPTK WORKING GROUP, NOR THE CONTRIBUTORS BE LIABLE
  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
  DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
  WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
  ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
  PERFORMANCE OF THIS SOFTWARE.
 ----------------------------------------------------------------
*/

/****************************************************************
* $Id: plot.h,v 1.5 2007/09/10 14:21:01 heigazen Exp $          *  
* HEADER for XY-PLOTTER library                                 *
*                                                               *
*    Following commands are valid for the xy plotter FP5301     *
*                                                               *
*                                               Oct. 1, 1985    *
****************************************************************/

#include <stdio.h>

/* commands for FP5301 */

#define TRM 0    /* terminator */

#define _speed(l, n) printf("!%d %d%c", l, n, TRM) /* speed */
#define _speed_all(l) printf("!%d%c", l, TRM)  /* speed */
#define ERRMASK 0x8022   /* error mask  */
#define _font(n) printf("$%d%c", n, TRM)  /* font */
#define _hatch(m,p,t) printf("%%%d %d %d\n", m, p, t) /* hatching */
#define _factor(p,q,r) printf("&%d %d %d\n", p, q, r) /* factor */
#define ELLIPS 0x8029   /* ellips  */
#define _rotate(x,y,t) printf("/%d %d %d\n", x, y, t) /* rotate */
#define clear()  putchar(':')   /* clear */
#define ifclear() putchar(';')   /* interface clear */
#define term(t)  printf("=%c%c", t, t)  /* term */
#define CLIP 0x803e   /* clipping  */
#define RDOFF 0x803f   /* read offset  */
#define ARESET 0x8041   /* alpha reset  */
#define line_scale(l) printf("B%d\n", l)  /* line scale */
#define draw(x, y) printf("D%d %d\n%c", x, y, TRM) /* draw */
#define _draw()  putchar('D')   /* draw command */
#define rdraw(x, y) printf("E%d %d\n%c", x, y, TRM) /* relative draw */
#define home();  putchar('H')   /* home */
#define aitalic(p) printf("I%d\n", p)  /* alpha italic */
#define newpen(p) printf("J%d\n", p)  /* new pen */
#define join_type(p) printf("K%d\n", p)  /* join type */
#define KANA 0x804b   /* kana   */
#define line_type(p) printf("L%d\n", p)  /* line type */
#define move(x, y) printf("M%d %d\n", x, y) /* move */
#define _mark(m) printf("N%d\n", m)  /* mark */
#define rmove(x, y) printf("O%d %d\n", x, y) /* relative move */
#define print(p) printf("P%s%c", p, TRM)  /* print */
#define aspace(l) printf("Q%d\n", l)  /* alpha space */
#define arotate(t) printf("R%d\n", t)  /* alpha rotate */
#define ascale(a) printf("S%d\n", a)  /* alpha scale */
#define PROMPT 0x8054   /* prompt light  */
#define _circle(x,y,q,r,s,t) printf("W%d %d %d %d %d %d%c",x,y,q,r,s,t,TRM)
/* circle */
#define _axis(p,q,r,s,t) printf("X%d %d %d %d %d%c", p,q,r,s,t,TRM)
/* axis */
#define _curve(a) printf("Y%d\n", a)  /* curve */
#define wup_left(x, y) printf("Z%d %d\n", x, y) /* write upper right */
#define wlo_right(x, y) printf("\\%d %d\n", x, y) /* write lower left */
#define _rcircle(q,r,s,t) printf("]%d %d %d %d%c",q,r,s,t,TRM)
/* relative circle */
#define _offset(x, y) printf("^%d %d\n", x, y) /* offset */
#define RCURVE 0x805f   /* relative curve */
#define asciimode() printf("%c%c", 0x80, 0)  /* binary terminator */
#define terminate() putchar(TRM)   /* terminator */
#define sndcord(x, y) printf("%d %d\n", x, y)  /* send (x, y) */

/* interfacing */

struct co_ord {
   float xo;
   float yo;
};

#define sign(x)  (x & 0xffff8000)
#define DEG_RAD  (3.141592653589793 / 180.0)
#define SCALE 4096

/*  Required Functions  */
void rotate(float th);
void factor(float fx, float fy);
int offset(float x, float y);
void bound(double xl, double yl, double xh, double yh);
void rstbnd();
int hatch(int ip, float *ax, float *ay, int n, float d, float t);
int pen(int pen);
int font(int n);
int speed(int isp, int ip);
int mark(int mrk, float ax[], float ay[], int n, float f, int m);
int symbol(float x, float y, char *text, float h, float s, float th);
int number(float x, float y, float fval, float h, float s, float th, int m, int n);
int italic(float th);
int line(int ip, float *ax, float *ay, int n);
int circle(float x, float y, float rs, float re, float ths, float the);
int pntstyl(int ip);
void plots(int mode);
void plote();
int _plnorm(float x);
int _cordx(float x);
int _cordy(float y);
void plot(float x,float y, int z);
int plotr(int ip, float x, float y);
int plota(int ip, float x, float y);
void mode(int ltype, float lscale);
void _chlnmod(int lmode);
void origin(float x, float y);
void graph(FILE *fp);
void draw_fig0(float x[],float y[],int n);
int flush(float x[],float y[],int n);
void polyg(float x[],float y[],int n);
int is_in(float x,float y);
void swap(float *x,float *y);
char *gettxt(char *s);
char *script(char *s, int i);
char *getarg(char *s, char *arg);
char *gettyp(char *s, char *t);
char *getname(char *s, char *t);
int greek(char *p);
float sleng(char *p, float h, float w);
void _symbol(float x, float y, char *p, float h, float w, float t);
float ysadj(void);
float rx(float x, float y, float t);
float ry(float x, float y, float t);
float argapf(float x, float a);
