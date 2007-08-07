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
* XY-Plotter Library       *
*        *
* Calling sequence :     *
*  plopen(mode);     *
*  _plsend(buf, nbytes);    *
*  _plnorm(x);     *
*  plots(mode);     *
*  plote();     *
*  plot(x, y, z);     *
*  plotr(ip, x, y);    *
*  plota(ip, x, y);    *
*  origin(x, y);     *
*  mode(ltype, lscale);    *
*  chlnmod(_lnmode);    *
*        *
* Copyright 1985 by T. Kobayashi    *
****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "plot.h"

struct co_ord _org = { 0.0, 0.0 };

void plots (int mode)
{
   if ((mode & 0x80)==0) {
      asciimode();  /* ASCII mode */
      ifclear();  /* Interface CLEAR */
      sleep(1);  /* delay */
      home();   /* HOME */
   }
   term(TRM);
   _org.xo = _org.yo = 0;
}

void plote (void)
{
   return;
}

int _plnorm (float x)
{
   return((int)(10.0 * x + 0.5));
}

int _cordx (float x)
{
   return(_plnorm(x + _org.xo));
}

int _cordy (float y)
{
   return(_plnorm(y + _org.yo));
}

static short _ltype=0, _lscale=10;

void plot (float x, float y, int z)
{
   int ip;

   if (abs(z)==2)
      ip = 0;
   else if (abs(z)==3)
      ip = 1;
   else {
      if (z==999)
         plote();
      return;
   }
   plota(ip, x, y);
   if (z<0) {
      _org.xo += x;
      _org.yo += y;
   }
}

int plotr (int ip, float x, float y)
{
   if (ip==1)
      rmove(_plnorm(x), _plnorm(y));
   else if (ip==0 || ip==2) {
      if (ip==2)
         _chlnmod(1);
      rdraw(_plnorm(x), _plnorm(y));
      if (ip==2)
         _chlnmod(0);
   }
   else
      return(1);
   return(0);
}

int plota (int ip, float x, float y)
{
   if (ip==1)
      move(_cordx(x), _cordy(y));
   else if (ip==0 || ip==2) {
      if (ip==2)
         _chlnmod(1);
      draw(_cordx(x), _cordy(y));
      if (ip==2)
         _chlnmod(0);
   }
   else
      return(1);
   return(0);
}

void mode (int ltype, float lscale)
{
   _ltype  = ltype;
   _lscale = _plnorm(lscale);
}

void _chlnmod (int lmode)
{
   if (lmode) {
      line_type(_ltype);
      line_scale(_lscale);
   }
   else
      line_type(0);
}

void origin (float x, float y)
{
   _org.xo = x;
   _org.yo = y;
   plot(0., 0., 3);
}
