/*
  ---------------------------------------------------------------  
            Speech Signal Processing Toolkit (SPTK)

                      SPTK Working Group                           
                                                                   
                  Department of Computer Science                   
                  Nagoya Institute of Technology                   
                               and                                 
   Interdisciplinary Graduate School of Science and Engineering    
                  Tokyo Institute of Technology                    
                                                                   
                     Copyright (c) 1984-2007                       
                       All Rights Reserved.                        
                                                                   
  Permission is hereby granted, free of charge, to use and         
  distribute this software and its documentation without           
  restriction, including without limitation the rights to use,     
  copy, modify, merge, publish, distribute, sublicense, and/or     
  sell copies of this work, and to permit persons to whom this     
  work is furnished to do so, subject to the following conditions: 
                                                                   
    1. The source code must retain the above copyright notice,     
       this list of conditions and the following disclaimer.       
                                                                   
    2. Any modifications to the source code must be clearly        
       marked as such.                                             
                                                                   
    3. Redistributions in binary form must reproduce the above     
       copyright notice, this list of conditions and the           
       following disclaimer in the documentation and/or other      
       materials provided with the distribution.  Otherwise, one   
       must contact the SPTK working group.                        
                                                                   
  NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSTITUTE OF TECHNOLOGY,   
  SPTK WORKING GROUP, AND THE CONTRIBUTORS TO THIS WORK DISCLAIM   
  ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL       
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT   
  SHALL NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSTITUTE OF         
  TECHNOLOGY, SPTK WORKING GROUP, NOR THE CONTRIBUTORS BE LIABLE   
  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY        
  DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,  
  WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTUOUS   
  ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR          
  PERFORMANCE OF THIS SOFTWARE.                                    
                                                                   
  ---------------------------------------------------------------  
*/

/****************************************************************
* NAME : fig0 - subroutines for fig   *
****************************************************************/
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "fig.h"
#include "plot.h"

#define xt(x) ((is_xlog<0) ? log10(x) : (x))
#define yt(y) ((is_ylog<0) ? log10(y) : (y))
#define abs(x) (((x)<0) ? -(x) : (x))
#define nz(mx, mn) ((mx>mn) ? mx : mn)


extern int ltype, type, is_t;
extern float xo, yo, xl, yl, x00, y00, mh, mw, h, w;
extern float xclip0, yclip0, xclip1, yclip1;

static int lmod[]={ 0, 2, 6, 3, 4}, ptyp=1;
static int is_xlog=0, is_ylog=0;
static float lpit[]={ 10, 1.6, 10, 3, 5};
static float dhat=1, that=45;
static float xfct=1, yfct=1;
static char label[BUFLNG / 2]="";
static float xbuf[BUFLNG], ybuf[BUFLNG];

