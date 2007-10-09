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
*    Calculation of Cepstral Distance                                   *
*                                       1996.7  K.Koishida              *
*                                                                       *
*       usage:                                                          *
*               cdist [ options ] cfile [ infile ] > stdout             *
*       options:                                                        *
*               -m m     :  order of minimum-phase cepstrum   [25]      *
*               -o o     :  output format                     [0]       *
*                               0 ([dB])                                *
*                               1 (squared error)                       *
*                               2 (root squared error)                  *
*               -f       :  frame length                      [FALSE]   *
*       cfile:                                                          *
*       infile:                                                         *
*               minimum-phase cepstrum                                  *
*                          , c(0), c(1), ..., c(m),                     *
*       stdout:                                                         *
*               cepstral distance                                       *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id: cdist.c,v 1.17 2007/10/09 10:07:10 heigazen Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#if defined(WIN32)
#include "SPTK.h"
#else
#include <SPTK.h>
#endif

/*  Default Values  */
#define ORDER 25
#define FRAME FA
#define OTYPE 0

char *BOOL[] = {"FALSE", "TRUE"};

/*  Command Name  */
char *cmnd;


void usage (int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - calculation of cepstral distance\n",cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] cfile [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "      -m m  : order of minimum-phase cepstrum [%d]\n", ORDER);
   fprintf(stderr, "      -o o  : output format                   [%d]\n", OTYPE);
   fprintf(stderr, "                0 ([dB])\n");
   fprintf(stderr, "                1 (squared error)\n");
   fprintf(stderr, "                2 (root squared error)\n");
   fprintf(stderr, "      -f    : output frame by frame           [%s]\n", BOOL[FRAME]);
   fprintf(stderr, "      -h    : print this message\n");
   fprintf(stderr, "  cfile:\n");
   fprintf(stderr, "  infile:                                     [stdin]\n");
   fprintf(stderr, "      minimum-phase cepstrum (%s)\n", FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "      cepstral distance (%s)\n", FORMAT);
#ifdef PACKAGE_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n",PACKAGE_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(status);
}

int main (int argc, char **argv)
{
   int  m=ORDER, num=0, otype=OTYPE, i;
   FILE *fp=stdin, *fp1=NULL;
   double *x, *y, sub, sum, z=0.0;
   Boolean frame=FRAME;
    
   if ((cmnd = strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;
   while (--argc)
      if (**++argv=='-') {
         switch (*(*argv+1)) {
         case 'm':
            m = atoi(*++argv);
            --argc;
            break;
         case 'o':
            otype = atoi(*++argv);
            --argc;
            break;
         case 'f':
            frame = 1 - frame;
            break;
         case 'h':
            usage(0);
         default:
            fprintf(stderr, "%s : Invalid option '%c'!\n", cmnd, *(*argv+1));
            usage(1);
         }
      }
      else if (fp1 == NULL)
         fp1 = getfp(*argv, "rb");
      else
         fp = getfp(*argv, "rb");

   x = dgetmem(m+m+2);
   y = x + m + 1;

   while (freadf(x, sizeof(*x), m+1, fp) == m+1 && freadf(y, sizeof(*y), m+1, fp1)==m+1) {
      sum = 0.0;
      for (i=1; i<=m; i++) {
         sub = x[i] - y[i];
         sum += sub * sub;
      }

      if (otype==0) {
         sum = sqrt(2.0*sum);
         sum *= LN_TO_LOG;
      }
      else if (otype==2)
         sum = sqrt(sum);

      if (!frame) {
         z += sum;
         num++;
      }
      else
         fwritef(&sum, sizeof(sum), 1, stdout);
   }

   if (!frame) {
      z = z / (double)num;
      fwritef(&z, sizeof(z), 1, stdout);
   }
    
   return 0;
}
