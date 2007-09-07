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
*         *
*    Transform Mel-Generalize Cepstrum to Spectrum   *
*         *
*     1996.4  K.Koishida  *
*         *
* usage:        *
*  mgc2sp [ options ] [ infile ]>stdout   *
* options:       *
*  -m  m    :  order of mel-genralized cepstrum    [25] *
*  -a  a    :  alpha     [0.0]  *
*  -g  g    :  gamma     [0.0]  *
*  -n       :  regard input as normalized cepstrum [FALSE] *
*  -u       :  regard input as multiplied by gamma [FALSE] *
*  -l  l    :  FFT length     [256]  *
*  -o  o    :  output format (see stdout)  [0]  *
*  -p       :  output phase   [FALSE] *
* infile:        *
*  mel-generalized cepstrum    *
*      , c(0), c(1), ..., c(m),    *
* stdout:        *
*         output format       scale    *
*         0 :       20 * log|H(z)|   *
*         1 :            ln|H(z)|    *
*         2 :            |H(z)|    *
*         (-p option is used)     *
*         0 :       arg|H(z)| / pi       [pi rad] *
*         1 :            arg|H(z)|            [rad] *
*         2 :            arg|H(z)| * 180 / pi [deg] *
*  spectrum      *
*      , s(0), s(1), ..., s(L/2),    *
* notice:        *
*  if g>1.0, g = -1 / g .    *
* require:       *
*  mgc2sp()      *
*         *
************************************************************************/

static char *rcs_id = "$Id: mgc2sp.c,v 1.9 2007/09/07 05:50:31 heigazen Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SPTK.h>


/*  Default Values  */
#define ORDER  25
#define ALPHA  0.0
#define GAMMA  0.0
#define NORM  FA
#define LENG  256
#define OTYPE  0
#define PHASE  FA
#define MULG  FA

char *BOOL[] = {"FALSE", "TRUE"};

/*  Command Name  */
char *cmnd;


void usage (int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - transform mel-generalized cepstrum to spectrum\n",cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ]>stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -a a  : alpha                               [%g]\n", ALPHA);
   fprintf(stderr, "       -g g  : gamma                               [%g]\n", GAMMA);
   fprintf(stderr, "       -m m  : order of mel-genralized cepstrum    [%d]\n", ORDER);
   fprintf(stderr, "       -n    : regard input as normalized cepstrum [%s]\n", BOOL[NORM]);
   fprintf(stderr, "       -u    : regard input as multiplied by gamma [%s]\n", BOOL[MULG]);
   fprintf(stderr, "       -l l  : FFT length                          [%d]\n", LENG);
   fprintf(stderr, "       -p    : output phase                        [%s]\n", BOOL[PHASE]);
   fprintf(stderr, "       -o o  : output format                       [%d]\n", OTYPE);
   fprintf(stderr, "                 0 (20*log|H(z)|)\n");
   fprintf(stderr, "                 1 (ln|H(z)|)\n");
   fprintf(stderr, "                 2 (|H(z)|)\n");
   fprintf(stderr, "             -p option is specified\n");
   fprintf(stderr, "                 0 (arg|H(z)|/pi     [pi rad])\n");
   fprintf(stderr, "                 1 (arg|H(z)|        [rad])\n");
   fprintf(stderr, "                 2 (arg|H(z)|*180/pi [deg])\n");
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       mel-generalized cepstrum (float)            [stdin]\n");
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       spectrum (float)\n");
   fprintf(stderr, "  notice:\n");
   fprintf(stderr, "       if g>1.0, g = -1 / g .\n");
#ifdef SPTK_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n",SPTK_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(status);
}

int main (int argc, char **argv)
{
   int m=ORDER, l=LENG, otype=OTYPE, no, i;
   double alpha=ALPHA, gamma=GAMMA, *c, *x, *y, logk;
   Boolean norm=NORM, phase=PHASE, mulg=MULG;
   FILE *fp=stdin;

   if ((cmnd=strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;
   while (--argc)
      if (**++argv=='-') {
         switch (*(*argv+1)) {
         case 'm':
            m = atoi(*++argv);
            --argc;
            break;
         case 'a':
            alpha = atof(*++argv);
            --argc;
            break;
         case 'g':
            gamma = atof(*++argv);
            --argc;
            if (gamma>1.0) gamma = -1.0 / gamma;
            break;
         case 'n':
            norm = 1 - norm;
            break;
         case 'u':
            mulg = 1 - mulg;
            break;
         case 'l':
            l = atoi(*++argv);
            --argc;
            break;
         case 'o':
            otype = atoi(*++argv);
            --argc;
            break;
         case 'p':
            phase = 1 - phase;
            break;
         case 'h':
            usage(0);
         default:
            fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
            usage(1);
         }
      }
      else
         fp = getfp(*argv, "r");

   x = dgetmem(l+l+m+1);
   y = x + l;
   c = y + l;

   no = l / 2 + 1;
   logk = 20.0 / log(10.0);

   while (freadf(c, sizeof(*c), m+1, fp)==m+1) {
      if (norm)
         ignorm(c, c, m, gamma);
      else if (mulg) {
         if (gamma==0) {
            fprintf(stderr, "%s : gamma for input mgc coefficients should not equal to 0 if you specify -u option!\n", cmnd);
            usage(1);
         }
         c[0] = (c[0] - 1.0) / gamma;
      }

      if (mulg) {
         if (gamma==0) {
            fprintf(stderr, "%s : gamma for input mgc coefficients should not equal to 0 if you specify -u option!\n", cmnd);
            usage(1);
         }
         for (i=m;i>0;i--)
            c[i] /= gamma;
      }

      mgc2sp(c, m, alpha, gamma, x, y, l);

      if (phase)
         switch (otype) {
         case 1 :
            for (i=no; i--;) x[i] = y[i];
            break;
         case 2 :
            for (i=no; i--;)
               x[i] = y[i] * 180 / PI;
            break;
         default :
            for (i=no; i--;)
               x[i] = y[i] / PI;
            break;
         }
      else
         switch (otype) {
         case 1 :
            break;
         case 2 :
            for (i=no; i--;)
               x[i] = exp(x[i]);
            break;
         default :
            for (i = no; i--;)
               x[i] *= logk;
            break;
         }

      fwritef(x, sizeof(*x), no, stdout);
   }
   
   return(0);
}
