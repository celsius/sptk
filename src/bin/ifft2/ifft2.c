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
*    Inverse 2D-FFT for complex sequence                                *
*                                                                       *
*                                       1989    T.Kobayashi             *
*                                       1996.2  N.Miyazaki modified     *
*                                                                       *
*       usage:                                                          *
*               ifft2 [ infile ] [ options ] > stdout                   *
*       options:                                                        *
*               -l l    :  FFT size power of 2      [64]                *
*               +r      :  input is real                                *
*               -t      :  transposed output                            *
*               -c      :  compensated boundary                         *
*               -q      :  first quadrant output                        *
*               -R      :  real part                                    *
*               -I      :  imaginary part                               *
*       infile:                                                         *
*               stdin for default                                       *
*               input is assumed to be complex                          *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id$";

/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SPTK.h>

static int size=64, outopt=0, out=' ', is_fftr=0;

#ifdef DOUBLE
char *FORMAT = "double";
#else
char *FORMAT = "float";
#endif /* DOUBLE */

/*  Command Name  */
static char *cmnd;


int usage(void)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - 2D inverse FFT for complex sequence\n", cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -l l  : FFT size power of 2   [64]\n");
   fprintf(stderr, "       +r    : input is real\n");
   fprintf(stderr, "       -t    : transposed output\n");
   fprintf(stderr, "       -c    : compensated boundary\n");
   fprintf(stderr, "       -q    : first quadrant output\n");
   fprintf(stderr, "       -R    : output real part\n");
   fprintf(stderr, "       -I    : output imaginary part\n");
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       data sequence (%s)         [stdin]\n", FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       IFFT sequence (%s)\n", FORMAT);
#ifdef SPTK_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n", SPTK_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(1);
}


int main (int argc,char *argv[])
{
   FILE *fp;
   char *s, *infile=NULL, c;
   int dft (FILE *fp);
 
   if ((cmnd = strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
   cmnd++;
   
   while (--argc) {
      if (*(s = *++argv)=='-') {
         c = *++s;
         if ((c == 'l') && (*++s=='\0')) {
            s = *++argv;
            --argc;
         }
         switch (c) {
         case 'l':
            size = atoi(s);
            break;
         case 't':
         case 'c':
         case 'q':
            if ((c=='t') || (*++s=='t'))
               outopt = 1;
            if ((c=='c') || (*s=='c'))
               outopt = 2;
            if (c=='q')
               outopt = -1;
            break;
         case 'i':
         case 'r':
            c -= ('a' - 'A');
         case 'I':
         case 'R':
            out = c;
            break;
         case 'h':
         default:
            usage();
         }
      } 
      else if ((*s=='+') && (*(s+1)=='r'))
         is_fftr = 1 - is_fftr;
      else
         infile = s;
   }
 
   if (infile) {
      fp = getfp(infile, "r");
      dft(fp);
      fclose(fp);
   } 
   else
      dft(stdin);
   
   return(0);
}

int dft (FILE *fp)
{
   double *x, *y;
   int size2, nread;
   void trans(double *p);
   
   size2 = size * size;
   
   x = dgetmem(2*size2);
   y = x + size2;
   nread = (is_fftr) ? size2 : 2 * size2;
   
   while (!feof(fp)) {
      if (freadf(x, sizeof(*x), nread, fp)!=nread)
         break;
      if (is_fftr)
         fillz(y,sizeof(*y), size2 * (sizeof(double) / sizeof(long)));
   
      ifft2(x, y, size);
   
      if (out!='I') {
         if (outopt)
            trans(x);
         else
            fwritef(x, sizeof(*x), size2, stdout);
      }
      if (out!='R') {
         if(outopt)
            trans(y);
         else
            fwritef(y, sizeof(*y), size2, stdout);
      }
   }
   
   free(x);
   return(0);
}

void trans (double *p)
{
   int k, sizeh, nout;
   double *q;

   sizeh = size / 2;
   nout = (outopt == 1) ? sizeh : sizeh + 1;

   if (outopt>0)
      for (q=p+sizeh*size,k=sizeh; --k>=0; q+=size) {
         fwritef(q + sizeh, sizeof(*p), sizeh, stdout);
         fwritef(q, sizeof(*p), nout, stdout);
      }
 
   for (q=p,k=nout; --k>=0; q+=size) {
      if (outopt>0)
         fwritef(q + sizeh, sizeof(*p), sizeh, stdout);
      fwritef(q, sizeof(*p), nout, stdout);
   }
}
