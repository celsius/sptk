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
#include <stdio.h>

struct bbmargin  {      /*  Bounding Box Margin  */
       int      top;
       int      bottom;
       int      left;
       int      right;
} bbm;

struct page_media  {
       char    *size;
       int      width;
       int      height;
};

#define   PU_PT      (72.0/254.0)
#define   SHIFT      15
#define   LAND_OFFSET   254
#define   SCALE      10

#define   MIN_OFFSET   12
#define   MAX_OFFSET   17
#define   CHAR_HEIGHT   10

#define   norm(x)      (int)(x)

typedef struct cord  {
       int   x;
       int   y;
} Cord;

extern char   *filename;
extern char   *title;
extern char   *progname;

extern struct  page_media   paper[];
extern char   *orientations[];

extern char   *media;
extern int     xleng;
extern int     yleng;
extern int     resolution;
extern int     paper_num;
extern char   *orientation;

extern int     psmode;
extern int     landscape;
extern int     font_no;
extern int     clip_mode;

void epsf_setup(FILE *fp, float shrink, int xoffset, int yoffset, struct bbmargin bbm, int ncopy );
void epsf_init(int *xmin, int *ymin, int *xmax, int *ymax, int ncopy );
void epsf_scale(float shrink,int xoffset,int yoffset );
void epsf_end();
void bbox(FILE *fp, int *xmin, int *ymin, int *xmax, int *ymax, float shrink, int xoffset, int yoffset, struct bbmargin bbm );
int getd( FILE *fp, int *x, int *y );
int getstrlength(FILE *fp );
void dict();
void plot( FILE *fp );
void _move(int x, int y );
void _line(FILE *fp );
int _getcord(FILE *fp, Cord *buf);
void _send(Cord *buf);
void _flush();
void polylines(int *x,int *y,int n );
void polyg( FILE *fp,int type );
void intstyle(int type );
void hatching(FILE *fp,int type );
void rectangle(int x0,int y0,int x1,int y1,int frame,int fill );
void get_str(FILE *fp );
void text(char *s,int n,int fn );
void line_type(int type );
void join_type(int type );
void newpen(int width );
void mark(int no );
void rect(int x,int y,int w,int h,int fill );
void tangle(int size,int fill );
void arc(int x,int y,int r,int ang1,int ang2,int fill );
void symbol(char *code,int size,int xoffset,int yoffset );
void clip(int x0,int y0,int x1,int y1 );
