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

/************************************************************************
*                                                                       *
*      $Id: _root_pol.c,v 1.6 2007/09/12 08:37:20 heigazen Exp $        *
*                                                                       *
*      Durand-Kerner-Aberth for Higher Order Equation                   *
*                                                                       *
*                                         1988.12 K.Fukushi             *
*                                         1989.3  T.Kobayashi modified  *
*                                         1999.12 T.Yoshimura modified  *
*                                                                       *
*    Usage: root_pol(a, odr, x, a_zero, eps, itrat)                     *
*                                                                       *
*    Description:                                                       *
*                                                                       *
*        real*         a[2*odr];                                        *
*        int           odr, a_zero, itrat;                              *
*        double        eps;                                             *
*                                                                       *
*        Pn(x) = a[0]x^n + a[1]x^(n-1) + a[2]x^(n-2) + ... + a[n]       *
*                                                                       *
*    Output:                                                            *
*                                                                       *
*        x[0].re,x[0].im,...,x[n-1].re,x[n-1].im                        *
*                                                                       *
*                                                                       *
************************************************************************/

/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SPTK.h>

typedef enum { plus, minus, multiply, divide } opt;


static double rad_root (const double x, const int i)
{
   if (x==0.0)
      return -1.0;
   else
      return exp(log(x)/i);
}

static complex c_math (complex c1, opt op, complex c2)
{
   double p;
   complex t;

   switch (op) {
   case plus :
      t.re = c1.re + c2.re;
      t.im = c1.im + c2.im;
      break;
   case minus :
      t.re = c1.re - c2.re;
      t.im = c1.im - c2.im;
      break;
   case multiply :
      t.re = c1.re*c2.re - c1.im*c2.im;
      t.im = c1.re*c2.im + c1.im*c2.re;
      break;
   case divide :
      p = c2.re*c2.re + c2.im*c2.im;
      t.re = (c1.re*c2.re + c1.im*c2.im)/p;
      t.im = (c1.im*c2.re - c1.re*c2.im)/p;
      break;
   default :
      t.re = c1.re;
      t.im = c1.im;
      break;
   }
   return t;
}

static double c_mag (complex x)
{
   return sqrt(x.re*x.re + x.im*x.im);
}

static double c_arg (complex x)
{
   return atan2(x.im, x.re);
}

void output_root_pol (complex *x, int odr, int form)
{
   int i, k;
   double mag, arg, *a;

   a = dgetmem(2*odr);

   switch (form) {
   case 1:
      for (k=i=0; i<odr; i++) {
         a[k++] = c_mag(x[i+1]);
         a[k++] = c_arg(x[i+1]);
      }
      break;
   case 2:
   case 3:
      for (k=i=0; i<odr; i++) {
         mag = 1 / c_mag(x[i+1]);
         arg = - c_arg(x[i+1]);
         if (form==3) {
            a[k++] = mag;
            a[k++] = arg;
         }
         else {
            a[k++] = mag * cos(arg);
            a[k++] = mag * sin(arg);
         }
      }
      break;
   case 0:
   default:
      for (k=i=0; i<odr; i++) {
         a[k++] = x[i+1].re;
         a[k++] = x[i+1].im;
      }
      break;
   }

   fwritef(a, sizeof(*a), odr*2, stdout);
   
   return;
}

complex *cplx_getmem (const int leng)
{
   int i;
   complex *p=NULL;

   if ((p=(complex *)malloc(sizeof(complex)*leng))==NULL) {
      fprintf(stderr, "Memory allocation error !\n");
      exit(3);
   }

   for (i=0;i<leng;i++) p[i].re = p[i].im = 0;

   return p;
}

void root_pol (double *a, const int odr, complex *x, const int a_zero, const double eps, const int itrat)
{
   int i, j, k, l;
   double th, th1, th2, cm, cmax;
   complex cden, cnum, c1, *deltx;

   deltx = cplx_getmem(odr);

   if (!a_zero)
      for (i=1; i<=odr; i++) a[i] /= a[0];

   cmax = 0;
   for (i=2; i<=odr; i++) {
      cm = odr * rad_root(fabs(a[i]),i);
      if (cm>cmax) cmax = cm;
   }

   th1 = PI*2.0 / odr;
   th2 = th1 / 4.0;
   for (i=1; i<=odr; i++) {
      th = th1*(i-1) + th2;
      x[i].re = cmax*cos(th);
      x[i].im = cmax*sin(th);
   }

   l = 1;
   do {
      for (i=1; i<=odr; i++) {
         cden.re = 1.0;
         cden.im = 0.0;
         cnum.re = 1.0;
         cnum.im = 0.0;
         c1 = x[i];
         for (j=1; j<=odr; j++) {
            cnum = c_math(cnum, multiply, c1);
            cnum.re += a[j];
            if ( j!=i )
               cden = c_math(cden, multiply, c_math(c1, minus, x[j]));
         }
         deltx[i] = c_math(cnum, divide, cden);
         x[i] = c_math(c1, minus, deltx[i]);
      }
      k = 1;
      while ((k<=odr) && (c_mag(deltx[k++])<=eps));
      l++;
   }
   while ((l<=itrat) && (k<=odr));

   if (l>itrat) {
      fprintf(stderr, "root_pol : No convergence.\n");
      exit(1);
   }
   
   return;
}

