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

/*********************************************************************************************
*                                                                                            *
*    Delta Calculation                                                                       *
*                                                                                            *
*                                      2008.6 H.Zen                                          *
*       usage:                                                                               *
*               delta [ options ] [ infile ] > stdout                                        *
*       options:                                                                             *
*               -m M                   : order of vector                              [24]   *
*               -l L                   : length of vector                             [m+1]  *
*               -d fn                  : filename of delta coefficients               [N/A]  *
*               -t t                   : number of input vectors                      [EOF]  *
*               -d coef [coef...]      : delta coefficients                           [N/A]  *
*               -r n w1 [w2]           : number and width of regression coefficients  [N/A]  *
*               -R n Wf1 Wb1 [Wf2 Wb2] : number and width of regression coefficients  [N/A]  *
*               -M magic               : magic number                                 [N/A]  *
*       infile:                                                                              *
*              static feature sequence                                                       *
*                      x_1(1), ..., x_1(L), x_2(1), ..., x_2(L), x_3(1), ...                 *
*       stdout:                                                                              *
*              static and dynamic feature sequence                                           *
*                      x_1(1), ..., x_1(L), \Delta x_1(1), ..., \Delta x_1(L), ...           *
*                                                                                            *
**********************************************************************************************/

static char *rcs_id = "$Id: delta.c,v 1.8 2011/06/06 05:11:03 mataki Exp $";


/*  Standard C Libraries  */
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
#define MAGIC_NUM 0
#define MAGIC_FLAG FA

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
           "       -m M                   : order of vector                              [%d]\n",
           LENG - 1);
   fprintf(stderr,
           "       -l L                   : length of vector                             [m+1]\n");
   fprintf(stderr,
           "       -t T                   : number of input vectors                      [EOF]\n");
   fprintf(stderr,
           "       -d coef [coef...]      : delta coefficients                           [N/A]\n");
   fprintf(stderr,
           "       -r n t1 [t2]           : number and width of regression coefficients  [N/A]\n");
   fprintf(stderr,
           "       -R n Wf1 Wb1 [Wf2 Wb2] : number and width of regression coefficients  [N/A]\n");
   fprintf(stderr,
           "                                Combining -M option, magic number is skipped\n");
   fprintf(stderr,
           "                                during the delta calculation.\n");

   fprintf(stderr,
           "       -M magic               : magic number                                 [N/A]\n");
   fprintf(stderr,
           "                                valid only when -R option is specified.\n");
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

/* LU decomposition */
void LU(int n, double **A)
{
   int i, j, k;
   double q;

   for (k = 0; k < n - 1; k++) {
      for (i = k + 1; i < n; i++) {
         q = A[i][k] / A[k][k];
         for (j = k + 1; j < n; j++) {
            A[i][j] -= q * A[k][j];
         }
         A[i][k] = q;
      }
   }
}

/* solve linear equation Ax = b via LU decomposition */
void SOLVE(int n, double **A, double *b)
{
   int i, j;

   for (i = 0; i < n; i++) {
      for (j = 0; j < i; j++) {
         b[i] -= A[i][j] * b[j];
      }
   }

   for (i = n - 1; i >= 0; i--) {
      for (j = i + 1; j < n; j++) {
         b[i] -= A[i][j] * b[j];
      }
      b[i] /= A[i][i];
   }
}

/* calculate regression polynomial coefficients */
void GetCoefficient(double *input, double *outp, int dw_num,
                    int *position, int TOTAL, int total, int length,
                    int *win_size_forward, int *win_size_backward)
{
   int i, j, l, t, d, index, width;
   double T0, T1, T2, T3, T4, b[3];
   double **Matrix = (double **) malloc(sizeof(double *) * 3);
   double *tmpMat = dgetmem(3 * 3);
   for (i = 0, j = 0; i < 3; i++, j += 3) {
      Matrix[i] = tmpMat + j;
   }

   for (d = 0; d < dw_num - 1; d++) {
      for (t = 0; t < total; t++) {
         T0 = T1 = T2 = T3 = T4 = 0.0;
         for (i = -win_size_backward[d]; i <= win_size_forward[d]; i++) {
            index = t + i;
            if (index < 0 || index >= total) {
               width = i;
            } else {
               width = position[index] - position[t];
            }
            T0++;
            T1 += width;
            T2 += pow(width, 2);
            T3 += pow(width, 3);
            T4 += pow(width, 4);
         }
         Matrix[0][0] = T0;
         Matrix[0][1] = T1;
         Matrix[0][2] = T2;
         Matrix[1][0] = T1;
         Matrix[1][1] = T2;
         Matrix[1][2] = T3;
         Matrix[2][0] = T2;
         Matrix[2][1] = T3;
         Matrix[2][2] = T4;

         LU(3, Matrix);         /* LU decomposition */

         for (l = 0; l < length; l++) {
            b[0] = 0.0;
            b[1] = 0.0;
            b[2] = 0.0;
            for (i = -win_size_backward[d]; i <= win_size_forward[d]; i++) {
               index = t + i;
               if (index < 0 || index >= total) {
                  width = i;
               } else {
                  width = position[index] - position[t];
               }
               int tmp = position[t] + width;
               int pos = length * tmp + l;
               if (tmp >= TOTAL) {
                  pos = length * (TOTAL - 1) + l;
               } else if (tmp < 0) {
                  pos = l;
               }
               b[0] += input[pos];
               b[1] += width * input[pos];
               b[2] += pow(width, 2) * input[pos];
            }

            /* solve linear equation via LU decomposition */
            SOLVE(3, Matrix, b);

            if (d == 0) {
               outp[dw_num * length * t + l] = input[length * position[t] + l];
               outp[dw_num * length * t + length + l] = b[1];
            } else if (d == 1) {
               outp[dw_num * length * t + length * 2 + l] = 2 * b[2];
            }
         }
      }
   }
}

