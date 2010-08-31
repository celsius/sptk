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
/*                1996-2010  Nagoya Institute of Technology          */
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
*                                                                        *
*    Data Type Transformation                                            *
*                                                                        *
*                                 1985.12 K.Tokuda                       *
*                                 1996.5  K.Koishida                     *
*                                 2010.3  A.Tamamori                     *
*                                 2010.9  T.Sawada                       *
*                                                                        *
*       usage:                                                           *
*               x2x [options] [infile] > stdout                          *
*       options:                                                         *
*               +type1   :  input data type                   [f]        *
*               +type2   :  output data type                  [type1]    *
*                           c (char)           C  (unsigned char)        *
*                           s (short)          S  (unsigned short)       *
*                           i (int)            I  (unsigned int)         *
*                           i3 (int, 3byte)    I3 (unsigned int, 3byte)  *
*                           l (long)           L  (unsigned long)        *
*                           f (float)          d  (double)               *
*                           a (ascii)                                    *
*               +aN      :  specify the column number N       [1]        *
*               -r       :  specify rounding off when a real number      *
*                           is substituted for a integer      [FALSE]    *
*               -o       :  clip by minimum and maximum of               *
*                           output type if input data is                 *
*                           over the range of output type     [FALSE]    *
*               -w W     :  type of warning                   [0]        *
*                           when input data is over the range            *
*                           of output  type                              *
*                             0 no-warning message                       *
*                             1 output warning message                   * 
*                             2 interrupt execute with warning message   *
*               %format  :  specify output format similar to  [%g]       *
*                           "printf()".                                  *
*                           if type2 is ascii.                           *
*                                                                        *
*************************************************************************/

static char *rcs_id = "$Id$";


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

#if defined(WIN32)
#  include "SPTK.h"
#else
#  include <SPTK.h>
#endif

#include <limits.h>
#include <float.h>
/*  Default Values  */
#define ROUND      FA
#define CLIP       FA
#define COL        1
#define WARN       0
#define FORM_INT   "%d"
#define FORM_FLOAT "%g"
#define INT3_MAX   8388607
#define INT3_MIN   -8388608
#define UINT3_MAX  16777215
char *BOOL[] = { "FALSE", "TRUE" };

/*  Command Name  */
char *cmnd;


void usage(int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - data type transformation\n", cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr,
           "       +type1  : input data type                             [f]\n");
   fprintf(stderr,
           "       +type2  : output data type                            [type1]\n");
   fprintf(stderr, "                 c  (char)           C  (unsigned char)\n");
   fprintf(stderr,
           "                 s  (short)          S  (unsigned short)\n");
   fprintf(stderr, "                 i  (int)            I  (unsigned int)\n");
   fprintf(stderr,
           "                 i3 (int, 3byte)     I3 (unsigned int, 3byte)\n");
   fprintf(stderr, "                 l  (long)           L  (unsigned long)\n");
   fprintf(stderr, "                 f  (float)          d  (double)\n");
   fprintf(stderr, "                 a  (ascii)\n");
   fprintf(stderr,
           "       +aN     : specify the column number N                 [%d]\n",
           COL);
   fprintf(stderr,
           "       -r      : specify rounding off when a real number\n");
   fprintf(stderr,
           "                 is substituted for a integer                [%s]\n",
           BOOL[ROUND]);
   fprintf(stderr, 
           "       -o     : clip by minimum and maximum of output type       \n");
   fprintf(stderr,
           "                if input data is over the range of               \n"); 
   fprintf(stderr,
           "                output type                                  [%s]\n",
           BOOL[CLIP]);
   fprintf(stderr, 
           "       -w W   : type of warning                                  \n");
   fprintf(stderr, 
           "                when input data is over the range of             \n");
   fprintf(stderr,
           "                output type                                  [%d]\n",
           WARN);
   fprintf(stderr, "                 0 no-warning message                    \n");
   fprintf(stderr, "                 1 output warning message                \n");
   fprintf(stderr, "                 2 interrupt execute with warning message\n");
   fprintf(stderr,
           "       %%format : specify output format similar to 'printf()', \n");
   fprintf(stderr,
           "                 if type2 is ascii.                          [%%g]\n");
   fprintf(stderr, "       -h      : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr,
           "       data sequence                                    [stdin]\n");
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       transformed data sequence\n");
#ifdef PACKAGE_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n", PACKAGE_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(status);
}

