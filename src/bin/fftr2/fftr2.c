/*
  ---------------------------------------------------------------  
            Speech Signal Processing Toolkit (SPTK)

                      SPTK Working Group                           
                                                                   
                  Department of Computer Science                   
                  Nagoya Institute of Technology                   
                               and                                 
   Interdisciplinary Graduate School of Science and Engineering    
                  Tokyo Institute of Technology                    
                                                                   
                     Copyright (c) 1984-2007                       
                       All Rights Reserved.                        
                                                                   
  Permission is hereby granted, free of charge, to use and         
  distribute this software and its documentation without           
  restriction, including without limitation the rights to use,     
  copy, modify, merge, publish, distribute, sublicense, and/or     
  sell copies of this work, and to permit persons to whom this     
  work is furnished to do so, subject to the following conditions: 
                                                                   
    1. The source code must retain the above copyright notice,     
       this list of conditions and the following disclaimer.       
                                                                   
    2. Any modifications to the source code must be clearly        
       marked as such.                                             
                                                                   
    3. Redistributions in binary form must reproduce the above     
       copyright notice, this list of conditions and the           
       following disclaimer in the documentation and/or other      
       materials provided with the distribution.  Otherwise, one   
       must contact the SPTK working group.                        
                                                                   
  NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSTITUTE OF TECHNOLOGY,   
  SPTK WORKING GROUP, AND THE CONTRIBUTORS TO THIS WORK DISCLAIM   
  ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL       
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT   
  SHALL NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSTITUTE OF         
  TECHNOLOGY, SPTK WORKING GROUP, NOR THE CONTRIBUTORS BE LIABLE   
  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY        
  DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,  
  WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTUOUS   
  ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR          
  PERFORMANCE OF THIS SOFTWARE.                                    
                                                                   
  ---------------------------------------------------------------  
*/

/************************************************************************
*                                                                       *
*    2D-FFT for Real Sequence                                           *
*                                                                       *
*                                       1989    T.Kobayashi             *
*                                       1996.2  N.Miyazaki modified     *
*                                                                       *
*       usage:                                                          *
*                fftr2 [ infile ] [ options ] > stdout                  *
*       options:                                                        *
*                -l l     :  FFT size. power of 2        [64]           *
*                -m m1 m2 :  region of support           [m1=64,m2=m1]  *
*                -t       :  transposed output           [FALSE]        *
*                -c       :  compensated boundary        [FALSE]        *
*                -q       :  first quadrant output       [FALSE]        *
*                -A       :  amplitude                   [FALSE]        *
*                -R       :  real part                   [FALSE]        *
*                -I       :  imaginary part              [FALSE]        *
*                -P       :  power                       [FALSE]        *
*       infile:                                                         *
*                stdin for default                                      *
*                input is assumed to be double                          *
************************************************************************/

static char *rcs_id = "$Id: fftr2.c,v 1.18 2007/10/08 16:49:29 heigazen Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#if defined(WIN32)
#include "SPTK.h"
#else
#include <SPTK.h>
#endif

/*  Command Name  */
static char *cmnd;


static int size=64, outopt=0, n1=0, n2=0, out=' ';

int usage (void)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - 2D-FFT for real sequence\n", cmnd); 
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -l l      : FFT size power of 2   [64]\n");
   fprintf(stderr, "       -m m1 m2  : region of support     [m1=64, m2=m1]\n");
   fprintf(stderr, "       -t        : transposed output     [FALSE]\n");
   fprintf(stderr, "       -c        : compensated boundary  [FALSE]\n");
   fprintf(stderr, "       -q        : first quadrant output [FALSE]\n");
   fprintf(stderr, "       -A        : amplitude             [FALSE]\n");
   fprintf(stderr, "       -R        : real part             [FALSE]\n");
   fprintf(stderr, "       -I        : imaginary part        [FALSE]\n");
   fprintf(stderr, "       -P        : power                 [FALSE]\n");
   fprintf(stderr, "       -h        : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       data sequence (%s)             [stdin]\n", FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       FFT sequence (%s)\n", FORMAT);
#ifdef PACKAGE_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n", PACKAGE_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(1);
}

