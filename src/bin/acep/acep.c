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
*    Adaptive Cepstral Analysis                                         *
*                                                                       *
*                                       1993.8  K.Tokuda                *
*                                       1996.1  K.Koishida              *
*                                                                       *
*       usage:                                                          *
*               acep [ options ] [ pefile ] < stdin > stdout            *
*       options:                                                        *
*               -m m     :  order of cepstrum           [25]            *
*               -l l     :  leakage factor              [0.98]          *
*               -t t     :  momentum constant           [0.9]           *
*               -k k     :  step size                   [0.1]           *
*               -p p     :  output period of cepstrum   [1]             *
*               -s       :  output smoothed cepstrum    [FALSE]         *
*               -e e     :  minimum value for epsilon   [0]             *
*               -P P     :  order of Pade approximation [4]             *
*       infile:                                                         *
*               data sequence                                           *
*                   , x(0), x(1), ...                                   *
*       stdout:                                                         *
*               cepstrum                                                *
*                   , c(0), c(1), ..., c(M),                            *
*       output:                                                         *
*               prediction error (if pefile is specified)               *
*                   , e(0), e(1), ...                                   *
*       note:                                                           *
*               P = 4 or 5                                              *
*       require:                                                        *
*               lmadf()                                                 *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id$";


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
#define PADEORD 4
#define LAMBDA 0.98
#define STEP 0.1
#define PERIOD 1
#define AVEFLAG FA
#define TAU 0.9
#define EPS 0.0

char *BOOL[] = {"FALSE", "TRUE"};

/*  Command Name  */
char *cmnd;


void usage (const int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - adaptive cepstral analysis\n",cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ pefile ] < stdin > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -m m  : order of cepstrum           [%d]\n", ORDER);
   fprintf(stderr, "       -l l  : leakage factor              [%g]\n", LAMBDA);
   fprintf(stderr, "       -t t  : momentum constant           [%g]\n", TAU);
   fprintf(stderr, "       -k k  : step size                   [%g]\n", STEP);
   fprintf(stderr, "       -p p  : output period of cepstrum   [%d]\n", PERIOD);
   fprintf(stderr, "       -s    : output smoothed cepstrum    [%s]\n", BOOL[AVEFLAG]);
   fprintf(stderr, "       -e e  : minimum value for epsilon   [%g]\n", EPS);
   fprintf(stderr, "       -P P  : order of Pade approximation [%d]\n", PADEORD);
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  stdin:\n");
   fprintf(stderr, "       data sequence (%s)\n", FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       cepstrum (%s)\n", FORMAT);
   fprintf(stderr, "  pefile:\n");
   fprintf(stderr, "       prediction error (%s)\n", FORMAT);
   fprintf(stderr, "  note:\n");
   fprintf(stderr, "       P = 4 or 5\n");
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
   int m=ORDER, period=PERIOD, i, j, pd=PADEORD;
   FILE *fp=stdin, *fpe=NULL;
   Boolean aveflag=AVEFLAG;
   double lambda=LAMBDA, step=STEP, tau=TAU, eps=EPS,
          *c, *e, *ep, *cc, *d, *avec, 
          x, ll, gg, tt, mu, ttx;

   if ((cmnd = strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;

   while (--argc)
      if (**++argv == '-') {
         switch (*(*argv+1)) {
         case 'l':
            lambda = atof(*++argv);
            --argc;
            break;
         case 't':
            tau = atof(*++argv);
            --argc;
            break;
         case 'k':
            step = atof(*++argv);
            --argc;
            break;
         case 'm':
            m = atoi(*++argv);
            --argc;
            break;
         case 'p':
            period = atoi(*++argv);
            --argc;
            break;
         case 's':
            aveflag = 1 - aveflag;
            break;
         case 'P':
            pd = atoi(*++argv);
            --argc;
            break;
         case 'e':
            eps = atof(*++argv);
            --argc;
            break;
         case 'h':
            usage(0);
         default:
            fprintf(stderr, "%s : Invalid option '%c'!\n", cmnd, *(*argv+1));
            usage(1);
         }
      }
      else 
         fpe = getfp(*argv, "wb");

   if ((pd < 4)||(pd > 5)) {
      fprintf(stderr,"%s : Order of Pade approximation should be 4 or 5!\n",cmnd);
      return(1);
   }

   c  = dgetmem(5*(m+1)+(m+1)*pd*2);
   cc = c  + m + 1;
   e  = cc + m + 1;
   ep = e  + m + 1;
   avec = ep + m + 1;
   d = avec + m + 1;

   j  = period;
   ll = 1.0 - lambda;
   gg = 1.0;
   step /= (double)m;
   tt = 2 * (1.0 - tau);
    
   while (freadf(&x, sizeof(x), 1, fp) == 1) {
      for (i=1; i<=m; i++)
         cc[i] = -c[i];

      x = lmadf(x, cc, m, pd, d);

      for (i=m; i>=1; i--)
         e[i] = e[i-1];
      e[0] = x;

      gg = gg * lambda + ll * e[0] * e[0];
      c[0] = 0.5 * log(gg);

      gg = (gg < eps) ? eps : gg;
      mu = step / gg;
      ttx = tt * e[0]; 
   
      for (i=1; i<=m; i++) {
         ep[i] = tau * ep[i] - ttx * e[i];
         c[i] -= mu * ep[i];
      }

      if (aveflag)
         for (i=0; i<=m; i++)
            avec[i] += c[i];

      if (fpe != NULL)
         fwritef(&x, sizeof(x), 1, fpe);

      if (--j==0) {
         j = period;
         if (aveflag) {
            for (i=0; i<=m; i++)
               avec[i] /= period;
            fwritef(avec, sizeof(*avec), m+1, stdout);
            fillz(avec, sizeof(*avec), m+1);
         }
         else
            fwritef(c, sizeof(*c), m+1, stdout);
      }
   }
   return(0);
}