double r = 0.0;

int main(int argc, char **argv)
{
   char c1 = 'f', c2 = 'f', *form = FORM_FLOAT;
   double x;
   size_t size1 = 0, size2 = 0;
   int i = 1, col = COL;
   int warn = WARN;
   FILE *fp = stdin;
   Boolean round = ROUND, clip = CLIP;
   void x2x(void *x1, void *x2, char c1, char c2, int clip, int warn);

   if ((cmnd = strrchr(argv[0], '/')) == NULL)
      cmnd = argv[0];
   else
      cmnd++;
   while (--argc)
      if (**++argv == '+') {
         (*argv)++;
         while (**argv != '\0') {
            switch (**argv) {
            case 's':
               if (size1 == 0) {
                  c1 = 's';
                  size1 = sizeof(short);
               } else {
                  c2 = 's';
                  size2 = sizeof(short);
               }
               break;
            case 'S':
               if (size1 == 0) {
                  c1 = 'S';
                  size1 = sizeof(unsigned short);
               } else {
                  c2 = 'S';
                  size2 = sizeof(unsigned short);
               }
               break;
            case 'i':
               if (*(*argv + 1) == '3') {
                  if (size1 == 0) {
                     c1 = 't';
                     size1 = 3;
                  } else {
                     c2 = 't';
                     size2 = 3;
                  }
                  (*argv)++;
               } else {
                  if (size1 == 0) {
                     c1 = 'i';
                     size1 = sizeof(int);
                  } else {
                     c2 = 'i';
                     size2 = sizeof(int);
                  }
               }
               break;
            case 'I':
               if (*(*argv + 1) == '3') {
                  if (size1 == 0) {
                     c1 = 'T';
                     size1 = 3;
                  } else {
                     c2 = 'T';
                     size2 = 3;
                  }
                  (*argv)++;
               } else {
                  if (size1 == 0) {
                     c1 = 'I';
                     size1 = sizeof(unsigned int);
                  } else {
                     c2 = 'I';
                     size2 = sizeof(unsigned int);
                  }
               }
               break;
            case 'l':
               if (size1 == 0) {
                  c1 = 'l';
                  size1 = sizeof(long);
               } else {
                  c2 = 'l';
                  size2 = sizeof(long);
               }
               break;
            case 'L':
               if (size1 == 0) {
                  c1 = 'L';
                  size1 = sizeof(unsigned long);
               } else {
                  c2 = 'L';
                  size2 = sizeof(unsigned long);
               }
               break;
            case 'f':
               if (size1 == 0) {
                  c1 = 'f';
                  size1 = sizeof(float);
               } else {
                  c2 = 'f';
                  size2 = sizeof(float);
               }
               break;
            case 'd':
               if (size1 == 0) {
                  c1 = 'd';
                  size1 = sizeof(double);
               } else {
                  c2 = 'd';
                  size2 = sizeof(double);
               }
               break;
            case 'c':
               if (size1 == 0) {
                  c1 = 'c';
                  size1 = sizeof(char);
               } else {
                  c2 = 'c';
                  size2 = sizeof(char);
               }
               break;
            case 'C':
               if (size1 == 0) {
                  c1 = 'C';
                  size1 = sizeof(unsigned char);
               } else {
                  c2 = 'C';
                  size2 = sizeof(unsigned char);
               }
               break;
            case 'a':
               if (size1 == 0) {
                  c1 = 'a';
                  size1 = -1;
               } else {
                  c2 = 'a';
                  size2 = -1;
                  if (*(*argv + 1) != '\0') {
                     if (sscanf(*argv + 1, "%d", &col) == 0)
                        col = COL;
                     while (*(*argv + 1) != '\0')
                        (*argv)++;
                  }
                  if (!((c1 == 'd') || (c1 == 'f'))) {
                     form = (char *) malloc(strlen(FORM_INT) + 1);
                     strcpy(form, FORM_INT);
                  }
               }
               break;
            default:
               fprintf(stderr, "%s : Invalid option '+%c'!\n", cmnd,
                       *(*argv + 1));
               usage(1);
            }
            (*argv)++;
         }
      } else if (**argv == '-') {
         switch (*(*argv + 1)) {
         case 'r':
            round = 1 - round;
            break;
         case 'w':
	    warn = atoi(*++argv);
            --argc;
            break;
         case 'o':
	    clip = 1 - clip;
            break;
         case 'h':
            usage(0);
         default:
            fprintf(stderr, "%s : Invalid option '-%c'!\n", cmnd, *(*argv + 1));
            usage(1);
         }
      } else if (**argv == '%')
         form = *argv;
      else if (c1 == 'a')
         fp = getfp(*argv, "rt");
      else
         fp = getfp(*argv, "rb");

   if (round)
      r = 0.5;

   if (size1 == 0) {
      size1 = sizeof(float);
      c1 = 'f';
   }
   if (size2 == 0) {
      size2 = size1;
      c2 = c1;
   }

   if (c1 == 'a') {
      if (c2 == 'a')
         while (fscanf(fp, "%le", &x) != EOF) {
            printf(form, x);
            if (i == col) {
               i = 1;
               printf("\n");
            } else {
               i++;
               printf("\t");
            }
	 } else
         while (fscanf(fp, "%le", &x) != EOF) {
	   x2x(&x, &x, 'd', c2,clip,warn);
            fwritex(&x, size2, 1, stdout);
         }
   } else {
      if (c2 == 'a') {
         while (freadx(&x, size1, 1, fp) == 1) {
	   x2x(&x, &x, c1, 'd',clip,warn);
            switch (c1) {
            case 'd':
               printf(form, x);
               break;
            case 'f':
               printf(form, x);
               break;
            default:
               printf(form, (int) x);
            }
            if (i == col) {
               i = 1;
               printf("\n");
            } else {
               i++;
               printf("\t");
            }
         }
      } else
         while (freadx(&x, size1, 1, fp) == 1) {
	   x2x(&x, &x, c1, c2,clip,warn);
            fwritex(&x, size2, 1, stdout);
         }
   }

   return (0);
}

