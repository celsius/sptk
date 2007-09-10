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
*    FFT for Real Sequence                                              *
*                                                                       *
*                                       1987    T.Kobayashi             *
*                                       1995.12 N.Isshiki   modified    *
*                                                                       *
*       usage:                                                          *
*               fftr [ options ] [ infile ] > stdout                    *
*       options:                                                        *
*               -l l     :  FFT size power of 2         [256]           *
*               -m m     :  order of sequence           [l-1]           *
*               -A       :  amplitude                                   *
*               -R       :  real part                                   *
*               -I       :  imaginary part                              *
*               -H       :  output half size                            *
*               -P       :  power                                       *
*       infile:                                                         *
*               stdin for default                                       *
*               input is assumed to be float                            *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id$";


/* Standard C Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SPTK.h>


/* Default Values */
#define SIZE 256


/* Command Name */
char *cmnd;


void usage (int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - FFT for real sequence\n", cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -l l  : FFT size power of 2 [%d]\n", SIZE);
   fprintf(stderr, "       -m m  : order of sequence   [l-1]\n");
   fprintf(stderr, "       -A    : amplitude\n");
   fprintf(stderr, "       -R    : real part\n");
   fprintf(stderr, "       -I    : imaginary part\n");
   fprintf(stderr, "       -P    : power\n");
   fprintf(stderr, "       -H    : output half size\n");
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       data sequence (float)       [stdin]\n");
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       FFT sequence (float)\n");
#ifdef SPTK_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n", SPTK_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(status);
}

int main (int argc,char *argv[])
{
   FILE *fp;
   char *s, *infile=NULL, c;
   int size=SIZE, nout=0, nd=-1, out=' '; 
   int dft (FILE *fp, const int size, const int nd, const int out, const int nout);
   
   if ((cmnd = strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;
   while (--argc) {
      if (*(s = *++argv)=='-') {
         c = *++s;
         if ((c=='l' || c=='m') && (*++s=='\0')) {
            s = *++argv;
            --argc;
         }
         switch (c) {
         case 'l':
            size = atoi(s);
            break;
         case 'm':
            nd = atoi(s) + 1;
            break;
         case 'H':
            nout = 1;
            break;
         case 'i':
         case 'p':
         case 'r':
            c -= ('a' - 'A');
#ifdef AOP
         case 'A':
#endif
         case 'I':
         case 'P':
         case 'R':
            out = c;
            break;
         case 'h':
            usage(0);
         default:
            fprintf(stderr, "%s: unknown option '%c'\n", cmnd, c);
            usage(1);
         }
      }
      else
         infile = s;
   }

   if (nd==-1) nd = size;
   if (nd>size) {
      fprintf(stderr, "%s: oder of sequence > FFT size\n", cmnd);
      return(1);
   }
   
   nout = (nout) ? size / 2 + 1 : size;
   if (infile) {
      fp = getfp(infile, "r");
      dft(fp,size,nd,out,nout);
      fclose(fp);
   }
   else
      dft(stdin,size,nd,out,nout);
 
   return 0;
}

int dft (FILE *fp, const int size, const int nd, const int out, const int nout)
{
   double *x, *y;
   int k;

   x = dgetmem(size + size);
   y = x + size;

   while (!feof(fp)) {
      fillz(x, size, sizeof(double));
      if (freadf(x, sizeof(*x), nd, fp)==0)
         break;
      fftr(x, y, size);
      if (out=='P')
         for (k=0; k<size; k++)
            x[k] = x[k]*x[k] + y[k]*y[k];
#ifdef AOP
      else if (out=='A')
         for (k=0; k<size; k++)
            x[k] = sqrt(x[k]*x[k] + y[k]*y[k]);
#endif
      if (out!='I')
         fwritef(x, sizeof(*x), nout, stdout);
      if (out==' ' || out=='I')
         fwritef(y, sizeof(*y), nout, stdout);
   }
   return(0);
}
