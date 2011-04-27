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

/****************************************************************************************
*                                                                                       *
*    Delta Calculation                                                                  *
*                                                                                       *
*                                      2008.6 H.Zen                                     *
*       usage:                                                                          *
*               delta [ options ] [ infile ] > stdout                                   *
*       options:                                                                        *
*               -m M              : order of vector                              [24]   *
*               -l L              : length of vector                             [m+1]  *
*               -d fn             : filename of delta coefficients               [N/A]  *
*               -t t              : number of input vectors                      [EOF]  *
*               -d coef [coef...] : delta coefficients                           [N/A]  *
*               -r n w1 [w2]      : number and width of regression coefficients  [N/A]  *
*       infile:                                                                         *
*              static feature sequence                                                  *
*                      x_1(1), ..., x_1(L), x_2(1), ..., x_2(L), x_3(1), ...            *
*       stdout:                                                                         *
*              static and dynamic feature sequence                                      *
*                      x_1(1), ..., x_1(L), \Delta x_1(1), ..., \Delta x_1(L), ...      *
*                                                                                       *
****************************************************************************************/

static char *rcs_id = "$Id: delta.c,v 1.7 2011/04/27 13:46:38 mataki Exp $";


/*  Standard C Libralies  */
#include <stdio.h>

#ifdef HAVE_STRING_H
#  include <string.h>
#else
#  include <strings.h>
#  ifndef HAVE_STRRCHR
#     define strrchr rindex
#  endif
#endif

#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#if defined(WIN32)
#  include "SPTK.h"
#else
#  include <SPTK.h>
#endif

/*  Default Values  */
#define  LENG  25
#define  T     -1

char *BOOL[] = { "FALSE", "TRUE" };

/*  Command Name  */
char *cmnd;


/*  Other Definitions  */
#ifdef DOUBLE
typedef double real;
#else
typedef float real;
#endif


void usage(int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - delta calculation\n", cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr,
           "       -m M              : order of vector                              [%d]\n",
           LENG - 1);
   fprintf(stderr,
           "       -l L              : length of vector                             [m+1]\n");
   fprintf(stderr,
           "       -t T              : number of input vectors                      [EOF]\n");
   fprintf(stderr,
           "       -d coef [coef...] : delta coefficients                           [N/A]\n");
   fprintf(stderr,
           "       -r n t1 [t2]      : number and width of regression coefficients  [N/A]\n");
   fprintf(stderr, "       -h     : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr,
           "       static feature vectors                                           [stdin]\n");
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       static and dynamic feature vectors\n");
#ifdef PACKAGE_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n", PACKAGE_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(status);
}


