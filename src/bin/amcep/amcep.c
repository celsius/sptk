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
*                                                                       *
*    Adaptive Mel Cepstral Analysis                                     *
*                                                                       *
*    usage:*
*amcep [ options ] [ pefile ] < stdin > stdout*
*options:*
*-m m     :  order of mel cepstrum[25]*
*-a a     :  alpha[0.35]*
*-l l     :  leakage factor[0.98]*
*-k k     :  step size[0.1]*
*-t t     :  momentum constant[0.9]*
*-p p     :  output period of mel cepstrum[1]*
*-e e     :  minimum value for epsilon[FALSE]*
*-s       :  smoothed (average) mel cepstrum[FALSE]*
*-P P     :  order of pade approximation[4]*
*infile:*
*data sequence*
*, x(0), x(1), ...*
*stdout:*
*mel cepstrum*
*, c~(0), c~(1), ..., c~(M),*
*output:*
*prediction error (if pefile is specified)*
*, e(0), e(1), ...*
*note:*
*P = 4 or 5*
*require:*
*mlsadf()*
**
************************************************************************/

static char *rcs_id = "$Id: amcep.c,v 1.10 2007/09/10 02:48:32 heigazen Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SPTK.h>


/*  Default Values  */
#define ALPHA 0.35
#define ORDER 25
#define LAMBDA 0.98
#define TAU 0.9
#define STEP 0.1
#define PERIOD 1
#define AVEFLAG FA
#define PEFLAG FA
#define EPS 0.0
#define PADEORDER 4

char *BOOL[] = {"FALSE", "TRUE"};
#ifdef DOUBLE
char *FORMAT = "double";
#else
char *FORMAT = "float";
#endif /* DOUBLE */

/*  Command Name  */
char*cmnd;


void usage (int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - adaptive mel cepstral analysis\n",cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ pefile ] < stdin > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -m m  : order of mel cepstrum         [%d]\n", ORDER);
   fprintf(stderr, "       -a a  : all-pass constant             [%g]\n", ALPHA);
   fprintf(stderr, "       -l l  : leakage factor                [%g]\n", LAMBDA);
   fprintf(stderr, "       -t t  : momentum constant             [%g]\n", TAU);
   fprintf(stderr, "       -k k  : step size                     [%g]\n", STEP);
   fprintf(stderr, "       -p p  : output period of mel cepstrum [%d]\n", PERIOD);
   fprintf(stderr, "       -s    : output smoothed mel cepstrum  [%s]\n", BOOL[AVEFLAG]);
   fprintf(stderr, "       -e e  : minimum value for epsilon     [%g]\n", EPS);
   fprintf(stderr, "       -P P  : order of pade approximation   [%d]\n", PADEORDER);
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  stdin:\n");
   fprintf(stderr, "       data sequence (%s)\n", FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       mel-cepstrum (%s)\n", FORMAT);
   fprintf(stderr, "  pefile:\n");
   fprintf(stderr, "       prediction error (%s)\n", FORMAT);
   fprintf(stderr, "  note:\n");
   fprintf(stderr, "       P = 4 or 5\n");
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
   int m=ORDER, period=PERIOD, pd=PADEORDER, i, j;
   FILE *fp=stdin, *fpe=NULL;
   Boolean aveflag=AVEFLAG;
   double alpha=ALPHA, lambda=LAMBDA, tau=TAU, step=STEP, eps=EPS,
          *mc, *b, *ep, *e, *bb, *d, *avemc, 
          x, xx, ll, tt, gg, mu, ttx;

   if ((cmnd = strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;
   while (--argc)
      if (**++argv == '-') {
         switch (*(*argv+1)) {
         case 'a':
            alpha = atof(*++argv);
            --argc;
            break;
         case 'l':
            lambda = atof(*++argv);
            --argc;
            break;
         case 't':
            tau = atof(*++argv);
            --argc;
            break;
         case 'k':
            step = atof(*++argv);
            --argc;
            break;
         case 'm':
            m = atoi(*++argv);
            --argc;
            break;
         case 'p':
            period = atoi(*++argv);
            --argc;
            break;
         case 'P':
            pd = atoi(*++argv);
            --argc;
            break;
         case 's':
            aveflag = 1 - aveflag;
            break;
         case 'e':
            eps = atof(*++argv);
            --argc;
            break;
         case 'h':
            usage(0);
         default:
            fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
         usage(1);
         }
      }
      else 
         fpe = getfp(*argv, "w");

   if ((pd<4) || (pd>5)) {
      fprintf(stderr,"%s : Order of pade approximation is 4 or 5!\n",cmnd);
      exit(1);
   }

   mc = dgetmem(6*(m+1)+3*(pd+1)+pd*(m+2));
   b  = mc + m + 1;
   bb = b  + m + 1;
   e  = bb + m + 1;
   ep = e  + m + 1;
   avemc = ep + m + 1;
   d = avemc  + m + 1;

   j  = period;
   ll = 1.0 - lambda;
   gg = 1.0;
   tt = 2 * (1.0 - tau);
   step /= (double) m;
   xx = 0.0;
    
   while (freadf(&x, sizeof(x), 1, fp) == 1) {
      for (i=1; i<=m; i++) 
         bb[i] = -b[i];

      x = mlsadf(x, bb, m, alpha, pd, d);
      phidf(xx, m, alpha, e);
      xx = x;

      gg = gg * lambda + ll * x * x;
      gg = (gg<eps) ? eps : gg;
      b[0] = 0.5 * log(gg);

      mu = step / gg;
      ttx = tt * x;

      for (i=1; i<=m; i++) {
         ep[i] = tau * ep[i] - ttx * e[i];
         b[i] -= mu * ep[i];
      }

      b2mc(b, mc, m, alpha);

      if (aveflag)
         for (i=0; i<=m; i++)
            avemc[i] += mc[i];

      if (fpe != NULL)
         fwritef(&x, sizeof(x), 1, fpe);

      if (--j == 0) {
         j = period;
         if (aveflag) {
            for (i=0; i<=m; i++)
               avemc[i] /= period;
            fwritef(avemc, sizeof(*avemc), m+1, stdout);
            fillz(avemc, sizeof(*avemc), m+1);
         }
         else
            fwritef(mc, sizeof(*mc), m+1, stdout);
      }
   }
   exit(0);
}

