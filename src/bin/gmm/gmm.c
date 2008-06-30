/* ----------------------------------------------------------------- */
/*             The Speech Signal Processing Toolkit (SPTK)           */
/*             developed by SPTK Working Group                       */
/*             http://sp-tk.sourceforge.net/                         */
/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 1984-2007  Tokyo Institute of Technology           */
/*                           Interdisciplinary Graduate School of    */
/*                           Science and Engineering                 */
/*                                                                   */
/*                1996-2008  Nagoya Institute of Technology          */
/*                           Department of Computer Science          */
/*                                                                   */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/* - Redistributions of source code must retain the above copyright  */
/*   notice, this list of conditions and the following disclaimer.   */
/* - Redistributions in binary form must reproduce the above         */
/*   copyright notice, this list of conditions and the following     */
/*   disclaimer in the documentation and/or other materials provided */
/*   with the distribution.                                          */
/* - Neither the name of the SPTK working group nor the names of its */
/*   contributors may be used to endorse or promote products derived */
/*   from this software without specific prior written permission.   */
/*                                                                   */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS */
/* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          */
/* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY    */
/* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           */
/* POSSIBILITY OF SUCH DAMAGE.                                       */
/* ----------------------------------------------------------------- */

/*************************************************************************
 *                                                                       *
 *    Gaussian Mixture Model                                             *
 *                                                                       *
 *                                       2000.7  C. Miyajima             *
 *                                                                       *
 *       usage:                                                          *
 *               gmm [options] [infile] > stdout                         *
 *       options:                                                        *
 *               -l l  :  length of vector                    [26]       *
 *               -m m  :  number of Gaussian components       [16]       *
 *               -t t  :  number of training vectors          [EOF]      *
 *               -a a  :  minimum number of EM iterations     [0]        *
 *               -b b  :  maximum number of EM iterations     [20]       *
 *               -e e  :  convergence factor                  [0.00001]  *
 *               -v v  :  floor value of variances            [0.001]    *
 *               -w w  :  floor value of weights (1/m)*w      [0.001]    *
 *       infile:                                                         *
 *               training data sequence                       [stdin]    *
 *       stdout:                                                         *
 *               GMM parameters                                          *
 *       notice:                                                         *
 *               -t option can be omitted, when input by its file name   *
 *                or from redirect                                       *
 *                                                                       *
 ************************************************************************/

static char *rcs_id = "$Id$";

/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>

#ifdef HAVE_STRING_H
#  include <string.h>
#else
#  include <strings.h>
#  ifndef HAVE_STRRCHR
#     define strrchr rindex
#  endif
#endif

#include <math.h>

#if defined(WIN32)
#  include "SPTK.h"
#else
#  include <SPTK.h>
#endif

#include "gmm.h"

/*  Default Values  */
#define DEF_L       26
#define DEF_M       16
#define DEF_T       -1
#define DEF_IMIN    0
#define DEF_IMAX    20
#define DEF_E       0.00001
#define DEF_V       0.001
#define DEF_W       0.001
#define DELTA       0.0001
#define END         0.0001
#define EPSILON     1.0e-6
#define MAXVALUE    1.0e10

char *BOOL[] = {"FALSE", "TRUE"};

/*  Command Name  */
char    *cmnd;

void usage (int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - EM estimation of GMM\n",cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -l l  : dimension of vector                [%d]\n", DEF_L);
   fprintf(stderr, "       -m m  : number of Gaussian components      [%d]\n", DEF_M);
   fprintf(stderr, "       -t t  : number of training vectors         [N/A]\n");
   fprintf(stderr, "       -a a  : minimum number of EM iterations    [%d]\n", DEF_IMIN);
   fprintf(stderr, "       -b b  : maximum number of EM iterations    [%d]\n", DEF_IMAX);
   fprintf(stderr, "       -e e  : convergence factor                 [%g]\n", DEF_E);
   fprintf(stderr, "       -v v  : flooring value for variance        [%g]\n", DEF_V);
   fprintf(stderr, "       -w w  : flooring value for weights (1/m)*w [%g]\n", DEF_W);    
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       training data sequence (float)             [stdin]\n");
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       GMM parameters (float)\n");
   fprintf(stderr, "         output order:\n");
   fprintf(stderr, "            weight-1, weight-2, ..., weight-m, mean_vec-1, variance_vec-1,\n");
   fprintf(stderr, "            mean_vec-2, variance_vec-2, ..., mean_vec-m, variance_vec-m\n");
   fprintf(stderr, "  notice:\n");
   fprintf(stderr, "       -t option can be omitted when training data is given by\n");
   fprintf(stderr, "         its file name or by file redirection with symbol \"<\".\n");
   fprintf(stderr, "       -e option specifies a threshold for the change of average\n");
   fprintf(stderr, "         log-likelihood for training data at each iteration.\n");
