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
*               %format  :  specify output format similar to  [%g]       *
*                           "printf()".                                  *
*                           if type2 is ascii.                           *
*                                                                        *
*************************************************************************/

static char *rcs_id = "$Id: x2x.c,v 1.27 2010/04/01 12:27:46 uratec Exp $";


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

/*  Default Values  */
#define ROUND      FA
#define COL        1
#define FORM_INT   "%d"
#define FORM_FLOAT "%g"

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
   FILE *fp = stdin;
   Boolean round = ROUND;
   void x2x(void *x1, void *x2, char c1, char c2);

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
            x2x(&x, &x, 'd', c2);
            fwritex(&x, size2, 1, stdout);
         }
   } else {
      if (c2 == 'a') {
         while (freadx(&x, size1, 1, fp) == 1) {
            x2x(&x, &x, c1, 'd');
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
            x2x(&x, &x, c1, c2);
            fwritex(&x, size2, 1, stdout);
         }
   }

   return (0);
}

void x2x(void *x1, void *x2, char c1, char c2)
{
   double x = 0.0;
   int y = 0;

   switch (c1) {
   case 's':
      x = *(short *) x1;
      break;
   case 'i':
      x = *(int *) x1;
      break;
   case 'l':
      x = *(long *) x1;
      break;
   case 'S':
      x = *(unsigned short *) x1;
      break;
   case 'I':
      x = *(unsigned int *) x1;
      break;
   case 'L':
      x = *(unsigned long *) x1;
      break;
   case 'f':
      x = *(float *) x1;
      break;
   case 'd':
      x = *(double *) x1;
      break;
   case 'c':
      x = *(char *) x1;
      break;
   case 'C':
      x = *(unsigned char *) x1;
      break;
   case 't':                   /* 3byte, signed */
      y = *(int *) x1 & 0x00FFFFFF;
      if (y >> 23 == 1)
         y = y | 0xFF000000;
      x = y;
      break;
   case 'T':                   /* 3byte, unsigned */
      x = *(unsigned int *) x1 & 0x00FFFFFF;
      break;
   }

   switch (c2) {
   case 's':
      if (x > 0)
         *(short *) x2 = x + r;
      else
         *(short *) x2 = x - r;
      break;
   case 'i':
      if (x > 0)
         *(int *) x2 = x + r;
      else
         *(int *) x2 = x - r;
      break;
   case 'l':
      if (x > 0)
         *(long *) x2 = x + r;
      else
         *(long *) x2 = x - r;
      break;
   case 'S':
      if (x > 0)
         *(unsigned short *) x2 = x + r;
      else
         *(unsigned short *) x2 = x - r;
      break;
   case 'I':
      if (x > 0)
         *(unsigned int *) x2 = x + r;
      else
         *(unsigned int *) x2 = x - r;
      break;
   case 'L':
      if (x > 0)
         *(unsigned long *) x2 = x + r;
      else
         *(unsigned long *) x2 = x - r;
      break;
   case 'f':
      *(float *) x2 = x;
      break;
   case 'd':
      *(double *) x2 = x;
      break;
   case 'c':
      if (x > 0)
         *(char *) x2 = x + r;
      else
         *(char *) x2 = x - r;
      break;
   case 'C':
      if (x > 0)
         *(unsigned char *) x2 = x + r;
      else
         *(unsigned char *) x2 = x - r;
      break;
   case 't':
      if (x > 0)
         *(int *) x2 = x + r;
      else
         *(int *) x2 = x - r;
      break;
   case 'T':
      if (x > 0)
         *(unsigned int *) x2 = x + r;
      else
         *(unsigned int *) x2 = x - r;
      break;
   }

   return;
}
