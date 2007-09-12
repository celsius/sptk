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
*    FFT for Complex Sequence                                           *
*                                                                       *
*                                       1995.12 N.Isshiki modified      *
*       usage:                                                          *
*               fft [ options ] [ infile ] > stdout                     *
*       options:                                                        *
*               -l l     :  FFT size power of 2         [256]           *
*               -m m     :  order of sequence           [l-1]           *
*               -A       :  amplitude                   [FALSE]         *
*               -R       :  real part                   [FALSE]         *
*               -I       :  imaginary part              [FALSE]         *
*               -P       :  power                       [FALSE]         *
*       infile:                                                         *
*               stdin for default                                       *
*               input is assumed to be double                           *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id$";

/* Standard C Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <SPTK.h>
#include <math.h>
#include <string.h>


/* Default Values */
#define SIZE 256
#ifdef DOUBLE
char *FORMAT = "double";
#else
char *FORMAT = "float";
#endif /* DOUBLE */

/* Command Name */
char *cmnd;


void usage (int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - FFT for complex sequence\n", cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -l l  : FFT size  power of 2 [%d]\n", SIZE);
   fprintf(stderr, "       -m m  : order of sequence    [l-1]\n");
   fprintf(stderr, "       -A    : amplitude            [FALSE]\n");
   fprintf(stderr, "       -R    : real part            [FALSE]\n");
   fprintf(stderr, "       -I    : imaginary part       [FALSE]\n");
   fprintf(stderr, "       -P    : power                [FALSE]\n");
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       data sequence (%s)        [stdin]\n", FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       FFT seqeunce (%s)\n", FORMAT);
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
   int size=SIZE, nd=-1, out=' ';
   int dft (FILE *fp, const int size, const int nd, const int out);
   
   if ((cmnd = strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;

   while (--argc) {
      if (*(s = *++argv)=='-') {
         c = *++s;
         switch(c) {
         case 'l':
            size = atoi(*++argv);
            argc--;
            break;
         case 'm':
            nd = atoi(*++argv) + 1;
            argc--;
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
            fprintf(stderr,"%s: unknown option '%c'\n", cmnd, c);
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
   if (infile) {
      fp = getfp(infile, "r");
      dft(fp,size,nd,out);
      fclose(fp);
   }
   else
      dft(stdin,size,nd,out);
   
   return 0;
}

int dft (FILE *fp, const int size, const int nd, const int out)
{
   double *x, *y;
   int k ,size2;

   x = dgetmem(size2=size+size);
   y = x + size;

   while (!feof(fp)) {
      fillz(x, size2, sizeof(double));
      if (freadf(x, sizeof(*x), nd, fp)==0)
         break;
      if (freadf(y, sizeof(*y), nd, fp)==0)
         break;
      fft(x, y, size);
      if (out=='P')
         for (k=0; k<size; k++)
            x[k] = x[k] * x[k] + y[k] * y[k];
#ifdef AOP
      else if (out=='A')
         for (k=0; k<size; k++)
            x[k] = sqrt(x[k]*x[k] + y[k]*y[k]);
#endif
      if (out!='I')
         fwritef(x, sizeof(*x), size, stdout);
      if (out==' ' || out=='I')
         fwritef(y, sizeof(*y), size, stdout);
   }
   
   return(0);
}
