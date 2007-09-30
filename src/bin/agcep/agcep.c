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

/******************************************************************************
*                                                                             *
*    Adaptive Generalized Cepstral Analysis                                   *
*                                                                             *
*                                       1991.11 K.Tokuda                      *
*                                       1996.1  K.Koishida                    *
*                                                                             *
*       usage:                                                                *
*               agcep [ options ] [ pefile ] < stdin > stdout                 *
*       options:                                                              *
*               -m m     :  order of generalized cepstrum          [25]       *
*               -g g     :  -1 / gamma                             [1]        *
*               -l l     :  leakage factor                         [0.98]     *
*               -t t     :  momentum constant                      [0.9]      *
*               -k k     :  step size                              [0.1]      *
*               -p p     :  output period of generalized cepstrum  [1]        *
*               -s       :  output smoothed generalized cepstrum   [FALSE]    *
*               -n       :  output normalized generalized cepstrum [FALSE]    *
*               -e e     :  minimum value for epsilon              [0]        *
*       infile:                                                               *
*               data sequence                                                 *
*                   , x(0), x(1), ...                                         *
*       stdout:                                                               *
*               generalized cepstrum                                          *
*                   , c(0), c(1), ..., c(m),                                  *
*       output:                                                               *
*               prediction error (if pefile is specified)                     *
*                   , e(0), e(1), ...                                         *
*       require:                                                              *
*               iglsadf1(), ignorm()                                          *
*                                                                             *
******************************************************************************/

static char *rcs_id = "$Id: agcep.c,v 1.18 2007/09/30 16:20:29 heigazen Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SPTK.h>


/*  Default Values  */
#define ORDER 25
#define STAGE 1
#define LAMBDA 0.98
#define STEP 0.1
#define PERIOD 1
#define AVERAGE FA
#define NORM FA
#define TAU 0.9
#define EPS 0.0

char *BOOL[] = {"FALSE", "TRUE"};

/*  Command Name  */
char *cmnd;


void usage (int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - adaptive generalized cepstral analysis\n",cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ pefile ] < stdin > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -m m  : order of generalized cepstrum          [%d]\n", ORDER);
   fprintf(stderr, "       -g g  : -1 / gamma                             [%d]\n", STAGE);
   fprintf(stderr, "       -l l  : leakage factor                         [%g]\n", LAMBDA);
   fprintf(stderr, "       -t t  : momentum constant                      [%g]\n", TAU);
   fprintf(stderr, "       -k k  : step size                              [%g]\n", STEP);
   fprintf(stderr, "       -p p  : output period of generalized cepstrum  [%d]\n", PERIOD);
   fprintf(stderr, "       -s    : output smoothed generalized cepstrum   [%s]\n", BOOL[AVERAGE]);
   fprintf(stderr, "       -n    : output normalized generalized cepstrum [%s]\n", BOOL[NORM]);
   fprintf(stderr, "       -e e  : minimum value for epsilon              [%g]\n", EPS);
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  stdin:\n");
   fprintf(stderr, "       data sequence (%s)\n", FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       generalized cepstrum (%s)\n", FORMAT);
   fprintf(stderr, "  pefile:\n");
   fprintf(stderr, "       prediction error (%s)\n", FORMAT);
#ifdef PACKAGE_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n",PACKAGE_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif /* PACKAGE_VERSION */
   fprintf(stderr, "\n");
   exit(status);
}


int main (int argc, char **argv)
{
   int m=ORDER, period=PERIOD, stage=STAGE, i, j;
   FILE *fp=stdin, *fpe=NULL;
   Boolean ave=AVERAGE, norm=NORM;
   double lambda=LAMBDA, step=STEP, eps=EPS,
          *c, *cc, *eg, *ep, *d, *avec, tau=TAU,
          x, ee, ll, gg, mu, gamma, tt, ttx;

   if ((cmnd = strrchr(argv[0], '/')) == NULL)
      cmnd = argv[0];
   else
      cmnd++;
   while (--argc)
      if (**++argv=='-') {
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
      case 'g':
         stage = atoi(*++argv);
         --argc;
         break;
      case 'p':
         period = atoi(*++argv);
         --argc;
         break;
      case 's':
         ave = 1 - ave;
         break;
      case 'n':
         norm = 1 - norm;
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
      fpe = getfp(*argv, "w");

   if (stage==0) {
      fprintf(stderr, "%s : gamma should not equal to 0!\n", cmnd);
      usage(1);
   }
   gamma = -1.0 / (double)stage;
         
   c  = dgetmem(5*(m+1)+m*stage);
   cc = c  + m + 1;
   eg = cc + m + 1;
   ep = eg + m + 1;
   avec = ep + m + 1;
   d = avec  + m + 1;
   
   j  = period;
   ll = 1.0 - lambda;
   gg = 1.0;
   ee = 1.0;
   step /= (double)m;
   tt = 2 * (1.0 - tau);
    
   while (freadf(&x, sizeof(x), 1, fp)==1) {
      eg[m] = d[stage*m-1];
      x = iglsadf1(x, c, m, stage, d);

      movem(d+(stage-1)*m, eg, sizeof(*d), m);
   
      gg = lambda * gg  + ll*eg[0]*eg[0];
      gg = (gg<eps) ? eps : gg;
      mu = step / gg;
      ttx = tt * x;
   
      for (i=1; i<=m; i++) {
         ep[i] = tau * ep[i] - ttx * eg[i];
         c[i] -= mu * ep[i];
      }

      ee = lambda * ee + ll * x * x;
      c[0] = sqrt(ee);

      if (ave)
         for (i=0; i<=m; i++)
            avec[i] += c[i];

      if (fpe!=NULL)
         fwritef(&x, sizeof(x), 1, fpe);
   
      if (--j==0) {
         j = period;
         if (ave) {
            for (i=0; i<=m; i++)
               avec[i] /= period;
            if(!norm)
               ignorm(avec, cc, m, gamma);
            fwritef(cc, sizeof(*cc), m+1, stdout);
            fillz(avec, sizeof(*avec), m+1);
         }
         else if (!norm) {
            ignorm(c, cc, m, gamma);
            fwritef(cc, sizeof(*cc), m+1, stdout);
         }
         else
            fwritef(c, sizeof(*c), m+1, stdout);
      }
   }
   return(0);
}
