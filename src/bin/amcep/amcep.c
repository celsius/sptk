/* ----------------------------------------------------------------- */
/*             The Speech Signal Processing Toolkit (SPTK)           */
/*             developed by SPTK Working Group                       */
/*             http://sp-tk.sourceforge.net/                         */
/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 1984-2007  Tokyo Institute of Technology           */
/*                           Interdisciplinary Graduate School of    */
/*                           Science and Engineering                 */
/*                                                                   */
/*                1996-2017  Nagoya Institute of Technology          */
/*                           Department of Computer Science          */
/*                                                                   */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/* - Redistributions of source code must retain the above copyright  */
/*   notice, this list of conditions and the following disclaimer.   */
/* - Redistributions in binary form must reproduce the above         */
/*   copyright notice, this list of conditions and the following     */
/*   disclaimer in the documentation and/or other materials provided */
/*   with the distribution.                                          */
/* - Neither the name of the SPTK working group nor the names of its */
/*   contributors may be used to endorse or promote products derived */
/*   from this software without specific prior written permission.   */
/*                                                                   */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS */
/* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          */
/* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY    */
/* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           */
/* POSSIBILITY OF SUCH DAMAGE.                                       */
/* ----------------------------------------------------------------- */

/************************************************************************
*                                                                       *
*    Adaptive Mel Cepstral Analysis                                     *
*                                                                       *
*       usage:                                                          *
*               amcep [ options ] [ pefile ] < stdin > stdout           * 
*       options:                                                        *
*               -m m     :  order of mel cepstrum            [25]       *
*               -a a     :  all-pass constant                [0.35]     *
*               -l l     :  leakage factor                   [0.98]     * 
*               -t t     :  momentum constant                [0.9]      *
*               -k k     :  step size                        [0.1]      *
*               -p p     :  output period of mel cepstrum    [1]        *
*               -s       :  output smoothed mel cepstrum     [FALSE]    *
*               -e e     :  minimum value for epsilon        [0]        *
*               -P P     :  order of Pade approximation      [4]        *
*       infile:                                                         *
*               data sequence                                           *
*                   , x(0), x(1), ...                                   *
*       stdout:                                                         *
*               mel cepstrum                                            *
*                   , c~(0), c~(1), ..., c~(M),                         *
*       output:                                                         *
*               prediction error (if pefile is specified)               *
*                   , e(0), e(1), ...                                   *
*       notice:                                                         *
*               P = 4, 5, 6, or 7                                       *
*       require:                                                        *  
*               mlsadf()                                                *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>

#ifdef HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#ifndef HAVE_STRRCHR
#define strrchr rindex
#endif
#endif

#include <math.h>

#if defined(WIN32)
#include "SPTK.h"
#else
#include <SPTK.h>
#endif

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

char *BOOL[] = { "FALSE", "TRUE" };

/*  Command Name  */
char *cmnd;


void usage(int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - adaptive mel cepstral analysis\n", cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ pefile ] < stdin > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -m m  : order of mel cepstrum         [%d]\n",
           ORDER);
   fprintf(stderr, "       -a a  : all-pass constant             [%g]\n",
           ALPHA);
   fprintf(stderr, "       -l l  : leakage factor                [%g]\n",
           LAMBDA);
   fprintf(stderr, "       -t t  : momentum constant             [%g]\n", TAU);
   fprintf(stderr, "       -k k  : step size                     [%g]\n", STEP);
   fprintf(stderr, "       -p p  : output period of mel cepstrum [%d]\n",
           PERIOD);
   fprintf(stderr, "       -s    : output smoothed mel cepstrum  [%s]\n",
           BOOL[AVEFLAG]);
   fprintf(stderr, "       -e e  : minimum value for epsilon     [%g]\n", EPS);
   fprintf(stderr, "       -P P  : order of Pade approximation   [%d]\n",
           PADEORDER);
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  stdin:\n");
   fprintf(stderr, "       data sequence (%s)\n", FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       mel-cepstrum (%s)\n", FORMAT);
   fprintf(stderr, "  pefile:\n");
   fprintf(stderr, "       prediction error (%s)\n", FORMAT);
   fprintf(stderr, "  notice:\n");
   fprintf(stderr, "       P = 4, 5, 6, or 7\n");
#ifdef PACKAGE_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n", PACKAGE_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(status);
}


int main(int argc, char **argv)
{
   int m = ORDER, period = PERIOD, pd = PADEORDER, i, j;
   FILE *fp = stdin, *fpe = NULL;
   Boolean aveflag = AVEFLAG;
   double alpha = ALPHA, lambda = LAMBDA, tau = TAU, step = STEP, eps = EPS,
       *mc, *b, *avemc, x, mu;

   if ((cmnd = strrchr(argv[0], '/')) == NULL)
      cmnd = argv[0];
   else
      cmnd++;
   while (--argc)
      if (**++argv == '-') {
         switch (*(*argv + 1)) {
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
            fprintf(stderr, "%s : Invalid option '%c'!\n", cmnd, *(*argv + 1));
            usage(1);
         }
      } else
         fpe = getfp(*argv, "wb");

   if ((pd < 4) || (pd > 7)) {
      fprintf(stderr, "%s : Order of Pade approximation should be an integer in the rage of 4 to 7!\n",
              cmnd);
      return (1);
   }

   mc = dgetmem(2 * (m + 1) + 3 * (pd + 1) + pd * (m + 2));
   b = mc + m + 1;
   avemc = b + m + 1;

   j = period;

   while (freadf(&x, sizeof(x), 1, fp) == 1) {

      x = amcep(x, b, m, alpha, lambda, step, tau, pd, eps);

      b2mc(b, mc, m, alpha);

      if (aveflag)
         for (i = 0; i <= m; i++)
            avemc[i] += mc[i];

      if (fpe != NULL)
         fwritef(&x, sizeof(x), 1, fpe);

      if (--j == 0) {
         j = period;
         if (aveflag) {
            for (i = 0; i <= m; i++)
               avemc[i] /= period;
            fwritef(avemc, sizeof(*avemc), m + 1, stdout);
            fillz(avemc, sizeof(*avemc), m + 1);
         } else
            fwritef(mc, sizeof(*mc), m + 1, stdout);
      }
   }
   return (0);
}
