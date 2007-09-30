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
*    2nd Order Standard Form Digital Filter                             *
*                                                                       *
*                                       1985.8  K.Tokuda                *
*                                       1995.12 N.Isshiki modified      *
*                                                                       *
*       usage:                                                          *
*               df2 [ options ] [ infile ] > stdout                     *
*       options:                                                        *
*               -f f     :  sampling frequency(Hz)        [10000]       *
*               -p f b   :  center frequency f(Hz)                      *
*                            and band width b(Hz) of pole [N/A]         *
*               -z f b   :  center frequency f(Hz)                      *
*                            and band width b(Hz) of zero [N/A]         *
*       note:                                                           *
*               -p and -z option can be specified repeatedly.           *
*                                                                       *
************************************************************************/ 

static char *rcs_id = "$Id: df2.c,v 1.15 2007/09/30 16:20:39 heigazen Exp $";

/* Standard C Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SPTK.h>
#include <string.h>

/* Default Values */
#define SAMPLEF 10000
#define SIZE 2048

/* Command Name */
char *cmnd;


void usage (int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - 2nd order standard form digital filter\n", cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout \n",cmnd);
   fprintf(stderr, "  options:\n");           
   fprintf(stderr, "       -f f   : sampling frequency(Hz)         [%d]\n",SAMPLEF);
   fprintf(stderr, "       -p f b : center frequency f(Hz)\n");
   fprintf(stderr, "                 and band width b(Hz) of pole  [N/A]\n");
   fprintf(stderr, "       -z f b : center frequency f(Hz)\n");
   fprintf(stderr, "                 and band width b(Hz) of zero  [N/A]\n");
   fprintf(stderr, "       -h     : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       filter input (%s)                    [stdin]\n", FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       filter output (%s)\n", FORMAT);
   fprintf(stderr, "  note:\n");
   fprintf(stderr, "       -p and -z option can be specified repeatedly.\n");
#ifdef PACKAGE_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n", PACKAGE_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(status);
}

int main (int argc, char *argv[])
{
   FILE *fpp=stdin;
   int bufp[SIZE], fp=0, fz=0,i,n,fp1,fz1;
   double x;
   double sf=SAMPLEF, af0[SIZE], bf0[SIZE], ab[SIZE], bb[SIZE];
   double buf[SIZE][3];
   double af01,bf01,ab1,bb1;

   if ((cmnd = strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++; 
   while (--argc)
      if (**++argv=='-') {
         argc--;
         switch (*(*argv+1)) {
         case 'f':
            sf = atof(*++argv);
            break;
         case 'z':
            af0[fz] = atof(*++argv);
            argc--;
            ab[fz] = atof(*++argv);
            fz ++;
            break;
         case 'p':
            bf0[fp] = atof(*++argv);
            argc--;
            bb[fp] = atof(*++argv);
            fp ++;
            break;
         case 'h':
            usage(0);
         default:
            fprintf(stderr, "%s : Invalid option '%c'!\n", cmnd, *(*argv+1));
            usage(1);
         }
      }
      else
         fpp = getfp(*argv, "r");

   n = (fp>fz) ? fp : fz;
   for (i=0; i<n; i++) {
      fillz(buf[i], sizeof(**buf), 3);
      bufp[i]=0;
   }

   for (;;) {
      if (freadf(&x, sizeof(x), 1, fpp)!=1)
         break;

      for (i=0; i<n; i++) {
         bf01 = (i>=fp) ? 0 : bf0[i];
         bb1  = (i>=fp) ? 0 : bb[i];
         af01 = (i>=fz) ? 0 : af0[i];
         ab1  = (i>=fz) ? 0 : ab[i];
         fp1  = (i>=fp) ? 0 : 1;
         fz1  = (i>=fz) ? 0 : 1;
      
         x = df2(x, sf, bf01, bb1, af01, ab1, fp1, fz1, buf[i], &bufp[i]);
      }
      fwritef(&x, sizeof(x), 1, stdout);
   }
   
   return 0;
}
