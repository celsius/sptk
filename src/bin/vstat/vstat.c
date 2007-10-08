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
*    Vector Statistics Calculation                                      *
*                                                                       *
*                                      1998.12 T.Masuko                 *
*       usage:                                                          *
*               vstat [ options ] [ infile ] > stdout                   *
*       options:                                                        *
*               -l l     :  length of vector                   [1]      *
*               -n n     :  order of vector                    [1-1]    *
*               -t t     :  number of vector                   [all]    *
*               -o o     :  output format                      [0]      *
*                             0 mean & covariance                       *
*                             1 mean                                    *
*                             2 covariance                              *
*               -d       :  diagonal covariance                [FALSE]  *
*               -i       :  output inverse cov. instead of cov.[FALSE]  * 
*               -r       :  output correlation instead of cov. [FALSE]  *
*       infile:                                                         *
*              data sequence                                            *
*                      x_1(1), ..., x_1(L), x_2(1) ...                  *
*       stdout:                                                         *
*              mean vector                                              *
*                      m(1), ..., m(L),                                 *
*              covariance matrix                                        *
*                      U(11), ..., m(1L),                               *
*                      ...............,                                 *
*                      U(L1), ..., m(LL), ...                           *
*       note:                                                           *
*              if '-d' is specified,                                    *
*              off-diagonal elements are suppressed.                    *
*              '-d' and '-r' are exclusive                              *
*              ('-r' has priority over '-d').                           *
*       require:                                                        *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libralies  */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#if defined(WIN32)
#include "SPTK.h"
#else
#include <SPTK.h>
#endif

/*  Default Values  */
#define LENG    1
#define OUTMEAN TR
#define OUTCOV  TR
#define DIAGC   FA
#define INV     FA
#define CORR    FA

char *BOOL[] = {"FALSE", "TRUE"};

/*  Command Name  */
char *cmnd;


void usage (int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - vector statistics calculation\n",cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -l l   : length of vector                    [%d]\n", LENG);
   fprintf(stderr, "       -n n   : order of vector                     [l-1]\n");
   fprintf(stderr, "       -t t   : number of vector                    [N/A]\n");
   fprintf(stderr, "       -o o   : output format                       [0]\n");
   fprintf(stderr, "                  0 mean & covariance\n");
   fprintf(stderr, "                  1 mean\n");
   fprintf(stderr, "                  2 covariance\n");
   fprintf(stderr, "       -d     : diagonal covariance                 [%s]\n", BOOL[DIAGC]);
   fprintf(stderr, "       -i     : output inverse cov. instead of cov. [%s]\n", BOOL[INV]);
   fprintf(stderr, "       -r     : output correlation instead of cov.  [%s]\n", BOOL[CORR]);
   fprintf(stderr, "       -h     : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       vectors (%s)                [stdin]\n", FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       mean(s) and covariance(s) of input vectors (%s)\n", FORMAT);
   fprintf(stderr, "  note:\n");
   fprintf(stderr, "       if '-d' is specified, off-diagonal elements are suppressed.\n");
   fprintf(stderr, "       '-d' and '-r' are exclusive ('-r' has priority over '-d').\n");
#ifdef PACKAGE_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n",PACKAGE_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(status);
}


