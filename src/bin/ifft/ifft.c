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
*    Inverse FFT for Complex Sequence                                   *
*                                                                       *
*       usage:                                                          *
*               ifft [ options ] [ infile ] > stdout                    *
*                                                                       *
*       options:                                                        *
*               -l l    :  FFT size power of 2      [256]               *
*               -R      :  real part                [FALSE]             *
*               -I      :  imaginary part           [FALSE]             *
*       infile:                                                         *
*               stdin for default                                       *
*               input is assumed to be real                             *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id: ifft.c,v 1.13 2007/09/21 15:18:57 heigazen Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SPTK.h>
#include <string.h>

static int size=256, out=' ';

#ifdef DOUBLE
char *FORMAT = "double";
#else
char *FORMAT = "float";
#endif /* DOUBLE */

/*  Command Name  */
char *cmnd;


int usage (void)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - inverse FFT for complex sequence\n", cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -l l  : FFT size power of 2 [256]\n");
   fprintf(stderr, "       -R    : real part           [FALSE]\n");
   fprintf(stderr, "       -I    : imaginary part      [FALSE]\n");
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       data sequence (%s)       [stdin]\n", FORMAT);
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
   int dft(FILE *fp);
 
   if ((cmnd = strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;
      
   while (--argc) {
      if (*(s = *++argv)=='-') {
         c = *++s;
         if ((c=='l') && (*++s=='\0')) {
            s = *++argv;
            --argc;
         }
         switch (c) {
         case 'l':
            size = atoi(s);
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
   
   return 0;
}

int dft (FILE *fp)
{
   double *x, *y;
   int size2;

   x = dgetmem(size2=size+size);

   y = x + size;

   while (!feof(fp)) {
      if (freadf(x, sizeof(*x), size2, fp)!=size2)
         break;
   
      ifft(x, y, size);

      if (out!='I')
         fwritef(x, sizeof(*x), size, stdout);
      if (out!='R')
         fwritef(y, sizeof(*y), size, stdout);
   }
   
   return(0);
}
