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
*    Transform Negative Derivative of Phase Spectrum (NDPS) to Cepstrum *
*                                                                       *
*                                       1986.9 K.Tokuda                 *
*                                       2014.1 T.Aritake                *
*                                                                       *
*       usage:                                                          *
*               ndps2c [ options ] [ infile ] > stdout                  *
*       options:                                                        *
*               -m M     :  order of cepstrum           [25]            *
*               -l L     :  FFT length                  [256]           *
*       stdin:                                                          *
*               Negative Derivative of Phase Spectrum (NDPS) (real)     *
*                   , n(0), n(1), ..., n(L/2)                           *
*       stdout:                                                         *
*               minimum phase cepstrum (real)                           *
*                   , c(0), c(1), ..., c(M),                            *
*                                                                       *
************************************************************************/

static char *rcs_id = "";

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

#include <ctype.h>
#include <math.h>

#if defined(WIN32)
#include "SPTK.h"
#else
#include <SPTK.h>
#endif

/*  Default Values  */
#define ORDER 25
#define FLENG 256

/*  Command Name  */
char *cmnd;

void usage(int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr,
           " %s - transform Negative Derivative of Phase Spectrum (NDPS) to cepstrum\n",
           cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -m M  : order of cepstrum        [%d]\n", ORDER);
   fprintf(stderr, "       -l L  : FFT length               [%d]\n", FLENG);
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr,
           "       Negative Derivative of Phase Spectrum (NDPS) (%s) [stdin]\n",
           FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       cepstrum (%s)\n", FORMAT);
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
   int m = ORDER, l = FLENG;
   FILE *fp = stdin;
   double *c, *n;

   if ((cmnd = strrchr(argv[0], '/')) == NULL)
      cmnd = argv[0];
   else
      cmnd++;
   while (--argc)
      if (**++argv == '-') {
         switch (*(*argv + 1)) {
         case 'm':
         case 'l':
            if (isdigit(**(argv + 1)) == 0) {
               if ((**(argv + 1)) != '+') {
                  fprintf(stderr,
                          "%s : %s option need positive value !\n", cmnd,
                          *argv);
                  usage(1);
               } else if (isdigit(*(*(argv + 1) + 1)) == 0) {
                  fprintf(stderr,
                          "%s : %s option need positive value !\n", cmnd,
                          *argv);
                  usage(1);
               }
            }
            if ((*(*argv + 1)) == 'm')
               m = atoi(*++argv);
            else if ((*(*argv + 1)) == 'l')
               l = atoi(*++argv);
            --argc;
            break;

         case 'h':
            usage(0);
         default:
            fprintf(stderr, "%s : Invalid option '%c'!\n", cmnd, *(*argv + 1));
            usage(1);
         }
      } else
         fp = getfp(*argv, "rb");


   c = dgetmem(m + 1);
   n = dgetmem(l);

   while (freadf(n, sizeof(*n), l / 2 + 1, fp) == l / 2 + 1) {
      ndps2c(n, l, c, m);
      fwritef(c, sizeof(*c), m + 1, stdout);
   }

   free(c);
   free(n);

   return (0);
}