int main(int argc, char *argv[])
{
   FILE *fp = stdin, *fpc;
   char *coef = NULL;
   double *x = NULL, *dx = NULL, **dw_coef = NULL;
   int i, j, l, d, t, tj, ispipe, fsize, leng = LENG, total = T;
   int dw_num = 1, **dw_width = NULL, dw_calccoef = -1, dw_coeflen =
       1, dw_maxw[2] = { 0, 0 }, dw_leng = 1;
   char **dw_fn = (char **) calloc(sizeof(char *), argc);

   if ((cmnd = strrchr(argv[0], '/')) == NULL)
      cmnd = argv[0];
   else
      cmnd++;

   while (--argc) {
      if (**++argv == '-') {
         switch (*(*argv + 1)) {
         case 'd':
            if (dw_calccoef == 1) {
               fprintf(stderr,
                       "%s : Options '-r' and '-d' should not be defined simultaneously!\n",
                       cmnd);
               return (1);
            }
            dw_calccoef = 0;
            if (isfloat(*++argv)) {
               dw_coeflen = 0;
               for (i = 0; (i < argc - 1) && isfloat(argv[i]); i++) {
                  dw_coeflen += strlen(argv[i]) + 1;
               }
               dw_coeflen += 1;
               coef = dw_fn[dw_num] = getmem(dw_coeflen, sizeof(char));
               for (j = 0; j < i; j++) {
                  sprintf(coef, " %s", *argv);
                  coef += strlen(*argv) + 1;
                  if (j < i - 1) {
                     argv++;
                     argc--;
                  }
               }
            } else {
               dw_fn[dw_num] = *argv;
            }
            dw_num++;
            --argc;
            break;
         case 'r':
            if (dw_calccoef == 0) {
               fprintf(stderr,
                       "%s : Options '-r' and '-d' should not be defined simultaneously!\n",
                       cmnd);
               return (1);
            }
            dw_calccoef = 1;
            dw_coeflen = atoi(*++argv);
            --argc;
            if ((dw_coeflen != 1) && (dw_coeflen != 2)) {
               fprintf(stderr,
                       "%s : Number of delta parameter should be 1 or 2!\n",
                       cmnd);
               return (1);
            }
            if (argc <= 1) {
               fprintf(stderr,
                       "%s : Window size for delta parameter required!\n",
                       cmnd);
               return (1);
            }
            dw_fn[dw_num] = *++argv;
            dw_num++;
            --argc;
            if (dw_coeflen == 2) {
               if (argc <= 1) {
                  fprintf(stderr,
                          "%s : Window size for delta-delta parameter required!\n",
                          cmnd);
                  return (1);
               }
               dw_fn[dw_num] = *++argv;
               dw_num++;
               --argc;
            }
            break;
         case 'm':
            leng = atoi(*++argv) + 1;
            --argc;
            break;
         case 'l':
            leng = atoi(*++argv);
            --argc;
            break;
         case 't':
            total = atoi(*++argv);
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
   }

   /* -- Count number of input vectors -- */
   if (total == -1) {
      ispipe = fseek(fp, 0L, 2);
      total = (int) (ftell(fp) / (double) leng / (double) sizeof(float));
      rewind(fp);

      if (ispipe == -1) {       /* input vectors is from standard input via pipe */
         fprintf(stderr,
                 "\n %s (Error) -t option must be specified for the standard input via pipe.\n",
                 cmnd);
         usage(1);
      }
   }


   /* parse window files */
   /* memory allocation */
   if ((dw_width = (int **) calloc(dw_num, sizeof(int *))) == NULL) {
      fprintf(stderr, "%s : Cannot allocate memory!\n", cmnd);
      exit(1);
   }
   for (i = 0; i < dw_num; i++)
      if ((dw_width[i] = (int *) calloc(2, sizeof(int))) == NULL) {
         fprintf(stderr, "%s : Cannot allocate memory!\n", cmnd);
         exit(1);
      }
   if ((dw_coef = (double **) calloc(dw_num, sizeof(double *))) == NULL) {
      fprintf(stderr, "%s : Cannot allocate memory!\n", cmnd);
      exit(1);
   }

   /* window for static parameter */
   dw_width[0][0] = dw_width[0][1] = 0;
   dw_coef[0] = dgetmem(1);
   dw_coef[0][0] = 1;

   /* set delta coefficients */
   if (dw_calccoef == 0) {
      for (i = 1; i < dw_num; i++) {
         if (dw_fn[i][0] == ' ') {
            fsize = str2darray(dw_fn[i], &(dw_coef[i]));
         } else {
            /* read from file */
            fpc = getfp(dw_fn[i], "rb");

            /* check the number of coefficients */
            fseek(fpc, 0L, 2);
            fsize = ftell(fpc) / sizeof(real);
            fseek(fpc, 0L, 0);

            /* read coefficients */
            dw_coef[i] = dgetmem(fsize);
            freadf(dw_coef[i], sizeof(**(dw_coef)), fsize, fpc);
         }

         /* set pointer */
         dw_leng = fsize / 2;
         dw_coef[i] += dw_leng;
         dw_width[i][0] = -dw_leng;
         dw_width[i][1] = dw_leng;
         if (fsize % 2 == 0)
            dw_width[i][1]--;
      }
   } else if (dw_calccoef == 1) {
      int a0, a1, a2;
      for (i = 1; i < dw_num; i++) {
         dw_leng = atoi(dw_fn[i]);
         if (dw_leng < 1) {
            fprintf(stderr,
                    "%s : Width for regression coefficient shuould be more than 1!\n",
                    cmnd);
            exit(1);
         }
         dw_width[i][0] = -dw_leng;
         dw_width[i][1] = dw_leng;
         dw_coef[i] = dgetmem(dw_leng * 2 + 1);
         dw_coef[i] += dw_leng;
      }

      dw_leng = atoi(dw_fn[1]);
      for (a1 = 0, j = -dw_leng; j <= dw_leng; a1 += j * j, j++);
      for (j = -dw_leng; j <= dw_leng; j++)
         dw_coef[1][j] = (double) j / (double) a1;

      if (dw_num > 2) {
         dw_leng = atoi(dw_fn[2]);
         for (a0 = a1 = a2 = 0, j = -dw_leng; j <= dw_leng;
              a0++, a1 += j * j, a2 += j * j * j * j, j++);
         for (j = -dw_leng; j <= dw_leng; j++)
            dw_coef[2][j] =
                ((double) (a0 * j * j - a1)) / ((double) (a2 * a0 - a1 * a1)) /
                2;
      }
   }

   /* max window width */
   dw_maxw[0] = dw_maxw[1] = 0;
   for (i = 0; i < dw_num; i++) {
      if (dw_maxw[0] > dw_width[i][0])
         dw_maxw[0] = dw_width[i][0];
      if (dw_maxw[1] < dw_width[i][1])
         dw_maxw[1] = dw_width[i][1];
   }

   /* allocate memory for input/output vectors */
   x = dgetmem(leng * total);
   dx = dgetmem(dw_num * leng * total);
   fillz(dx, sizeof(*x), dw_num * leng * total);

   /* read input vectors */
   freadf(x, sizeof(*x), total * leng, fp);

   /* calculate delta and delta-delta */
   for (t = 0; t < total; t++) {
      for (d = 0; d < dw_num; d++) {
         for (j = dw_width[d][0]; j <= dw_width[d][1]; j++) {
            tj = t + j;
            if (tj < 0)
               tj = 0;
            if (!(tj < total))
               tj = total - 1;
            for (l = 0; l < leng; l++)
               dx[dw_num * leng * t + leng * d + l] +=
                   dw_coef[d][j] * x[leng * tj + l];
         }
      }
   }

   /* output static, delta, delta-delta */
   fwritef(dx, sizeof(*dx), dw_num * total * leng, stdout);

   return (0);
}