void graph(FILE *fp)
{
   static char buf[BUFLNG], arg[BUFLNG / 2], xtype[16], ytype[16];
   static float xa, ya, xap, yap, xmin, xmax, ymin, ymax;
   static float xs=-NSCALE, ys=-NSCALE;
   int n, c;
   char *s, *p;
   float  x, y, lpt, th, dt, lscale, rad;
   int  is_grid, old_lbl=0;
   char  xory;

   for (n=0; (s=fgets(buf, BUFLNG, fp)); ) {
      s = getarg(s, arg);
      if (s==NULL || *arg=='#')
         ;    /* comment line */
      else if ( (!is_t && strcmp(arg, "x")==0) || (is_t && strcmp(arg, "y")==0) ) {
         s = gettyp(s, xtype);
         if (sscanf(s, "%f %f %f", &xmin, &xmax, &xa) != 3)
            xa = xmin;
         if (strncmp(xtype, "log", 3)==0) {
            xmin = log10(xmin);
            xmax = log10(xmax);
            xa = log10(xa);
            is_xlog = (xtype[3]=='*') ? -1 : 1;
         }
         xfct = xl / (xmax - xmin);
         xap  = (xa - xmin) * xfct;
         x00  = - xmin * xfct;
      }
      else if ( (!is_t && strcmp(arg, "y")==0) || (is_t && strcmp(arg, "x")==0) ) {
         s = gettyp(s, ytype);
         if (sscanf(s, "%f %f %f", &ymin, &ymax, &ya) != 3)
            ya = ymin;
         if (strncmp(ytype, "log", 3)==0) {
            ymin = log10(ymin);
            ymax = log10(ymax);
            ya = log10(ya);
            is_ylog = (ytype[3]=='*') ? -1 : 1;
         }
         yfct = (yl) ? yl / (ymax - ymin) : 0;
         yap  = (ya - ymin) * yfct;
         y00  = - ymin * yfct;
      }
      else if ( (!is_t && strncmp(arg, "xscale", 6)==0) || (is_t && strncmp(arg, "yscale", 6)==0) ) {
         is_grid = *(arg + 6);
         if (type<0 || (ya != ymin && ya != ymax)) {
            plot(0.0, yap, 3);
            plot(xl,  yap, 2);
         }
         ys = yap - h - MSCALE;
         while ((s = getarg(s, p=arg)) != NULL) {
            if (*p != '"') {
               x = atof((is_number(*p)) ? p : p + 1);
               if (strncmp(xtype, "mel", 3)==0)
                  x = argapf(x / nz(xmax, xmin),
                             atof(xtype + 3)) * nz(xmax, xmin);
               else if (is_xlog)
                  x = log10(x);
               x = (x - xmin) * xfct;
               lscale = (*p=='s') ? LSCALE / 2:
                        LSCALE;
               if (*p != '\\' && *p != '@') {
                  plot(x, yap, 3);
                  plot(x, yap + lscale, 2);
                  if (type>0 && !is_grid
                        && yap==0) {
                     plot(x, yl, 3);
                     plot(x, yl - lscale, 2);
                  }
               }
               else if (*p=='\\')
                  ++p;
            }
            if (is_number(*p) || *p++=='"')
               _symbol(x - sleng(p, h, w) / 2,
                       ys - ysadj(), p, h, w, 0.0);
         }
      }
      else if ( (!is_t && strncmp(arg, "yscale", 6)==0) || (is_t && strncmp(arg, "xscale", 6)==0) ) {
         is_grid = *(arg + 6);
         if (type<0 || (xa != xmin && xa != xmax)) {
            plot(xap, 0.0, 3);
            plot(xap, yl,  2);
         }
         while ((s = getarg(s, p=arg)) != NULL) {
            if (*p != '"') {
               y = atof((is_number(*p)) ? p : p + 1);
               if (strncmp(ytype, "mel", 3)==0)
                  y = argapf(y / nz(ymax, ymin),
                             atof(ytype + 3)) * nz(ymax, ymin);
               else if (is_ylog)
                  y = log10(y);
               y = (y - ymin) * yfct;
               lscale = (*p=='s') ? LSCALE / 2:
                        LSCALE;
               if (*p != '\\' && *p != '@') {
                  plot(xap, y, 3);
                  plot(xap + lscale, y, 2);
                  if (type>0 && !is_grid
                        && xap==0) {
                     plot(xl, y, 3);
                     plot(xl - lscale, y, 2);
                  }
               }
               else if (*p=='\\')
                  ++p;
            }
            if (is_number(*p) || *p++=='"') {
               x = xap - sleng(p, h, w) - MSCALE;
               if (x<xs)
                  xs = x;
               _symbol(x, y - h * 0.5, p, h, w, 0.0);
            }
         }
      }
      else if (strcmp(arg + 1, "grid")==0) {
         draw_fig0(xbuf, ybuf, n);
         if ( (!is_t && (*arg=='x')) || (is_t && (*arg=='y')) ) {
            ybuf[0] = 0;
            ybuf[1] = yl;
            while ((s = getarg(s, arg)) != NULL) {
               x = atof(arg);
               if (is_xlog)
                  x = log10(x);
               xbuf[0] = xbuf[1]
                         = (x - xmin) * xfct;
               draw_fig0(xbuf, ybuf, 2);
            }
         }
         else {
            xbuf[0] = 0;
            xbuf[1] = xl;
            while ((s=getarg(s, arg)) != NULL) {
               y = atof(arg);
               if (is_ylog)
                  y = log10(y);
               ybuf[0] = ybuf[1]
                         = (y - ymin) * yfct;
               draw_fig0(xbuf, ybuf, 2);
            }
         }
         n = 0;
      }
      else if (strcmp(arg + 1, "circle")==0) {
         xory = *arg;
         s = getarg(s, arg);
         x = xt(atof(arg));
         s = getarg(s, arg);
         y = yt(atof(arg));
         swap(&x, &y);
         x = xfct * x + x00;
         y = yfct * y + y00;
         while ((s = getarg(s, arg)) != NULL) {
            if ( (!is_t && xory=='x') || (is_t && xory=='y') )
               rad = xt(atof(arg)) * xfct;
            else
               rad = yt(atof(arg)) * yfct;
            pntstyl(ptyp);
            circle(x, y, rad, rad, 0., 360.);
         }
      }
      else if (strcmp(arg, "circle")==0) {
         s = getarg(s, arg);
         x = xt(atof(arg));
         s = getarg(s, arg);
         y = yt(atof(arg));
         swap(&x, &y);
         x = xfct * x + x00;
         y = yfct * y + y00;
         while ((s = getarg(s, arg)) != NULL) {
            rad = atof(arg);
            pntstyl(ptyp);
            circle(x, y, rad, rad, 0., 360.);
         }
      }
      else if (strcmp(arg + 1, "name")==0) {
         s = getname(s, p = arg + 1);
         if ( (!is_t && *arg=='x') || (is_t && *arg=='y') )
            _symbol((xl - sleng(s, h, w)) / 2,
                    (*p) ? - atof(p) - h : ys - h - NSCALE,
                    s, h, w, 0.0);
         else
            _symbol((*p) ? - atof(p) : xs - MSCALE,
                    (yl - sleng(s, h, w)) / 2, s, h, w, 90.0);
      }
      else if (strncmp(arg, "title", 5)==0
               || strncmp(arg, "print", 5)==0) {
         sscanf(s, "%f %f", &x, &y);
         swap(&x, &y);
         if (*arg=='p') {
            x = xfct * xt(x) + x00;
            y = yfct * yt(y) + y00;
         }
         s = gettxt(s);
         th = getarg(s + strlen(s) + 1, arg) ? atof(arg) : 0;
         if (*(arg + 5)) {
            x -= rx(LADJ * h / 2, h / 2, th);
            y -= ry(LADJ * h / 2, h / 2, th);
         }
         _symbol(x, y, s, h, w, th);
      }
      else if (strcmp(arg, "eod")==0 || strcmp(arg, "EOD")==0) {
         draw_fig0(xbuf, ybuf, n);
         n = 0;
      }
      else if (strcmp(arg, "pen")==0) {
         n = flush(xbuf, ybuf, n);
         pen(atoi(s));
      }
      else if (strcmp(arg, "join")==0) {
         n = flush(xbuf, ybuf, n);
         join(atoi(s));
      }
      else if (strcmp(arg, "csize")==0) {
         if (sscanf(s, "%f %f", &h, &w) != 2)
            w = h;
      }
      else if (strcmp(arg, "hight")==0) {
         if (sscanf(s, "%f %f", &mh, &mw) != 2)
            mw = mh;
      }
      else if (strcmp(arg, "line")==0) {
         n = flush(xbuf, ybuf, n);
         if (sscanf(s, "%d %f", &ltype, &lpt) != 2) {
            if (ltype>0)
               lpt = lpit[ltype - 1];
         }
         if (--ltype >= 0)
            mode(lmod[ltype], lpt);
      }
      else if (strcmp(arg, "italic")==0)
         italic(atof(s));
      else if (strcmp(arg, "mark")==0) {
         while (*s==' ' || *s=='\t')
            ++s;
         if (*s=='\\' && *(s + 1)=='0')
            *label = '\0';
         else
            strcpy(label, s);
      }
      else if (strcmp(arg, "paint")==0) {
         sscanf(s, "%d %f %f", &ptyp, &dhat, &that);
      }
      else if (strcmp(arg, "clip")==0) {
         draw_fig0(xbuf, ybuf, n);
         for (n = 0; (s = getarg(s, arg)) != NULL; ++n) {
            x = xt(atof(arg));
            if ((s = getarg(s, arg))==NULL)
               break;
            y = yt(atof(arg));
            swap(&x, &y);
            xbuf[n] = xfct * x + x00;
            ybuf[n] = yfct * y + y00;
         }
         if (n==0) {
            xclip0 = yclip0 = 0;
            xclip1 = xl;
            yclip1 = yl;
            swap(&xclip1, &yclip1);
         }
         else if (n==2) {
            xclip0 = xbuf[0];
            yclip0 = ybuf[0];
            xclip1 = xbuf[1];
            yclip1 = ybuf[1];
         }
         n = 0;
      }
      else if (strcmp(arg, "box")==0) {
         draw_fig0(xbuf, ybuf, n);
         for (n = 0; (s = getarg(s, arg)) != NULL; ++n) {
            x = xt(atof(arg));
            if ((s = getarg(s, arg))==NULL)
               break;
            y = yt(atof(arg));
            swap(&x, &y);
            xbuf[n] = xfct * x + x00;
            ybuf[n] = yfct * y + y00;
         }
         if (n==2) {
            xbuf[2] = xbuf[1];
            ybuf[3] = ybuf[2] = ybuf[1];
            ybuf[1] = ybuf[0];
            xbuf[3] = xbuf[0];
            n = 4;
         }
         polyg(xbuf, ybuf, n);
         n = 0;
      }
      else {
         x = xt(atof(arg));
         s = getarg(s, arg);
         y = yt(atof(arg));
         swap(&x, &y);
         xbuf[n] = x = xfct * x + x00;
         ybuf[n] = y = yfct * y + y00;
         if (is_in(x, y) && ((s = getarg(s, arg))
                             || *label || old_lbl>0 )) {
            c = 0;
            if (s || *label) {
               if (s==NULL)
                  s = getarg(label, arg);
               if (*arg=='\\' && (abs(c=atoi(arg + 1)))<16)
                  mark(abs(c),&x,&y,1,mh,1);
               else if (abs(c)==16) {
                  pntstyl(ptyp);
                  circle(x,y,mh/2,mh/2,0.,360.);
               }
               else {
                  if (c) {
                     *arg = c;
                     *(arg + 1) = '\0';
                  }
                  _symbol(x - LADJ*h/2,y - w / 2,
                          arg, h, w, atof(s));
               }
            }
            if (c>0)
               n = flush(xbuf, ybuf, n);
            if ((c>0  || old_lbl>0) && n) {
               dt = atan2(y - ybuf[0], x - xbuf[0]);
               if (old_lbl>0) {
                  xbuf[0] += MADJ * mh * cos(dt);
                  ybuf[0] += MADJ * mh * sin(dt);
               }
               if (c>0) {
                  xbuf[1] -= MADJ * mh * cos(dt);
                  ybuf[1] -= MADJ * mh * sin(dt);
               }
               draw_fig0(xbuf, ybuf, 2);
               xbuf[0] = x;
               ybuf[0] = y;
               n = 0;
            }
            old_lbl = c;
         }
         if (++n >= BUFLNG)
            n = flush(xbuf, ybuf, n);
      }
   }
   draw_fig0(xbuf, ybuf, n);
}

void draw_fig0(float x[],float y[],int n)
{
   if (n && ltype >= 0) {
      bound(xclip0, yclip0, xclip1, yclip1);
      line(1, x, y, n);
      rstbnd();
   }
}

int flush(float x[],float y[],int n)
{
   if (n>1) {
      draw_fig0(x, y, n);
      x[0] = x[n - 1];
      y[0] = y[n - 1];
      return(1);
   }
   else
      return(n);
}

void polyg(float x[],float y[],int n)
{
   bound(xclip0, yclip0, xclip1, yclip1);
   hatch(ptyp, x, y, n, dhat, that);
   rstbnd();
}

int is_in(float x,float y)
{
   if (x >= 0 && x <= xl && y >= 0 && y <= yl)
      return(1);
   else
      return(0);
}

void swap(float *x,float *y)
{
   float t;

   if (is_t) {
      t = *y;
      *y = *x;
      *x = t;
   }
}
