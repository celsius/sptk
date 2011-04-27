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
/*                1996-2011  Nagoya Institute of Technology          */
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

    $Id: _gmm.c,v 1.9 2011/04/27 13:46:40 mataki Exp $

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

double cal_det(double **var, const int D)
{
   int i, j, l;
   double ldet = 0.0, **tri;

   tri = (double **) malloc(sizeof(double *) * D);
   for (l = 0; l < D; l++)
      tri[l] = dgetmem(D);

   for (i = 0; i < D; i++)
      for (j = 0; j < D; j++)
         tri[i][j] = 0.0;

   if (choleski(var, tri, D)) {
      for (l = 0; l < D; l++)
         ldet += log(tri[l][l]);
      return (2.0 * ldet);
   } else
      return 0;
}

double cal_gconst(double *var, const int D)
{
   int d;
   double gconst;

   gconst = D * log(M_2PI);
   for (d = 0; d < D; d++)
      gconst += log(var[d]);

   return (gconst);
}

double cal_gconstf(double **var, const int D)
{
   int d;
   double gconst, tmp;

   tmp = cal_det(var, D);
   if (tmp == 0) {
      return 0;
   }
   gconst = D * log(M_2PI);
   gconst += tmp;

   return (gconst);
}

void cal_tri_inv(double **S, double **S_inv, const int L)
{
   int i, j, k;

   for (i = 0; i < L; i++) {
      S_inv[i][i] = 1.0 / S[i][i];
   }
   for (i = 1; i < L; i++)
      for (j = i - 1; j >= 0; j--)
         for (k = j; k < i; k++) {
            S_inv[i][j] = S_inv[i][j] - S[i][k] * S_inv[k][j] / S[i][i];
         }
}

int choleski(double **cov, double **S, const int L)
{
   int i, j, k;
   double tmp;

   for (i = 0; i < L; i++) {
      for (j = 0; j < i; j++) {
         tmp = cov[i][j];
         for (k = 0; k < j; k++)
            tmp -= S[i][k] * S[j][k];
         S[i][j] = tmp / S[j][j];
      }
      tmp = cov[i][i];
      for (k = 0; k < i; k++)
         tmp -= S[i][k] * S[i][k];
      if (tmp <= 0) {
         return 0;
      }
      S[i][i] = sqrt(tmp);
   }
   return 1;
}

void cal_inv(double **cov, double **inv, const int L)
{
   int i, j, k;
   double **S, **S_inv;

   S = (double **) malloc(sizeof(double *) * L);
   S_inv = (double **) malloc(sizeof(double *) * L);

   for (i = 0; i < L; i++) {
      S[i] = dgetmem(L);
      S_inv[i] = dgetmem(L);
   }

   for (i = 0; i < L; i++) {
      for (j = 0; j < L; j++) {
         S[i][j] = 0.0;
         S_inv[i][j] = 0.0;
         inv[i][j] = 0.0;
      }
   }

   if (choleski(cov, S, L) == 0)
      return;
   cal_tri_inv(S, S_inv, L);

   for (i = 0; i < L; i++)
      for (j = 0; j < L; j++) {
         if (i > j)
            for (k = i; k < L; k++)
               inv[i][j] = inv[i][j] + S_inv[k][i] * S_inv[k][j];
         else
            for (k = j; k < L; k++)
               inv[i][j] = inv[i][j] + S_inv[k][i] * S_inv[k][j];
      }
}

void fillz_gmm(GMM * gmm, const int M, const int L)
{
   int m, l;

   for (m = 0; m < M; m++) {
      gmm->weight[m] = 0.;

      for (l = 0; l < L; l++)
         gmm->gauss[m].mean[l] = 0.;

      for (l = 0; l < L; l++)
         gmm->gauss[m].var[l] = 0.;
   }
}

void fillz_gmmf(GMM * gmm, const int M, const int L)
{
   int m, l, ll;

   for (m = 0; m < M; m++) {
      gmm->weight[m] = 0.;

      for (l = 0; l < L; l++)
         gmm->gauss[m].mean[l] = 0.;

      for (l = 0; l < L; l++)
         for (ll = 0; ll < L; ll++)
            gmm->gauss[m].cov[l][ll] = 0.;

      for (l = 0; l < L; l++)
         for (ll = 0; ll < L; ll++)
            gmm->gauss[m].inv[l][ll] = 0.;
   }
}

double log_wgd(GMM * gmm, const int m, double *dat, const int L)
{
   int l;
   double sum, diff, lwgd;

   sum = gmm->gauss[m].gconst;

   for (l = 0; l < L; l++) {
      diff = dat[l] - gmm->gauss[m].mean[l];
      sum += sq(diff) / gmm->gauss[m].var[l];
   }
   lwgd = log(gmm->weight[m]) - 0.5 * sum;
   return (lwgd);
}

double log_wgdf(GMM * gmm, const int m, double *dat, const int L)
{
   int l, ll;
   double sum, *diff, tmp, lwgd;

   diff = dgetmem(L);
   sum = gmm->gauss[m].gconst;

   for (l = 0; l < L; l++)
      diff[l] = dat[l] - gmm->gauss[m].mean[l];

   for (l = 0; l < L; l++) {
      for (ll = 0, tmp = 0.; ll < L; ll++)
         tmp += diff[ll] * gmm->gauss[m].inv[ll][l];
      sum += tmp * diff[l];
   }
   lwgd = log(gmm->weight[m]) - 0.5 * sum;
   return (lwgd);
}

double log_add(double logx, double logy)
{
   double swap, diff, minLogExp, z;

   if (logx < logy) {
      swap = logx;
      logx = logy;
      logy = swap;
   }

   diff = logy - logx;
   minLogExp = -log(-LZERO);

   if (diff < minLogExp)
      return ((logx < LSMALL) ? LZERO : logx);
   else {
      z = exp(diff);
      return (logx + log(1.0 + z));
   }
}

double log_outp(GMM * gmm, double *dat, const int M, const int L)
{
   int m;
   double logwgd, logb;

   for (m = 0, logb = LZERO; m < M; m++) {
      logwgd = log_wgd(gmm, m, dat, L);
      logb = log_add(logb, logwgd);
   }
   return (logb);
}
