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
* XY PLOTTER library     *
*        *
* Calling sequence :     *
*  factor(fx, fy);     *
*  rotate(th);     *
*  offset(x, y);     *
*  bound(xl, yl, xh, yh);    *
*  hatch(ip, x, y, n, d, t);   *
*  pen(ip);     *
*  speed(isp);     *
*  mark(mrk, ax, ay, n, f[, m]);   *
*  symbol(x, y, text, h, s, th)   *
*  number(x, y, fval, h, s, th, m, n);  *
*  italic(th)     *
*  font(n)      *
*  line(ip, x, y, n);    *
*  circle(x, y, rs, re, ths, the)   *
*  rcircle(rs, re, ths, the);   *
*        *
* Copyright 1985 by T. Kobayashi    *
****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "plot.h"

extern struct co_ord _org;

void rotate (float th)
{
   _rotate(_plnorm(_org.xo), _plnorm(_org.yo), _plnorm(th));
}

void factor (float fx, float fy)
{
   _factor((int)(fx * SCALE + 0.5), (int)(fy * SCALE + 0.5), SCALE);
}

int offset (float x, float y)
{
   _offset(_plnorm(x), _plnorm(y));
   return(0);
}

void bound (double xl, double yl, double xh, double yh)
{
   wlo_right(_cordx(xl), _cordy(yl));
   wup_left(_cordx(xh), _cordy(yh));
}

void rstbnd (void)
{
   wlo_right(0, 0);
   wup_left(4000, 2850);
}

int hatch (int ip, float *ax, float *ay, int n, float d, float t)
{
   if (n<3)
      return(1);

   ip += 20;
   _hatch(ip, _plnorm(d), _plnorm(t));

   while (--n >= 0)
      sndcord(_cordx(*ax++), _cordy(*ay++));
   terminate();
   return(0);
}

int pen (int pen)
{
   if (pen<1 || pen>10)
      return(1);
   newpen(pen);
   return(0);
}

int join (int join)
{
   if (join<0 || join>2)
      return(1);
   join_type(join);
   return(0);
}

int font (int n)
{
   if (n<0 || n>19)
      return(1);
   _font(n);
   return(0);
}

int speed (int isp, int ip)
{
   if (isp<0 || isp>10 || ip<0 || ip>10)
      return(1);
   if (ip)
      _speed_all(isp);
   else
      _speed(isp, ip);
   return(0);
}

int mark (int mrk, float ax[], float ay[], int n, float f, int m)
{
   int i;

   if (mrk<0 || mrk>15)
      return(1);
   m = (n<0) ? m : 1;
   if ((n=abs(n))<1 || m<0)
      return(1);
   ascale(_plnorm(f*1.75));
   arotate(0);
   for (i=0; i<n; i+=m) {
      plot(ax[i], ay[i], 3);
      _mark(mrk);
   }
   return(0);
}

int symbol (float x, float y, char *text, float h, float s, float th)
{
   plot(x, y, 3);
   ascale(_plnorm(h));
   aspace(_plnorm(s));
   arotate(_plnorm(th));
   print(text);
   return(0);
}

int number (float x, float y, float fval, float h, float s, float th, int m, int n)
{
   char buf[32], format[8];

   if (abs(m)>10)
      return(1);
   if (n <= 0) {
      while (++n<0)
         fval /= 10.0;
      sprintf(format, "%%%dd%s", m, (n) ? ".\r" : "\r");
      sprintf(buf, format, (long)fval);
   }
   else {
      sprintf(format, "%%%d.%df\r", m, n);
      sprintf(buf, format, fval);
   }
   return(symbol(x, y, buf, h, s, th));
}

int italic (float th)
{
   int theta;

   if ((theta=256 * tan(th * DEG_RAD))>4000)
      return(1);
   aitalic(theta);
   return(0);
}

int line (int ip, float *ax, float *ay, int n)
{
   struct {
      short x;
      short y;
   } b, o, pb;
   int dx, dy;

   if (n<2)
      return(1);
   if (ip)
      _chlnmod(1);
   plot(*ax++, *ay++, 3);
   o.x = dx = _cordx(*ax++);
   o.y = dy = _cordy(*ay++);
   pb = o;
   _draw();

   while (--n>1) {
      dx = (b.x = _cordx(*ax++)) - pb.x;
      dy = (b.y = _cordy(*ay++)) - pb.y;
      if (dx || dy) {
         if (dx==0 && o.x==0 && sign(dy)==sign(o.y))
            pb.y = b.y;
         else if (dy==0 && o.y==0 && sign(dx)==sign(o.x))
            pb.x = b.x;
         else {
            sndcord(pb.x, pb.y);
            pb.x = b.x;
            pb.y = b.y;
         }
         o.x = dx;
         o.y = dy;
      }
   }
   sndcord(pb.x, pb.y);
   terminate();
   if (ip)
      _chlnmod(0);
   return(0);
}

int circle (float x, float y, float rs, float re, float ths, float the)
{
   int r1, r2;

   r1 = _plnorm(rs);
   r2 = _plnorm(re);
   if (r1==0 && r2==0)
      return(1);
   _circle(_cordx(x), _cordy(y), r1, r2, _plnorm(ths), _plnorm(the));
   return(0);
}

int pntstyl (int ip)
{
   ip += 20;
   aspace(ip); /* valid for only LBP */
   return(0);
}
