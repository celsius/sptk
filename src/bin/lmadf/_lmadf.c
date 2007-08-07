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

    $Id: _lmadf.c,v 1.4 2007/08/07 05:01:38 heigazen Exp $

    LMA Digital Filter

 double lmadflt(x, c, m, pd, d)

 double x   : input
 double *c  : cepstrum
 int m   : order of cepstrum
 int pd  : order of pade approximation
 double  *d  : delay

 return value : filtered data

*****************************************************************/

#include <stdio.h>
#include <SPTK.h>


static double pade[] = {1.0,
                        1.0, 0.0,
                        1.0, 0.0, 0.0,
                        1.0, 0.0, 0.0,    0.0,
                        1.0, 0.4999273, 0.1067005, 0.01170221, 0.0005656279,
                        1.0, 0.4999391, 0.1107098, 0.01369984, 0.0009564853, 0.00003041721
                       };

double *ppade;

double lmadf (double x, double *c, const int m, const int pd, double *d)
{
   ppade = &pade[pd*(pd+1) / 2];

   x = lmadf1(x, c, m, d, 1, 1, pd);   /* D1(z) */
   x = lmadf1(x, c, m, &d[(m+1)*pd], 2, m, pd); /* D2(z) */

   return(x);
}

/****************************************************************

 double lmadf1(x, c, m, d, m1, m2, pd)

 double x  : input
 double *c : cepstrum
 int m  : order of cepstrum
 double  *d : delay
 int m1 : start order
 int m2 : end order
 int pd : order of pade approximation

*****************************************************************/

double lmadf1 (double x, double *c, const int m, double *d, const int m1, const int m2, const int pd)
{
   double y, t, *pt;
   int i;

   pt = &d[pd*m];
   t = lmafir(pt[pd-1], c, m, &d[(pd-1)*m], m1, m2);
   y = (t *= ppade[pd]);
   x += (1 & pd) ? t : -t;
   for (i=pd-1; i>=1; i--) {
      pt[i] = t = lmafir(pt[i-1], c, m, &d[(i-1)*m], m1, m2);
      y += (t *= ppade[i]);
      x += (1 & i) ? t : -t;
   }
   y += (pt[0] = x);

   return(y);
}

/****************************************************************

 double lmafir(x, c, d, m, m1, m2)

 double x  : input
 double *c : cepstrum
 int m  : order of cepstrum
 double  *d : delay
 int m1 : start order
 int m2 : end order

*****************************************************************/

double lmafir (double x, double *c, const int m, double *d, const int m1, const int m2)
{
   int i;

   for (i=m-1; i>=1; i--) d[i] = d[i-1];
   d[0] = x;
   for (x=0.0,i=m1; i<=m2; i++) x += c[i] * d[i-1];

   return(x);
}

