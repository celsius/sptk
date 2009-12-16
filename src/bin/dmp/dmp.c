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
/*                1996-2009  Nagoya Institute of Technology          */
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
*    Binary File Dump                                                   * 
*                                                                       *
*                                       1996.5  K.Koishida              *
*                                                                       *
*       usage:                                                          *
*               dmp [options] [infile] > stdout                         *
*       options:                                                        *
*               -n n     :  block order  (0,...,n)           [EOD]      *
*               -l l     :  block length (1,...,l)           [EOD]      *
*               +type    :  data type                        [f]        *
*                               c (char)     s (short)                  *
*                               i (int)      l (long)                   *
*                               f (float)    d (double)                 *
*               %form    :  print format(printf style)       [N/A]      *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id: dmp.c,v 1.20 2009/12/16 13:12:28 uratec Exp $";


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

#include <ctype.h>

#if defined(WIN32)
#  include "SPTK.h"
#else
#  include <SPTK.h>
#endif

/*  Default Values  */
#define SIZE 128


/*  Command Name  */
char *cmnd;


void usage(int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - binary file dump\n", cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -n n  : block order   (0,...,n)      [EOD]\n");
   fprintf(stderr, "       -l l  : block length  (1,...,l)      [EOD]\n");
   fprintf(stderr, "       +type : data type                    [f]\n");
   fprintf(stderr, "                c (char)      s (short)\n");
   fprintf(stderr, "                i (int)       l (long)\n");
   fprintf(stderr, "                f (float)     d (double)\n");
   fprintf(stderr, "       %%form : print format(printf style)   [N/A]\n");
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       data sequence                        [stdin]\n");
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       dumped sequence\n");
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
   int n = -1, i = 0, eflag = 0, lflag = 0;
   size_t size = sizeof(float);
   FILE *fp = stdin;
   char *s, c, cc = 'f';
   char format[SIZE], form[SIZE];
   int ff = 0;
   union u {
      short s;
      int i;
      float f;
      double d;
      char c;
      long l;
   } x;

   if ((cmnd = strrchr(argv[0], '/')) == NULL)
      cmnd = argv[0];
   else
      cmnd++;

   while (--argc)
      if (*(s = *++argv) == '-') {
         c = *++s;
         switch (c) {
         case 'n':
            n = atoi(*++argv) + 1;
            --argc;
            break;
         case 'l':
            n = atoi(*++argv) - 1;
            lflag = 1;
            --argc;
            break;
         case 'e':
            eflag = 1 - eflag;
            break;
         case 'h':
            usage(0);
         default:
            fprintf(stderr, "%s : Invalid option '%c'!\n", cmnd, *(*argv + 1));
            usage(1);
         }
      } else if (*s == '+') {
         c = *++s;
         switch (c) {
         case 'c':
            cc = 'c';
            size = sizeof(char);
            break;
         case 's':
            cc = 's';
            size = sizeof(short);
            break;
         case 'l':
            cc = 'l';
            size = sizeof(long);
            break;
         case 'i':
            cc = 'i';
            size = sizeof(int);
            break;
         case 'f':
            cc = 'f';
            size = sizeof(float);
            break;
         case 'd':
            cc = 'd';
            size = sizeof(double);
            break;
         default:
            fprintf(stderr, "%s : Invalid option '%c'!\n", cmnd, *(*argv + 1));
            usage(1);
         }
      } else if (*s == '%') {
         strcpy(format, s);
         ff = 1;
      } else
         fp = getfp(*argv, "rb");

   for (i = 0;; i++) {
      if ((n >= 0) && (i >= n + lflag))
         i = 0;
      if (freadx(&x.f, size, 1, fp) != 1)
         break;
      if (eflag)
         printf("%d\t0\n", i + lflag);
      switch (cc) {
      case 's':
         strcpy(form, "%d\t%d\n");
         if (ff) {
            strcpy(form, "%d\t");
            strcat(form, format);
            strcat(form, "\n");
         }
         printf(form, i + lflag, x.s);
         break;
      case 'i':
         strcpy(form, "%d\t%d\n");
         if (ff) {
            strcpy(form, "%d\t");
            strcat(form, format);
            strcat(form, "\n");
         }
         printf(form, i + lflag, x.i);
         break;
      case 'f':
         strcpy(form, "%d\t%g\n");
         if (ff) {
            strcpy(form, "%d\t");
            strcat(form, format);
            strcat(form, "\n");
         }
         printf(form, i + lflag, x.f);
         break;
      case 'd':
         strcpy(form, "%d\t%g\n");
         if (ff) {
            strcpy(form, "%d\t");
            strcat(form, format);
            strcat(form, "\n");
         }
         printf(form, i + lflag, x.d);
         break;
      case 'c':
         strcpy(form, "%d\t%d\n");
         if (ff) {
            strcpy(form, "%d\t");
            strcat(form, format);
            strcat(form, "\n");
         }
         printf(form, i + lflag, x.c);
         break;
      case 'l':
         strcpy(form, "%d\t%d\n");
         if (ff) {
            strcpy(form, "%d\t");
            strcat(form, format);
            strcat(form, "\n");
         }
         printf(form, i + lflag, x.l);
         break;
      }
      if (eflag)
         printf("%d\t0\n", i + lflag);
   }

   return 0;
}
