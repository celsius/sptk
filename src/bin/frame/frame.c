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
*    Extract Frame from Data Sequence                                   *
*                                                                       *
*                                       1985.11 K.Tokuda                *
*                                       1996.4  K.Koishida              *
*                                                                       *
*       usage:                                                          *
*               frame [ options ] [ infile ] > stdout                   *
*       options:                                                        *
*               -l l     :  frame length                [256]           *
*               -p p     :  frame period                [100]           *
*               -n       :  no center start point       [FALSE]         *
*       infile:                                                         *
*               data sequence                                           *
*                   , x(0), x(1), ...,                                  *
*       stdout:                                                         *
*               frame sequence                                          *
*                   0, 0, ..., 0, x(0), x(1), ..., x(l/2),              *
*                   , x(t), x(t+1),       ...,       x(t+l-1),          *
*                   , x(2t), x(2t+1),     ....                          *
*              if -n specified                                          *
*                   x(0), x(1),           ...,       x(l),              *
*                   , x(t), x(t+1),       ...,       x(t+l-1),          *
*                   , x(2t), x(2t+1),     ....                          *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id$";


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


#if defined(WIN32)
#  include "SPTK.h"
#else
#  include <SPTK.h>
#endif

/*  Default Values  */
#define LENG 256
#define FPERIOD 100
#define NOCTR FA

char *BOOL[] = { "FALSE", "TRUE" };

/*  Command Name  */
char *cmnd;

typedef struct _float_list {
   float f;
   struct _float_list *next;
} float_list;

void usage(int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - extract frame from data sequence\n", cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -l l  : frame length          [%d]\n", LENG);
   fprintf(stderr, "       -p p  : frame period          [%d]\n", FPERIOD);
   fprintf(stderr, "       -n    : no center start point [%s]\n", BOOL[NOCTR]);
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       data sequence                 [stdin]\n");
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       extracted data sequence\n");
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
   int l = LENG, fprd = FPERIOD, i, j, length, left = 0, current = 0;
   FILE *fp = stdin;
   Boolean noctr = NOCTR;
   double *x, *in, *buf;
   char *s, c;
   float_list *top, *cur, *prev, *tmpf;

   if ((cmnd = strrchr(argv[0], '/')) == NULL)
      cmnd = argv[0];
   else
      cmnd++;
   while (--argc)
      if (*(s = *++argv) == '-') {
         c = *++s;
         switch (c) {
         case 'l':
            l = atoi(*++argv);
            --argc;
            break;
         case 'p':
            fprd = atoi(*++argv);
            --argc;
            break;
         case 'n':
            noctr = 1 - noctr;
            break;
         case 'h':
            usage(0);
         default:
            fprintf(stderr, "%s : Invalid option '%c'!\n", cmnd, *(*argv + 1));
            usage(1);
         }
      } else
         fp = getfp(*argv, "rb");


   in = dgetmem(1);
   top = prev = (float_list *) malloc(sizeof(float_list));
   length = 0;
   prev->next = NULL;
   while (freadf(in, sizeof(*x), 1, fp) == 1) {
      cur = (float_list *) malloc(sizeof(float_list));
      cur->f = (float) in[0];
      length++;
      prev->next = cur;
      cur->next = NULL;
      prev = cur;
   }
   tmpf = top->next;
   buf = dgetmem(length);
   for (i = 0; tmpf != NULL; i++, tmpf = tmpf->next)
      buf[i] = tmpf->f;

   x = dgetmem(l);

   if (!noctr)
      left = - l / 2;

   for(;current < length; current += fprd, left += fprd) {
      for (i = 0, j = left; j <= left + l - 1; j++, i++) {
         if (j < 0 || j > (length - 1))
            x[i] = 0.0;
         else
            x[i] = buf[j];
      }
      fwritef(x, sizeof(*x), l, stdout);
   }

   return 0;
}
