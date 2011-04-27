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

/************************************************************************
*                                                                       *
*    Calculation of Root Mean Squared Error                             *
*                                                                       *
*                                     1996.3  K.Koishida                *
*                                                                       *
*       usage:                                                          *
*               rmse [ options ] file1 [ infile ] > stdout              *
*       options:                                                        *
*               -l l     :  frame length        [0]                     *
*       infile:                                                         *
*               data sequence                                           *
*                       , x(0), x(1), ..., x(l-1),                      *
*       stdout:                                                         *
*               rmse                                                    *
*                       , x(0)+x(1)...+x(l-1)/l                         *
*       notice:                                                         *
*               if l>0, calculate rmse frame by frame                   *
*       require:                                                        *
*               rmse()                                                  *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id: rmse.c,v 1.22 2011/04/27 13:46:43 mataki Exp $";


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

/*  Default Values  */
#define LENG 0

/*  Command Name  */
char *cmnd;


void usage(int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - calculation of root mean squared error\n", cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] file1 [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "      -l l  : frame length       [%d]\n", LENG);
   fprintf(stderr, "      -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "      data sequence (%s)      [stdin]\n", FORMAT);
   fprintf(stderr, "  file1:\n");
   fprintf(stderr, "      data sequence (%s)\n", FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "      root mean squared error (%s)\n", FORMAT);
   fprintf(stderr, "  notice:\n");
   fprintf(stderr, "      if l>0, calculate rmse frame by frame\n");
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
   int l = LENG, num = 0;
   FILE *fp = stdin, *fp1 = NULL;
   double *x, *y, x1, y1, sub, z = 0.0;

   if ((cmnd = strrchr(argv[0], '/')) == NULL)
      cmnd = argv[0];
   else
      cmnd++;
   while (--argc)
      if (**++argv == '-') {
         switch (*(*argv + 1)) {
         case 'l':
            l = atoi(*++argv);
            --argc;
            break;
         case 'h':
            usage(0);
         default:
            fprintf(stderr, "%s : Invalid option '%c'!\n", cmnd, *(*argv + 1));
            usage(1);
         }
      } else if (fp1 == NULL)
         fp1 = getfp(*argv, "rb");
      else
         fp = getfp(*argv, "rb");

   if (l > 0) {
      x = dgetmem(l + l);
      y = x + l;
      while (freadf(x, sizeof(*x), l, fp) == l &&
             freadf(y, sizeof(*y), l, fp1) == l) {
         z = rmse(x, y, l);
         fwritef(&z, sizeof(z), 1, stdout);
      }
   } else {
      while (freadf(&x1, sizeof(x1), 1, fp) == 1 &&
             freadf(&y1, sizeof(y1), 1, fp1) == 1) {
         sub = x1 - y1;
         z += sub * sub;

         num++;
      }
      z = sqrt(z /= num);
      fwritef(&z, sizeof(z), 1, stdout);
   }

   return (0);
}
