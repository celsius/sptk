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
/*                1996-2011  Nagoya Institute of Technology          */
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

/*************************************************************************
 *                                                                       *
 *    Calculation of GMM log-probagility                                 *
 *                                                                       *
 *                                       2000.7  C. Miyajima             *
 *                                                                       *
 *       usage:                                                          *
 *               gmmp [options] gmmfile [infile] > stdout                *
 *       options:                                                        *
 *               -l l  :  length of vector                    [26]       *
 *               -m m  :  number of Gaussian components       [16]       *
 *               -a    :  output average log-probability      [FALSE]    *
 *       infile:                                                         *
 *               input vector sequence                        [stdin]    *
 *       stdout:                                                         *
 *               sequence of frame log-probabilities                     *
 *               average log-probability (if -a is used)                 *
 *                                                                       *
 ************************************************************************/

static char *rcs_id = "$Id: gmmp.c,v 1.5 2011/04/27 13:46:40 mataki Exp $";

/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>

#ifdef HAVE_STRING_H
#  include <string.h>
#else
#  include <strings.h>
#  ifndef HAVE_STRRCHR
#     define strrchr rindex
#  endif
#endif

#include <math.h>

#if defined(WIN32)
#  include "SPTK.h"
#else
#  include <SPTK.h>
#endif

#include "gmm.h"

/*  Default Values  */

#define DEF_L  26
#define DEF_M  16
#define DEF_A  FA

char *BOOL[] = { "FALSE", "TRUE" };

/*  Command Name  */
char *cmnd;

void usage(int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - Calculaton of GMM log-probability\n", cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] gmmfile [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");

   fprintf(stderr, "       -l l  : dimensionality of vectors          [%d]\n",
           DEF_L);
   fprintf(stderr, "       -m m  : number of Gaussian components      [%d]\n",
           DEF_M);
   fprintf(stderr, "       -a    : output average log-probability     [%s]\n",
           BOOL[DEF_A]);
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr,
           "       input data sequence (float)                [stdin]\n");
   fprintf(stderr, "  gmmfile:\n");
   fprintf(stderr, "       GMM parameters (float)\n");
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr,
           "       log-probabilities or average log-probability (float)\n");
   fprintf(stderr, "\n");
   exit(status);
}

int main(int argc, char **argv)
{
   FILE *fp = stdin, *fgmm = NULL;
   GMM gmm;
   double *dat, logwgd, *logp, ave_logp;
   int m, l, t, M = DEF_M, L = DEF_L, T;
   Boolean aflag = DEF_A;

   if ((cmnd = strrchr(argv[0], '/')) == NULL)
      cmnd = argv[0];
   else
      cmnd++;


   while (--argc)
      if (**++argv == '-') {
         switch (*(*argv + 1)) {
         case 'h':
            usage(0);
            break;
         case 'l':
            L = atoi(*++argv);
            --argc;
            break;
         case 'm':
            M = atoi(*++argv);
            --argc;
            break;
         case 'a':
            aflag = TR;
            break;
         default:
            fprintf(stderr, "%s: Illegal option \"%s\".\n", cmnd, *argv);
            usage(1);
         }
      } else if (fgmm == NULL)
         fgmm = getfp(*argv, "rb");
      else
         fp = getfp(*argv, "rb");


   /* Read GMM parameters */
   if (fgmm == NULL) {
      fprintf(stderr, "%s : GMM file name required !\n", cmnd);
      usage(1);
   }

   gmm.weight = dgetmem(M);
   gmm.gauss = (Gauss *) getmem(M, sizeof(Gauss));

   for (m = 0; m < M; m++) {
      gmm.gauss[m].mean = dgetmem(L);
      gmm.gauss[m].var = dgetmem(L);
   }

   freadf(gmm.weight, sizeof(double), M, fgmm);
   for (m = 0; m < M; m++) {
      freadf(gmm.gauss[m].mean, sizeof(double), L, fgmm);
      freadf(gmm.gauss[m].var, sizeof(double), L, fgmm);
   }
   fclose(fgmm);

   for (m = 0; m < M; m++)
      gmm.gauss[m].gconst = cal_gconst(gmm.gauss[m].var, L);


   /* Read data */
   fseek(fp, 0, 2);
   T = (int) ((double) ftell(fp)
              / (double) sizeof(float) / (double) L);
   rewind(fp);

   if (!T) {
      fprintf(stderr, "%s: No input data !\n", cmnd);
      usage(1);
   }

   dat = dgetmem(T * L);
   logp = dgetmem(T);

   freadf(dat, sizeof(double), T * L, fp);
   fclose(fp);


   /* Calculation of log-probability */
   for (t = 0, ave_logp = 0.0; t < T; t++, dat += L) {
      logp[t] = log_outp(&gmm, dat, M, L);
      ave_logp += logp[t];
   }
   ave_logp /= (double) T;


   /* Output log-probability */

   if (aflag)
      fwritef(&ave_logp, sizeof(double), 1, stdout);
   else
      fwritef(logp, sizeof(double), T, stdout);

   return (0);
}