int main(int argc, char *argv[])
{
   FILE *fp = stdin, *fpc;
   char *coef = NULL;
   double *x = NULL, *dx = NULL, **dw_coef = NULL, magic = MAGIC_NUM;
   int i, j, l, d, t, tj, ispipe, fsize, leng = LENG, total = T;
   int dw_num = 1, **dw_width = NULL, dw_calccoef = -1, dw_coeflen = 1,
       dw_leng = 1;
   char **dw_fn = (char **) calloc(sizeof(char *), argc);
   Boolean MAGIC = MAGIC_FLAG;
   int win_pos, win_size_forward[2], win_size_backward[2];

   if ((cmnd = strrchr(argv[0], '/')) == NULL)
      cmnd = argv[0];
   else
      cmnd++;

   while (--argc) {
      if (**++argv == '-') {
         switch (*(*argv + 1)) {
         case 'd':
            if (dw_calccoef == 1 || dw_calccoef == 2) {
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
            if (dw_calccoef == 0 || dw_calccoef == 2) {
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
         case 'R':
            if (dw_calccoef == 0 || dw_calccoef == 1) {
               fprintf(stderr,
                       "%s : Options '-r', '-d' and '-R' should not be defined simultaneously!\n",
                       cmnd);
               return (1);
            }
            dw_calccoef = 2;
            dw_num = atoi(*++argv) + 1;
            --argc;
            if ((dw_num != 2) && (dw_num != 3)) {
               fprintf(stderr,
                       "%s : Number of delta parameter should be 1 or 2!\n",
                       cmnd);
               return (1);
            }
            if (argc <= 1) {
               fprintf(stderr,
                       "%s : Window size for delta-delta parameter required!\n",
                       cmnd);
               return (1);
            }

            sscanf(*++argv, "%d", &win_size_forward[0]);
            --argc;
            sscanf(*++argv, "%d", &win_size_backward[0]);
            --argc;
            if (dw_num > 2) {
               sscanf(*++argv, "%d", &win_size_forward[1]);
               --argc;
               sscanf(*++argv, "%d", &win_size_backward[1]);
               --argc;
            }
            break;
         case 'M':
            sscanf(*++argv, "%lf", &magic);
            MAGIC = TR;
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
                2 * ((double) (a0 * j * j - a1)) /
                ((double) (a2 * a0 - a1 * a1));
      }
   }

   /* allocate memory for input/output vectors */
   x = dgetmem(leng * total);
   dx = dgetmem(dw_num * leng * total);
   fillz(dx, sizeof(*x), dw_num * leng * total);

   /* read input vectors */
   freadf(x, sizeof(*x), total * leng, fp);

   if (dw_calccoef == 0 || dw_calccoef == 1) {
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

   } else if (dw_calccoef == 2) {
      int *position = (int *) malloc(sizeof(int) * total);

      /* skip magic number */
      if (MAGIC == TR) {
          for (t = 0, win_pos = 0; t < total; t++) {
              for (l = 0; l < leng; l++) {
                  if (x[leng * t + l] == magic) {
                      break;
                  }
              }
              if (l == leng) {
                  position[win_pos] = t;      /* non-magic number */
                  win_pos++;
              }
          }
      } else {
          for (t = 0; t < total; t++) {
              position[t] = t;
          }
          win_pos = total;
      }

      /* calculate delta and delta-delta */
      GetCoefficient(x, dx, dw_num, position, total, win_pos, leng,
                     win_size_forward, win_size_backward);

      /* output static, delta, delta-delta */
      fwritef(dx, sizeof(*dx), dw_num * win_pos * leng, stdout);
   }

   return (0);
}
