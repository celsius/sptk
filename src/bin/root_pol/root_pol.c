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

/************************************************************************
*                                          *
*    Durand-Kerner-Aberth for Higher Order Equation   *
*         *
*     1988.12  K.Fukushi  *
*     1989.3  T.Kobayashi modified *
*     1999.12 T.Yoshimura modified *
*         *
* usage :        *
*   root_pol [ options ] [ infile ]>stdout  *
* options:       *
*  -m  : order of equation           [32]  *
*  -n  : number of maximum iteration    [1000] *
*  -e  : eps      [1.0e-14] *
*  -i  : a[0] = 1     [FALSE] *
*  -s  : revert the coefficient   [FALSE] *
*  -r  : ( radius, arg ) output    [(Real,Imag)] *
* Pn(x) = a[0]x^n + a[1]x^n-1 + ... + a[n-1]x + a[n]  *
* infile:        *
*   stdin for default     *
*   a[0],...,a[n]      *
* outfile:       *
*  x[0].re, x[1].re,...,x[n-1].re,    *
*  x[0].im, x[1].im,...,x[n-1].im    *
*         *
************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SPTK.h>


/* Default Values */
#define ORDER 32
#define ITER  1000
#define EPS   1.0e-14


/* Command Name */
char *cmnd;


int usage (void)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - Durand-Kerner-Aberth for higher order equation \n", cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ]>stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -m  : order of equation            [%d]\n",ORDER);
   fprintf(stderr, "       -n  : number of maximum iteration  [%d]\n",ITER);
   fprintf(stderr, "       -e  : eps                          [%1.1e]\n",EPS);
   fprintf(stderr, "       -i  : a[0] = 1                     [FALSE]\n");
   fprintf(stderr, "       -s  : revert the coefficient       [FALSE]\n");
   fprintf(stderr, "       -r  : (magnitude, arg) output      [(Real,Imag)]\n");
   fprintf(stderr, "       -h  : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       coefficients of polynomial (float) [stdin]\n");
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       root of polynomial (float)\n");
#ifdef SPTK_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n",SPTK_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(1);
}


int main (int argc,char *argv[])
{
   char *s;
   int c, k, odr=ORDER, itrat=ITER, form=0, a_zero=0;
   double *a, *d, eps=EPS;
   FILE *fp=stdin;
   complex *x;

   if ((cmnd=strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;

   while (--argc>0) {
      if (*(s = *++argv)=='-') {
         c = *++s;
         switch (c) {
         case 'i' :
            a_zero = 1;
            break;
         case 'r' :
            form |= 1;
            break;
         case 's' :
            form |= 2;
            break;
         case 'e' :
            if (*++s=='\0') {
               s = *++argv;
               --argc;
            }
            eps = atof(s);
            break;
         case 'm' :
            if (*++s=='\0') {
               s = *++argv;
               --argc;
            }
            odr = atoi(s);
            break;
         case 'n' :
            if (*++s=='\0') {
               s = *++argv;
               --argc;
            }
            itrat = atoi(s);
            break;
         default  :
            usage ();
         }
      }
      else {
         if ((fp=fopen(*argv, "r"))==NULL) {
            fprintf(stderr,"%s : cannot open %s !!\n", cmnd, *argv);
            exit(0);
         }
      }
   }

   a = dgetmem(odr+1);
   odr = freadf(a,sizeof(*a),odr,fp)-1;

   d = dgetmem(odr+1);
   x = cplx_getmem(odr+1);

   for (k=0; k<=odr; ++k) d[k] = a[k];

   root_pol(d, odr, x, a_zero, eps, itrat);
   output_root_pol(x, odr, form);

   return(0);
}
