/*
  ----------------------------------------------------------------
 Speech Signal Processing Toolkit (SPTK): version 3.0
    SPTK Working Group

     Department of Computer Science
     Nagoya Institute of Technology
    and
    Interdisciplinary Graduate School of Science and Engineering
     Tokyo Institute of Technology
        Copyright (c) 1984-2000
   All Rights Reserved.

  Permission is hereby granted, free of charge, to use and
  distribute this software and its documentation without
  restriction, including without limitation the rights to use,
  copy, modify, merge, publish, distribute, sublicense, and/or
  sell copies of this work, and to permit persons to whom this
  work is furnished to do so, subject to the following conditions:

    1. The code must retain the above copyright notice, this list
       of conditions and the following disclaimer.

    2. Any modifications must be clearly marked as such.

  NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSITITUTE OF TECHNOLOGY,
  SPTK WORKING GROUP, AND THE CONTRIBUTORS TO THIS WORK DISCLAIM
  ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT
  SHALL NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSITITUTE OF
  TECHNOLOGY, SPTK WORKING GROUP, NOR THE CONTRIBUTORS BE LIABLE
  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
  DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
  WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
  ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
  PERFORMANCE OF THIS SOFTWARE.
 ----------------------------------------------------------------
*/

/************************************************************************
*                                                                       *
*    Find NaN & Infty & Indefinite Values                               *
*                                                                       *
*                                    1997.7 G.Hirabayashi               *
*                                                                       *
*       usage:                                                          *
*               nan [ options ] [ infile ] > stdout                     *
*       options:                                                        *
*               -d       :  input is assumed to be double       [FALSE] *
*       infile:                                                         *
*               stdin for default                                       *
*               input is assumed to be float                            *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id: nan.c,v 1.9 2007/09/10 12:49:22 heigazen Exp $";


/* Standard C Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SPTK.h>

#ifndef __USE_ISOC99
#ifndef DOUBLE
#define F_MASKEXP    0x7F800000L
#define F_MASKSIG    0x007FFFFFL
#define F_INDEFINITE 0xFFC00000L
#define D_MASKEXP    0x7FF00000L
#else
#define D_MASKSIG    0x000FFFFFL
#define D_INDEFINITE 0xFFF80000L
#endif  /* DOUBLE */
#endif  /* __USE_ISOC99 */

/* Command Name */
char *cmnd;


void usage (int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - find NaN and Infty values\n", cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       data sequence (float)        [stdin]\n");
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       result messages\n");
#ifdef SPTK_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n",SPTK_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(status);
}

int main (int argc,char *argv[])
{
   FILE *fp;
   char *s, *infile=NULL, c;
   void nan_tmp(FILE *fp);

   if ((cmnd=strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;
   while (--argc) {
      if (*(s=*++argv)=='-') {
         c = *++s;
         switch (c) {
         case 'h':
            usage (0);
         default:
            fprintf(stderr, "%s: unknown option '%c'\n", cmnd, c);
            usage (1);
            break;
         }
      }
      else
         infile = s;
   }
   if (infile) fp = getfp(infile, "r");
   else  fp = stdin;

   nan_tmp(fp);

   return(0);
}

void nan_tmp (FILE *fp)
{
   long count=0;
#ifdef DOUBLE
   double x;
#else
   long x;
#endif /* DOUBLE */

#ifdef __USE_ISOC99
   while (fread(&x, sizeof(x), 1, fp)) {
      if (isinf(x))
         fprintf(stdout, "[No. %ld] is Infty\n", count);
      if (isnan(x))
         fprintf(stdout, "[No. %ld] is NaN\n", count);
      ++count;
   }
#else
   while(fread(&x, sizeof(x), 1, fp))
   {
#ifdef DOUBLE
       long upper = (long *)&x;
       long lower = upper+1;

       if (!((*upper & D_MASKEXP) ^ D_MASKEXP)) {
          if ((!(*upper & D_MASKSIG))&&(!*lower))
             fprintf(stdout, "[No. %ld] is Infty\n", count);
          else if((*upper == D_INDEFINITE)&&(!*lower))
             fprintf(stderr, "[No. %ld] is Indifinite\n", count);
          else
             fprintf(stdout, "[No. %ld] is NaN\n", count);
       }
#else
      if (!((x & F_MASKEXP) ^ F_MASKEXP)) {
         if (!(x & F_MASKSIG))
            fprintf(stdout, "[No. %ld] is Infty\n", count);
         else if (x==F_INDEFINITE)
            fprintf(stderr, "[No. %ld] is Indifinite\n", count);
         else
            fprintf(stdout, "[No. %ld] is NaN\n", count);
      }
#endif  /* DOUBLE */
      ++count;
   }
#endif  /* __USE_ISOC99 */

   return;
}