void x2x(void *x1, void *x2, char c1, char c2, int clip, int warn)
{
   long xl = 0;
   unsigned long xul = 0;
   double xd = 0.0;
   int y = 0;
   switch (c1) {
   case 's':
      xl = *(short *) x1;
      break;
   case 'i':
      xl = *(int *) x1;
      break;
   case 'l':
      xl = *(long *) x1;
      break;
   case 'S':
      xul = *(unsigned short *) x1;
      break;
   case 'I':
      xul = *(unsigned int *) x1;
      break;
   case 'L':
      xul = *(unsigned long *) x1;
      break;
   case 'f':
      xd = *(float *) x1;
      break;
   case 'd':
      xd = *(double *) x1;
      break;
   case 'c':
      xl = *(char *) x1;
      break;
   case 'C':
      xul = *(unsigned char *) x1;
      break;
   case 't':                   /* 3byte, signed */
      y = *(int *) x1 & 0x00FFFFFF;
      if (y >> 23 == 1)
         y = y | 0xFF000000;
      xl = y;
      break;
   case 'T':                   /* 3byte, unsigned */
      xul = *(unsigned int *) x1 & 0x00FFFFFF;
      break;
   }
   if (warn == 1) {
      switch (c2) {
      case 's':
        if (xl > SHRT_MAX || xl < SHRT_MIN || xul > SHRT_MAX || xd > SHRT_MAX || xd < SHRT_MIN) {
           fprintf(stderr, "%s : warning: input data is over the range of type 'short'!\n", cmnd);
        }
        break;
      case 'i':
        if (xl > INT_MAX || xl < INT_MIN || xul > INT_MAX || xd > INT_MAX || xd < INT_MIN) {
           fprintf(stderr, "%s : warning: input data is over the range of type 'int'!\n", cmnd);
        }
        break;
      case 'l':
        if (xul > LONG_MAX || xd > LONG_MAX || xd < LONG_MIN) {
           fprintf(stderr, "%s : warning: input data is over the range of type 'long'!\n", cmnd);
        }
        break;
      case 'S':
        if (xl > USHRT_MAX || xl < 0 || xul > USHRT_MAX || xd > USHRT_MAX || xd < 0.0) {
           fprintf(stderr, "%s : warning: input data is over the range of type 'unsigned short'!\n", cmnd);
        }
        break;
      case 'I':
        if (xl > UINT_MAX || xl < 0 || xul > UINT_MAX || xd > UINT_MAX || xd < 0.0) {
           fprintf(stderr, "%s : warning: input data is over the range of type 'unsigned int'!\n", cmnd);
        }
        break;
      case 'L':
        if (xl < 0|| xd > ULONG_MAX || xd < 0.0) {
           fprintf(stderr, "%s : warning: input data is over the range of type 'unsigned long'!\n", cmnd);
        }
        break;
      case 'f':
        if (xd > FLT_MAX || xd < (FLT_MAX * (-1))) {
           fprintf(stderr, "%s : warning: input data is over the range of type 'float'!\n", cmnd);
        }
        break;
      case 'd':
        if (xd > DBL_MAX || xd < (DBL_MAX * (-1))) {
           fprintf(stderr, "%s : warning: input data is over the range of type 'double'!\n", cmnd);
        }
        break;
      case 'c':
        if (xl > CHAR_MAX || xl < CHAR_MIN || xul > CHAR_MAX || xd > CHAR_MAX || xd < CHAR_MIN) {
           fprintf(stderr, "%s : warning: input data is over the range of type 'char'!\n", cmnd);
        }
        break;
      case 'C':
        if (xl > UCHAR_MAX || xl < 0.0 || xul > UCHAR_MAX || xd > UCHAR_MAX || xd < 0.0) {
           fprintf(stderr, "%s : warning: input data is over the range of type 'unsigned char'!\n", cmnd);
        }
        break;
      case 't':
        if (xl > INT3_MAX || xl < INT3_MIN || xul > INT3_MAX || xd > INT3_MAX || xd < INT3_MIN) {
           fprintf(stderr, "%s : warning: input data is over the range of type 'int(3byte)'!\n", cmnd);
        }
        break;
      case 'T':
        if (xl > UINT3_MAX || xl < 0|| xul > UINT3_MAX || xd > UINT3_MAX || xd < 0.0) {
           fprintf(stderr, "%s : warning: input data is over the range of type 'unsigned int(3byte)'!\n", cmnd);
        }
        break;
      }
   }else if (warn == 2) {
      switch (c2) {
      case 's':
        if (xl > SHRT_MAX || xl < SHRT_MIN || xul > SHRT_MAX || xd > SHRT_MAX || xd < SHRT_MIN) {
           fprintf(stderr, "%s : warning:input data is over the range of type 'short'!\n", cmnd);
           usage(1);
        }
        break;
      case 'i':
        if (xl > INT_MAX || xl < INT_MIN || xul > INT_MAX || xd > INT_MAX || xd < INT_MIN) {
           fprintf(stderr, "%s : warning:input data is over the range of type 'int'!\n", cmnd);
           usage(1);
        }
        break;
      case 'l':
        if (xul > LONG_MAX || xd > LONG_MAX || xd < LONG_MIN) {
           fprintf(stderr, "%s : warning:input data is over the range of type 'long'!\n", cmnd);
           usage(1);
        }
        break;
      case 'S':
        if (xl > USHRT_MAX || xl < 0 || xul > USHRT_MAX || xd > USHRT_MAX || xd < 0.0) {
           fprintf(stderr, "%s : warning:input data is over the range of type 'unsigned short'!\n", cmnd);
           usage(1);
        }
        break;
      case 'I':
        if (xl > UINT_MAX || xl < 0 || xul > UINT_MAX || xd > UINT_MAX || xd < 0.0) {
           fprintf(stderr, "%s : warning:input data is over the range of type 'unsigned int'!\n", cmnd);
           usage(1);
        }
        break;
      case 'L':
        if (xl < 0|| xd > ULONG_MAX || xd < 0.0) {
           fprintf(stderr, "%s : warning:input data is over the range of type 'unsigned long'!\n", cmnd);
           usage(1);
        }
        break;
      case 'f':
        if (xd > FLT_MAX || xd < (FLT_MAX * (-1))) {
           fprintf(stderr, "%s : warning:input data is over the range of type 'float'!\n", cmnd);
           usage(1);
        }
        break;
      case 'd':
        if (xd > DBL_MAX || xd < (DBL_MAX * (-1))) {
           fprintf(stderr, "%s : warning:input data is over the range of type 'double'!\n", cmnd);
           usage(1);
        }
        break;
      case 'c':
        if (xl > CHAR_MAX || xl < CHAR_MIN || xul > CHAR_MAX || xd > CHAR_MAX || xd < CHAR_MIN) {
           fprintf(stderr, "%s : warning:input data is over the range of type 'char'!\n", cmnd);
           usage(1);
        }
        break;
      case 'C':
        if (xl > UCHAR_MAX || xl < 0.0 || xul > UCHAR_MAX || xd > UCHAR_MAX || xd < 0.0) {
           fprintf(stderr, "%s : warning:input data is over the range of type 'unsigned char'!\n", cmnd);
           usage(1);
        }
        break;
      case 't':
        if (xl > INT3_MAX || xl < INT3_MIN || xul > INT3_MAX || xd > INT3_MAX || xd < INT3_MIN) {
           fprintf(stderr, "%s : warning:input data is over the range of type 'int(3byte)'!\n", cmnd);
           usage(1);
        }
        break;
      case 'T':
        if (xl > UINT3_MAX || xl < 0|| xul > UINT3_MAX || xd > UINT3_MAX || xd < 0.0) {
           fprintf(stderr, "%s : warning:input data is over the range of type 'unsigned int(3byte)'!\n", cmnd);
           usage(1);
        }
        break;
      }
   }
   if (clip) {
      switch (c2) {
      case 's':
        xl = ((xl < SHRT_MIN) ? SHRT_MIN : ((xl > SHRT_MAX) ? SHRT_MAX : xl));
        xul = ((xul > SHRT_MAX) ? SHRT_MAX : xl);
        xd = ((xd < SHRT_MIN) ? SHRT_MIN : ((xd > SHRT_MAX) ? SHRT_MAX : xd));
        break;
      case 'i':
        xl = ((xl < INT_MIN) ? INT_MIN : ((xl > INT_MAX) ? INT_MAX : xl));
        xul = ((xul > INT_MAX) ? INT_MAX : xl);
        xd = ((xd < INT_MIN) ? INT_MIN : ((xd > INT_MAX) ? INT_MAX : xd));
        break;
      case 'l':
        xul = ((xul > LONG_MAX) ? LONG_MAX : xl);
        xd = ((xd < LONG_MIN) ? LONG_MIN : ((xd > LONG_MAX) ? LONG_MAX : xd));
        break;
      case 'S':
        xl = ((xl < 0) ? 0 : ((xl > USHRT_MAX) ? USHRT_MAX : xl));
        xul = ((xul > USHRT_MAX) ? USHRT_MAX : xl);
        xd = ((xd < 0.0) ? 0.0 : ((xd > USHRT_MAX) ? USHRT_MAX : xd));
        break;
      case 'I':
        xl = ((xl < 0) ? 0 : ((xl > UINT_MAX) ? UINT_MAX : xl));
        xul = ((xul > UINT_MAX) ? UINT_MAX : xl);
        xd = ((xd < 0.0) ? 0.0 : ((xd > UINT_MAX) ? UINT_MAX : xd));
        break;
      case 'L':
        xl = ((xl < 0) ? 0 : xl);
        xd = ((xd < 0.0) ? 0.0 : ((xd > ULONG_MAX) ? ULONG_MAX : xd));
        break;
      case 'f':
        xd = ((xd < (FLT_MAX * (-1))) ? (FLT_MAX * (-1)) : ((xd > FLT_MAX) ? FLT_MAX : xd));
        break;
      case 'd':
        xd = ((xd < (DBL_MAX * (-1))) ? (DBL_MAX * (-1)) : ((xd > DBL_MAX) ? DBL_MAX : xd));
        break;
      case 'c':
        xl = ((xl < CHAR_MIN) ? CHAR_MIN : ((xl > CHAR_MAX) ? CHAR_MAX : xl));
        xul = ((xul > CHAR_MAX) ? CHAR_MAX : xl);
        xd = ((xd < CHAR_MIN) ? CHAR_MIN : ((xd > CHAR_MAX) ? CHAR_MAX : xd));
        break;
      case 'C':
        xl = ((xl < 0) ? 0 : ((xl > UCHAR_MAX) ? UCHAR_MAX : xl));
        xul = ((xul > UCHAR_MAX) ? UCHAR_MAX : xl);
        xd = ((xd < 0.0) ? 0.0 : ((xd > UCHAR_MAX) ? UCHAR_MAX : xd));
        break;
      case 't':
        xl = ((xl < INT3_MIN) ? INT3_MIN : ((xl > INT3_MAX) ? INT3_MAX : xl));
        xul = ((xul > INT3_MAX) ? INT3_MAX : xl);
        xd = ((xd < INT3_MIN) ? INT3_MIN : ((xd > INT3_MAX) ? INT3_MAX : xd));
        break;
      case 'T':
        xl = ((xl < 0) ? 0 : ((xl > UINT3_MAX) ? UINT3_MAX : xl));
        xul = ((xul > UINT3_MAX) ? UINT3_MAX : xl);
        xd = ((xd < 0.0) ? 0.0 : ((xd > UINT3_MAX) ? UINT3_MAX : xd));
        break;
      }
   }
 
   switch (c2) {
   case 's':
      if (c1 == 's' || c1 == 'i' || c1 == 'l' || c1 == 'i' || c1 == 'c' || c1 == 't')
        *(short *) x2 = xl;
      else if (c1 == 'S' || c1 == 'I' || c1 == 'L' || c1 == 'C' || c1 == 'T')
        *(short *) x2 = xul;
      else {
        if (xd > 0)
          *(short *) x2 = xd + r;
        else
          *(short *) x2 = xd - r;   
      }
      break;
   case 'i':
     if (c1 == 's' || c1 == 'i' || c1 == 'l' || c1 == 'i' || c1 == 'c' || c1 == 't')
        *(int *) x2 = xl;
      else if (c1 == 'S' || c1 == 'I' || c1 == 'L' || c1 == 'C' || c1 == 'T')
        *(int *) x2 = xul;
      else {
        if (xd > 0)
          *(int *) x2 = xd + r; 
        else
          *(int *) x2 = xd - r;   
      }
      break;
   case 'l':
      if (c1 == 's' || c1 == 'i' || c1 == 'l' || c1 == 'i' || c1 == 'c' || c1 == 't')
        *(long *) x2 = xl;
      else if (c1 == 'S' || c1 == 'I' || c1 == 'L' || c1 == 'C' || c1 == 'T')
        *(long *) x2 = xul;
      else {
        if (xd > 0)
          *(long *) x2 = xd + r; 
        else
          *(long *) x2 = xd - r;   
      }
      break;
   case 'S':
      if (c1 == 's' || c1 == 'i' || c1 == 'l' || c1 == 'i' || c1 == 'c' || c1 == 't')
        *(unsigned short *) x2 = xl;
      else if (c1 == 'S' || c1 == 'I' || c1 == 'L' || c1 == 'C' || c1 == 'T')
        *(unsigned short *) x2 = xul;
      else {
        if (xd > 0)
          *(unsigned short *) x2 = xd + r; 
        else
          *(unsigned short *) x2 = xd - r;   
      }
      break;
   case 'I':
      if (c1 == 's' || c1 == 'i' || c1 == 'l' || c1 == 'i' || c1 == 'c' || c1 == 't')
        *(unsigned int *) x2 = xl;
      else if (c1 == 'S' || c1 == 'I' || c1 == 'L' || c1 == 'C' || c1 == 'T')
        *(unsigned int *) x2 = xul;
      else {
        if (xd > 0)
          *(unsigned int *) x2 = xd + r; 
        else
          *(unsigned int *) x2 = xd - r;   
      }
      break;
   case 'L':
      if (c1 == 's' || c1 == 'i' || c1 == 'l' || c1 == 'i' || c1 == 'c' || c1 == 't')
        *(unsigned long *) x2 = xl;
      else if (c1 == 'S' || c1 == 'I' || c1 == 'L' || c1 == 'C' || c1 == 'T')
        *(unsigned long *) x2 = xul;
      else {
        if (xd > 0)
          *(unsigned long *) x2 = xd + r; 
        else
          *(unsigned long *) x2 = xd - r;   
      }
      break;
   case 'f':
      if (c1 == 's' || c1 == 'i' || c1 == 'l' || c1 == 'i' || c1 == 'c' || c1 == 't')
        *(float *) x2 = xl;
      else if (c1 == 'S' || c1 == 'I' || c1 == 'L' || c1 == 'C' || c1 == 'T')
        *(float *) x2 = xul;
      else {
        if (xd > 0)
          *(float *) x2 = xd; 
        else
          *(float *) x2 = xd;   
      }
      break;
   case 'd':
      if (c1 == 's' || c1 == 'i' || c1 == 'l' || c1 == 'i' || c1 == 'c' || c1 == 't')
        *(double *) x2 = xl;
      else if (c1 == 'S' || c1 == 'I' || c1 == 'L' || c1 == 'C' || c1 == 'T')
        *(double *) x2 = xul;
      else {
        if (xd > 0)
          *(double *) x2 = xd; 
        else
          *(double *) x2 = xd;   
      }
      break;
   case 'c':
      if (c1 == 's' || c1 == 'i' || c1 == 'l' || c1 == 'i' || c1 == 'c' || c1 == 't')
        *(char *) x2 = xl;
      else if (c1 == 'S' || c1 == 'I' || c1 == 'L' || c1 == 'C' || c1 == 'T')
        *(char *) x2 = xul;
      else {
        if (xd > 0)
          *(char *) x2 = xd + r; 
        else
          *(char *) x2 = xd - r;   
      }
      break;
   case 'C':
      if (c1 == 's' || c1 == 'i' || c1 == 'l' || c1 == 'i' || c1 == 'c' || c1 == 't')
        *(unsigned char *) x2 = xl;
      else if (c1 == 'S' || c1 == 'I' || c1 == 'L' || c1 == 'C' || c1 == 'T')
        *(unsigned char *) x2 = xul;
      else {
        if (xd > 0)
          *(unsigned char *) x2 = xd + r; 
        else
          *(unsigned char *) x2 = xd - r;   
      }
      break;
   case 't':
      if (c1 == 's' || c1 == 'i' || c1 == 'l' || c1 == 'i' || c1 == 'c' || c1 == 't')
        *(int *) x2 = xl;
      else if (c1 == 'S' || c1 == 'I' || c1 == 'L' || c1 == 'C' || c1 == 'T')
        *(int *) x2 = xul;
      else {
        if (xd > 0)
          *(int *) x2 = xd + r; 
        else
          *(int *) x2 = xd - r;   
      }
      break;
   case 'T':
      if (c1 == 's' || c1 == 'i' || c1 == 'l' || c1 == 'i' || c1 == 'c' || c1 == 't')
        *(unsigned int *) x2 = xl;
      else if (c1 == 'S' || c1 == 'I' || c1 == 'L' || c1 == 'C' || c1 == 'T')
        *(unsigned int *) x2 = xul;
      else {
        if (xd > 0)
          *(unsigned int *) x2 = xd + r; 
        else
          *(unsigned int *) x2 = xd - r;   
      }
      break;
   }
   return;
}