int main (int argc,char *argv[])
{
   FILE *fp=stdin;
   double *x, *mean, **cov=NULL, **invcov=NULL, *var=NULL;
   int leng=LENG, nv=-1, i, j, k, lp, m, outtype=0;
   Boolean outmean=OUTMEAN, outcov=OUTCOV, diagc=DIAGC, inv=INV, corr=CORR;
   
   if ((cmnd=strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;
   while (--argc)
      if (**++argv=='-') {
         switch (*(*argv+1)) {
         case 'l':
            leng = atoi(*++argv);
            --argc;
            break;
         case 'n':
            leng = atoi(*++argv)+1;
            --argc;
            break;
         case 't':
            nv = atoi(*++argv);
            --argc;
            break;
         case 'o':
            outtype = atoi(*++argv);
            --argc;
            break;
         case 'd':
            diagc = 1 - diagc;
            break;
         case 'r':
            corr = 1 - corr;
            break;
         case 'h':
            usage (0);
         default:
            fprintf(stderr, "%s : Invalid option '%c'!\n", cmnd, *(*argv+1));
            usage (1);
         }
      }
      else
         fp = getfp(*argv, "r");

   switch (outtype) {
   case 1:
      outcov = FA;
      break;
   case 2:
      outmean = FA;
      break;
   }
   if (diagc && corr)
      diagc = FA;
   if (diagc && inv)
      diagc = FA;
   if (corr && inv)
      corr = FA;

   mean = dgetmem(leng + leng);
   x = mean + leng;
   if (outcov) {
      if (!diagc) {
         cov = (double **)getmem(leng, sizeof(*cov));
         cov[0] = dgetmem(leng*leng);
         for (i=1; i<leng; i++)
            cov[i] = cov[i-1] + leng;
            
         if (inv) {
            invcov = (double **)getmem(leng, sizeof(*invcov));
            invcov[0] = dgetmem(leng*leng);
            for (i=1; i<leng; i++)
               invcov[i] = invcov[i-1] + leng;
         }
      }
      else
         var = dgetmem(leng);
   }

   while (!feof(fp)) {
      for (i=0; i<leng; i++) {
         mean[i] = 0.0;
         if (outcov) {
            if (!diagc)
               for (j=0; j<leng; j++)
                  cov[i][j] = 0.0;
            else
               var[i] = 0.0;
         }
      }
      for (lp=nv; lp; ) {
         if (freadf(x, sizeof(*x), leng, fp)!=leng)
            break;
         for (i=0; i<leng; i++) {
            mean[i] += x[i];
            if (outcov) {
               if (!diagc)
                  for (j=i; j<leng; j++)
                     cov[i][j] += x[i] * x[j];
               else
                  var[i] += x[i] * x[i];
            }
         }
         --lp;
      }
      if (lp==0 || nv==-1) {
         if (nv>0)
            k = nv;
         else
            k = -lp - 1;
         for (i=0; i<leng; i++)
            mean[i] /= k;
         if (outcov) {
            if (!diagc)
               for (i=0; i<leng; i++)
                  for (j=i; j<leng; j++)
                     cov[j][i] = cov[i][j] = cov[i][j]/k - mean[i]*mean[j];
            else
               for (i=0; i<leng; i++)
                  var[i] = var[i]/k - mean[i]*mean[i];
         }
         if (corr) {
            for (i=0; i<leng; i++)
               for (j = i+1; j<leng; j++)
                  cov[j][i] = cov[i][j] = cov[i][j] / sqrt(cov[i][i]*cov[j][j]);
            for (i=0; i<leng; i++)
               cov[i][i] = 1.0;
         }
         
         if (outmean)
            fwritef(mean, sizeof(*mean), leng, stdout);
         
         if (outcov) {
            if (!diagc) {
                if (inv) {
                   for (i=0; i<leng; i++) {
                      for (j=i+1; j<leng; j++) {
                         cov[j][i] /= cov[i][i];
                         for (m=i+1; m<leng; m++)
                            cov[j][m] -= cov[i][m] * cov[j][i];
                      }
                   }

                   for (m=0; m<leng; m++) {
                      for (i=0; i<leng; i++) {
                         if (i==m)
                            invcov[i][m] = 1.0;
                         else
                            invcov[i][m] = 0.0;
                      }
                      for (i=0; i<leng; i++) {
                         for (j=i+1; j<leng; j++) 
                            invcov[j][m] -= invcov[i][m] * cov[j][i];
                      }
                      for (i=leng-1; i>=0; i--) {
                         for (j=i+1; j<leng; j++) 
                            invcov[i][m] -= cov[i][j] * invcov[j][m];
                         invcov[i][m] /= cov[i][i];
                      }
                   }
                   fwritef(invcov[0], sizeof(*invcov[0]), leng*leng, stdout);
                }
                else 
                   fwritef(cov[0], sizeof(*cov[0]), leng*leng, stdout);
            }
            else
               fwritef(var, sizeof(*var), leng, stdout);
         }
      }
   }
   
   return(0);
}