#ifdef PACKAGE_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n", PACKAGE_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(status);
}

int main (int argc, char **argv)
{
   FILE *fp=stdin;
   GMM gmm, tgmm;
   double E=DEF_E, V=DEF_V, W=DEF_W, 
          *dat, *pd, *cb, *icb, *logwgd, logb, *sum, diff, sum_w,
          ave_logp0, ave_logp1, change = MAXVALUE, tmp1, tmp2;
   int ispipe, l, L=DEF_L , m, M=DEF_M, N, t, T=DEF_T,
       i, Imin=DEF_IMIN, Imax=DEF_IMAX, *tindex, *cntcb;

   double cal_gconst (double *var, const int D);
   void fillz_gmm (GMM *gmm, const int M, const int L);
   double log_wgd (GMM gmm, const int m, double *dat, const int L);
   double log_add (double logx, double logy);

   if ((cmnd=strrchr(argv[0],'/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;

   /* --  Check options -- */
   while (--argc)
      if (**++argv=='-') {
         switch (*(*argv+1)) {
         case 'h':
            usage(0);
            break;
         case 'l':
            L = atoi(*++argv);
            --argc;
            break;
         case 'm':
            M = atoi(*++argv);
            --argc;
            break;
         case 't':
            T = atoi(*++argv);
            --argc;
            break;
         case 'a':
            Imin = atoi(*++argv);
            --argc;
            break;
         case 'b':
            Imax = atoi(*++argv);
            --argc;
            break;
         case 'e':
            E = atof(*++argv);
            --argc;
            break;
         case 'v':
            V = atof(*++argv);
            --argc;
            break;
         case 'w':
            W = atof(*++argv);
            --argc;
            break;
         default:
            fprintf(stderr, "%s: Illegal option \"%s\".\n", cmnd, *argv);
            usage(1);
         }
      }
      else
         fp = getfp(*argv, "rb");
     

   /* -- Count number of training vectors -- */
   if (T==-1) {
      ispipe = fseek(fp, 0L, 2);
      T = (int)(ftell(fp)/(double)L/(double)sizeof(float));
      rewind(fp);

      if (ispipe==-1) {   /* training data is from standard input via pipe */
         fprintf(stderr, "\n %s (Error) -t option must be specified for the standard input via pipe.\n", cmnd);
         usage(1);
      }
   }

   /* Memory allocation */
   /* Training data */
   dat = dgetmem(T*L);

   /* for VQ */
   N = 1; while (N<M) N*=2;
   cb  = dgetmem(N*L);
   icb = dgetmem(L);
   tindex = (int *) getmem(T, sizeof(int));
   cntcb  = (int *) getmem(M, sizeof(int));
     
   /* GMM */
   gmm.weight = dgetmem(M);
   gmm.gauss  = (Gauss *) getmem(M, sizeof(Gauss));

   for (m=0; m<M; m++) {
      gmm.gauss[m].mean = dgetmem(L);
      gmm.gauss[m].var  = dgetmem(L);     
   }

   /* temporary */
   tgmm.weight = dgetmem(M);
   tgmm.gauss = (Gauss *) getmem(M, sizeof(Gauss));

   for (m=0; m<M; m++) {
      tgmm.gauss[m].mean = dgetmem(L);
      tgmm.gauss[m].var  = dgetmem(L);     
   }

   logwgd = dgetmem(M);
   sum    = dgetmem(M);

   
   /*  Read training data */
   freadf(dat, sizeof(*dat), T*L, fp);

   /* Initialization of GMM parameters */
   /* LBG */
   vaverage(dat, L, T, icb);
   lbg(dat, L, T, icb, 1, cb, N, DELTA, END);
     
   for (t=0,pd=dat; t<T; t++,pd+=L) {
      tindex[t] = vq(pd,cb,L,M);
      cntcb[tindex[t]]++;
   }

   for (m=0; m<M; m++)
      if (cntcb[m]==0) {
         fprintf(stderr, "Error: No data for mixture No.%d\n", m);
         usage(1);
      }

   fprintf(stderr, "T = %d  L = %d  M = %d\n", T, L, M);

   /* flooring value for weights */
   W = 1.0 / (double)M * (double)W;

     
   /* weights */
   for (m=0,sum_w=0.0; m<M; m++) {
      gmm.weight[m] = (double)cntcb[m] / (double)T;
      if (gmm.weight[m] < W) gmm.weight[m] = W;
      sum_w += gmm.weight[m];
   }
   if (sum_w!=1.0)
     for (m=0; m<M; m++)
        gmm.weight[m] /= sum_w;


   /* mean */
   for (m=0,pd=cb ; m<M; m++,pd+=L)
      movem(pd, gmm.gauss[m].mean, sizeof(double), L);


   /* variance */
   for (t=0, pd=dat; t<T; t++, pd+=L)
      for (l=0; l<L; l++) {
         diff = gmm.gauss[tindex[t]].mean[l] - pd[l];
         gmm.gauss[tindex[t]].var[l] += sq(diff);
      }

   for (m=0; m<M; m++)
      for (l=0; l<L; l++) {
         gmm.gauss[m].var[l] /= (double)cntcb[m];
         if (gmm.gauss[m].var[l]<V) gmm.gauss[m].var[l] = V;
      }

   for (m=0; m<M; m++)
      gmm.gauss[m].gconst = cal_gconst(gmm.gauss[m].var, L);


   /* EM training of GMM parameters */
   for (i=0; (i<=Imax) && ((i<=Imin) || (fabs(change)>E)); i++) {
      fillz_gmm(&tgmm, M, L);
      fillz(sum, sizeof(double), M);

      for (t=0,ave_logp1=0.0,pd=dat; t<T; t++,pd+=L) {
         for (m=0,logb=LZERO; m<M; m++) {
            logwgd[m] = log_wgd(gmm, m, pd, L);
            logb = log_add(logb, logwgd[m]);
         }
         ave_logp1 += logb;

         for (m=0; m<M; m++) {
            tmp1 = exp(logwgd[m] - logb);
            sum[m] += tmp1;
            
            for (l=0; l<L; l++) {
               tmp2 = tmp1 * pd[l];
               tgmm.gauss[m].mean[l] += tmp2;
               tgmm.gauss[m].var[l]  += tmp2 * pd[l];
            }
         }
      }
      
      /* Output average log likelihood at each iteration */
      ave_logp1 /= (double)T;
      if (i==1 && m==1) ave_logp0 = ave_logp1;

      fprintf(stderr, "iter %3d : ", i);
      fprintf(stderr, "ave_logprob = %g", ave_logp1);         
      if (i) {
         change = ave_logp1 - ave_logp0;
         fprintf(stderr, "  change = %g", change);
      }
      fprintf(stderr, "\n");
      ave_logp0 = ave_logp1;

      /* Update perameters */
      /* weights */
      for (m=0; m<M; m++)
         gmm.weight[m] = sum[m] / (double)T;

      /* mean, variance */
      for (m=0; m<M; m++) {
         for (l=0; l<L; l++)
            gmm.gauss[m].mean[l] = tgmm.gauss[m].mean[l] / sum[m];

         for (l=0; l<L; l++) {
            gmm.gauss[m].var[l] = tgmm.gauss[m].var[l] / sum[m] - sq(gmm.gauss[m].mean[l]);
            if (gmm.gauss[m].var[l] < V) gmm.gauss[m].var[l] = V;
         }
      }
      
      for (m=0; m<M; m++)
         gmm.gauss[m].gconst = cal_gconst(gmm.gauss[m].var, L);
   }

   /*  Output GMM parameters */
   fwritef(gmm.weight, sizeof(double), M, stdout);
   for (m=0; m<M; m++) {
      fwritef(gmm.gauss[m].mean, sizeof(double), L, stdout);
      fwritef(gmm.gauss[m].var,  sizeof(double), L, stdout);
   }
   
   return(0);
}

double cal_gconst (double *var, const int D)
{
   int d;
   double gconst;

   gconst = D * log(M_2PI);
   for (d=0; d<D; d++)
      gconst += log(var[d]);
   
   return(gconst);
}

void fillz_gmm (GMM *gmm, const int M, const int L)
{
   int m, l, ll;

   for (m=0; m<M; m++) {
      gmm->weight[m] = 0.;
    
      for (l=0; l<L; l++)
         gmm->gauss[m].mean[l] = 0.;
    
      for (l=0; l<L; l++)
         gmm->gauss[m].var[l] = 0.;
   }
   
   return;
}

double log_wgd (GMM gmm, const int m, double *dat, const int L)
{
   int l;
   double sum, diff, lwgd;

   sum = gmm.gauss[m].gconst;

   for (l=0; l<L; l++) {
      diff = dat[l] - gmm.gauss[m].mean[l];
      sum += sq(diff) / gmm.gauss[m].var[l];
   }

   lwgd = log(gmm.weight[m]) - 0.5 * sum;    

   return(lwgd);
}

double log_add (double logx, double logy)
{
   double swap, diff, minLogExp, z;

   if  (logx < logy) {
      swap = logx;
      logx = logy;
      logy = swap;
   }

   diff = logy - logx;
   minLogExp = -log(-LZERO);

   if (diff < minLogExp)
      return((logx<LSMALL) ? LZERO : logx);
   else {
      z = exp(diff);
      return(logx + log(1.0+z));
   }
}