int main (int argc, char *argv[])
{
   FILE *fp;
   char *s, *infile=NULL, c;
   double *x, *y;
   double *xp, *yp;
   void trans (double *p);

   int size2;
   int i, k;

   if ((cmnd = strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;
      
   while (--argc) {
      if (*(s = *++argv)=='-') {
         c = *++s;
         if ((c == 'l' || c == 'm') && *++s == '\0') {
            s = *++argv;
            --argc;
         }
         switch(c) {
         case 'l':
            size = atoi(s);
            break;
         case 'm':
            n1 = atoi(s);
            if (argc==1) {
               n2 = n1;
            }
            else {
               s = *++argv;    
               argc--;
               if (*s>='0' && *s<='9') {
                  n2 = atoi(s);
               } 
               else {
                  n2 = n1;
                  s = *--argv;
                  argc++;
               }
            }
            break;
         case 't':
         case 'c':
         case 'q':
            if (c=='t' || *++s=='t')
               outopt = 1;
            if (c=='c' || *s=='c')
               outopt = 2;
            if (c=='q')
               outopt = -1;
            break;
         case 'a':
         case 'p':
         case 'r':
         case 'i':
            c -= ('a' - 'A');
         case 'A':
         case 'P':
         case 'R':
         case 'I':
            out = c;
            break;
         case 'h':
            usage();
         default:
            usage();
         }
      }
      else
         infile = s;
   }

   if (n1>size) {
      fprintf(stderr, "%s : Region size %d should be less than the FFT size %d!\n", cmnd, n1, size);
      return(1);
   }
   if (n2>size) {
      fprintf(stderr, "%s : Region size %d should be less than the FFT size %d!\n", cmnd, n2, size);
      return(1);
   }

   if (infile)
      fp = getfp(infile, "r");
   else
      fp = stdin;

 
   size2 = size * size;

   x = dgetmem(2*size2);
   y = x + size2;


   size2 = size * size;

   x = dgetmem(2*size2);
   y = x + size2;

   while (!feof(fp)) {
      if(n1) {
         for (xp=x,k=n2; --k>=0; xp+=size) {
            if (freadf(xp, sizeof(*x), n1, fp) != n1)
               return(-1);
            if (n1<size)
               fillz(xp + n1, sizeof(*xp), size - n1);
         }
      }
      else {
         if ((k=freadf(x, sizeof(*x), size2, fp))==0)
            break;
         n2 = n1 = sqrt((double)k);
         if (k!=n1*n1) {
            fprintf(stderr, "%s : Region of support is not square!\n", cmnd);
            return(-1);
         }
         if (n1<size) {
            fillz(yp = x + size * n1,sizeof(*yp), size * (size - n1));
            yp -= (size - n1);
            xp = x + k;
            for (k=n1; --k>=0; yp-=(size-n1)) {
               fillz(yp,sizeof(*yp), size - n1);
               for (i=n1; --i>=0; )
                  *--yp = *--xp;
            }
         }
      }
     
      fftr2(x, y, size);

      if (out=='P') {
         for (k=0; k<size2; k++)
            x[k] = x[k]*x[k] + y[k]*y[k];
      }
      else if (out=='A') {
         for (k=0; k<size2; k++)
            x[k] = sqrt(x[k]*x[k] + y[k]*y[k]);
         if (out!='I') {
            if (outopt)
               trans(x);
            else
               fwritef(x, sizeof(*x), size2, stdout);
         }
      }
         
      if (out == ' ' || out == 'I') {
         if (outopt)
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
   nout = (outopt==1) ? sizeh : sizeh+1;

   if (outopt>0) {
      for (q=p+sizeh*size,k=sizeh; --k>=0; q+=size) {
         fwritef(q+sizeh, sizeof(*p), sizeh, stdout);
         fwritef(q, sizeof(*p), nout, stdout);
      }
      for (q=p,k=nout; --k>=0; q+=size) {
         if (outopt>0)
            fwritef(q+sizeh, sizeof(*p), sizeh, stdout);
         fwritef(q, sizeof(*p), nout, stdout);
      }
   }
   
   return;
}
