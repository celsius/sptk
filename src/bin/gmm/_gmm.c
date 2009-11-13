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
/*                1996-2009  Nagoya Institute of Technology          */
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

/****************************************************************

    $Id$

    GMM output prob calculation functions

*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#if defined(WIN32)
#  include "SPTK.h"
#else
#  include <SPTK.h>
#endif

#include "gmm.h"

double cal_det (double **var, const int D)
{
  int i, j, k;
  double det=1.0, tmp, **tmp_var;

  tmp_var = (double **)malloc(sizeof(double *) * D);
  for (i=0; i<D; i++)
    tmp_var[i] = (double *)malloc(sizeof(double) * D);

  for (i=0; i<D; i++)
    for (j=0; j<=i; j++)
      tmp_var[i][j] = var[i][j];

  for (i=0; i<D; i++)
    for(j=0; j<i; j++){
      tmp_var[j][i]=tmp_var[i][j];
    }
  for (i=0; i<D; i++){
    for (j=0; j<D; j++){
      if (i < j) {
	tmp = tmp_var[j][i] / tmp_var[i][i];
	for (k=0; k<D; k++){
	  tmp_var[j][k] -= tmp_var[i][k] * tmp;
	}
      }
    }
  }
  for (i=0; i<D; i++)
    det *= tmp_var[i][i];
  return (det);
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

double cal_gconstf (double **var, const int D)
{
   int d,i;
   double gconst, tmp;
   static int num=0;

   tmp = cal_det(var, D);
   if (tmp < LZERO){
     return 0;
   }
   gconst = D * log(M_2PI);
   gconst += log(tmp);

   return(gconst);
}

void fillz_gmm (GMM *gmm, const int M, const int L)
{
   int m, l;

   for (m=0; m<M; m++) {
      gmm->weight[m] = 0.;
    
      for (l=0; l<L; l++)
         gmm->gauss[m].mean[l] = 0.;
    
      for (l=0; l<L; l++)
         gmm->gauss[m].var[l] = 0.;
   }
   
   return;
}

void fillz_gmmf (GMM *gmm, const int M, const int L)
{
  int m, l, ll;

  for (m=0; m<M; m++) {
    gmm->weight[m] = 0.;
    
    for (l=0; l<L; l++)
      gmm->gauss[m].mean[l] = 0.;
    
    for (l=0; l<L; l++)
      for (ll=0; ll<L; ll++)
         gmm->gauss[m].cov[l][ll] = 0.;

    for (l=0; l<L; l++)
      for (ll=0; ll<L; ll++)
         gmm->gauss[m].inv[l][ll] = 0.;
   }
   return;
}


double log_wgd (GMM *gmm, const int m, double *dat, const int L)
{
   int l;
   double sum, diff, lwgd;

   sum = gmm->gauss[m].gconst;

   for (l=0; l<L; l++) {
      diff = dat[l] - gmm->gauss[m].mean[l];
      sum += sq(diff) / gmm->gauss[m].var[l];
   }
   lwgd = log(gmm->weight[m]) - 0.5 * sum;    
   return(lwgd);
}

double log_wgdf (GMM *gmm, const int m, double *dat, const int L)
{
  int i, j, k, l, ll;
  double sum, *diff, tmp, lwgd, inv_a[L][L];
  double **a;

  a = (double **)malloc(sizeof(double *) * L);
  for (l=0; l<L; l++)
    a[l] = (double *)malloc(sizeof(double) * L);

  for (i=0; i<L; i++)
    for (j=0; j<=i; j++)
      a[i][j] = gmm->gauss[m].cov[i][j];

  for (i=0; i<L; i++)
    for(j=0; j<i; j++)
      a[j][i] = a[i][j];

  diff = dgetmem(L);
  sum = gmm->gauss[m].gconst;
  
  for (i=0; i<L; i++){
    for (j=0; j<L; j++){
      inv_a[i][j] = (i==j)?1.0:0.0;
    }
  }
  for (i=0; i<L; i++){
    tmp = 1/a[i][i];
    for (j=0; j<L; j++){
      a[i][j] *= tmp;
      inv_a[i][j] *= tmp;
    }
    for (j=0; j<L; j++){
      if (i != j){
	tmp = a[j][i];
	for (k=0; k<L; k++){
	  a[j][k] -= a[i][k] * tmp;
	  inv_a[j][k] -= inv_a[i][k] * tmp;
	}
      }
    }
  }
      
  for (l=0; l<L; l++)
    diff[l] = dat[l] - gmm->gauss[m].mean[l];

  for (l=0; l<L;l++){
    for (ll=0, tmp=0.; ll<L; ll++)
      tmp += diff[ll] * inv_a[ll][l];
    sum += tmp * diff[l];
  }
  
  lwgd = log(gmm->weight[m]) - 0.5 * sum;    

  free(a);  
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

double log_outp (GMM *gmm, double *dat, const int M, const int L)
{
   int m;
   double logwgd, logb;

   for (m=0,logb=LZERO; m<M; m++) {
      logwgd = log_wgd(gmm, m, dat, L);
      logb   = log_add(logb, logwgd);
   }
   
   return(logb);
}
