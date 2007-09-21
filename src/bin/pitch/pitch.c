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
*    Pitch Extraction                                                   *
*                                                                       *
*                                      1998.7  M.Tamura                 *
*                                      2000.3  T.Tanaka                 *
*                                                                       *
*       usage:                                                          *
*               pitch [ options ] [ infile ] > stdout                   *
*       options:                                                        *
*               -s  s     :  sampling frequency            [10]         *
*               -l  l     :  frame length                  [400]        *
*               -t  t     :  voiced/unvoiced threshhold    [6.0]        *
*               -L  L     :  minimum fundamental frequency [60]         *
*                            to search for (Hz)                         *
*               -H  H     :  maximum fundamental frequency [240]        *
*                            to search for (Hz)                         *
*               -e  e     :  small value for calculate log [0]          *
*                            spectral envelope                          *
*               (level 2  :  for uels cepstral analysis)                *
*               -i  i     :  minimum number of iteration   [2]          *
*               -j  j     :  maximum number of iteration   [30]         *
*               -d  d     :  end condition                 [0.1]        *
*       infile:                                                         *
*               data sequence                                           *
*                       , x(0), x(1), ..., x(n-1),                      *
*       stdout:                                                         *
*               pitch                                                   *
*               p(t)                                                    *
*       require:                                                        *
*               pitch()                                                 *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id: pitch.c,v 1.19 2007/09/21 15:18:55 heigazen Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SPTK.h>


/*  Default Values  */
#define FREQ   10
#define ILNG   400
#define THRESH 6.0
#define LOW    60
#define HIGH   240
#define EPS    0.0


/*  Default Values for uels */
#define MINITR 2
#define MAXITR 30
#define END    0.1

#ifdef DOUBLE
char *FORMAT = "double";
#else
char *FORMAT = "float";
#endif /* DOUBLE */

/*  Command Name  */
char *cmnd;


void usage (int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - pitch extraction\n",cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -s s  : sampling frequency (kHz)        [%d]\n", FREQ);
   fprintf(stderr, "       -l l  : frame length                    [%d]\n", ILNG);
   fprintf(stderr, "       -t t  : voiced/unvoiced threshold       [%.1f]\n", THRESH);
   fprintf(stderr, "       -L L  : minimum fundamental             [%d]\n", LOW);
   fprintf(stderr, "               frequency to search for (Hz)\n");
   fprintf(stderr, "       -H H  : maximum fundamental             [%d]\n", HIGH);
   fprintf(stderr, "               frequency to search for (Hz)\n");
   fprintf(stderr, "       -e e  : small value for calculate       [%g]\n", EPS);
   fprintf(stderr, "               log-spectral envelope\n");
   fprintf(stderr, "     (level 2 : for uels cepstral analysis)\n");
   fprintf(stderr, "       -i i  : minimum number of iteration     [%d]\n", MINITR);
   fprintf(stderr, "       -j j  : maximum number of iteration     [%d]\n", MAXITR);
   fprintf(stderr, "       -d d  : end condition                   [%g]\n", END);
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       windowed sequence (%s)             [stdin]\n", FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       pitch (%s)\n", FORMAT);
#ifdef SPTK_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n",SPTK_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(status);
}


int main (int argc, char **argv)
{
   int freq=FREQ, n=ILNG, l, L=LOW , H=HIGH, m, itr1=MINITR, itr2=MAXITR, low, high;
   double *x, eps=EPS, p, thresh=THRESH, end=END;
   FILE *fp=stdin;

   if ((cmnd=strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;
   while (--argc)
      if (**++argv=='-') {
         switch (*(*argv+1)) {
         case 's':
            freq = atoi(*++argv);
            --argc;
            break;
         case 'l':
            n = atoi(*++argv);
            --argc;
            break;
         case 't':
            thresh = atof(*++argv);
            --argc;
            break;
         case 'L':
            L = atoi(*++argv);
            --argc;
            break;
         case 'H':
            H = atoi(*++argv);
            --argc;
            break;
         case 'e':
            eps = atof(*++argv);
            --argc;
            break;
         case 'i':
            itr1 = atoi(*++argv);
            --argc;
         case 'j':
            itr2 = atoi(*++argv);
            --argc;
         case 'd':
            end = atof(*++argv);
            --argc;
         case 'h':
            usage (0);
         default:
            fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
            usage (1);
         }
      }
      else
         fp = getfp(*argv, "r");

   low = freq * 1000 / H;
   high = freq * 1000 / L;
   m = (freq * 25)/10;
   l = 1;
   while (l<n)l+=l;

   x = dgetmem(l);

   while (freadf(x, sizeof(*x), n, fp)==n) {
      fillz(x+n,l-n,sizeof(double));
      p = pitch(x, l, thresh, low, high, eps, m, itr1, itr2, end);
      fwritef(&p, sizeof(p), 1, stdout);
   }
   
   return(0);
}

