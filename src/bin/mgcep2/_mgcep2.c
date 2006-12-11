/*
  ----------------------------------------------------------------
	Speech Signal Processing Toolkit (SPTK): version 3.0
			 SPTK Working Group

		   Department of Computer Science
		   Nagoya Institute of Technology
				and
    Interdisciplinary Graduate School of Science and Engineering
		   Tokyo Institute of Technology
		      Copyright (c) 1984-2000
			All Rights Reserved.

  Permission is hereby granted, free of charge, to use and
  distribute this software and its documentation without
  restriction, including without limitation the rights to use,
  copy, modify, merge, publish, distribute, sublicense, and/or
  sell copies of this work, and to permit persons to whom this
  work is furnished to do so, subject to the following conditions:

    1. The code must retain the above copyright notice, this list
       of conditions and the following disclaimer.

    2. Any modifications must be clearly marked as such.

  NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSITITUTE OF TECHNOLOGY,
  SPTK WORKING GROUP, AND THE CONTRIBUTORS TO THIS WORK DISCLAIM
  ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT
  SHALL NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSITITUTE OF
  TECHNOLOGY, SPTK WORKING GROUP, NOR THE CONTRIBUTORS BE LIABLE
  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
  DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
  WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
  ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
  PERFORMANCE OF THIS SOFTWARE.
 ----------------------------------------------------------------
*/

/****************************************************************

    Fast Mel-Generalized Cepstral Analysis for gamma=-1/2
                                           (convolution version)

	int mgcep2(xw, flng, c, order, alpha, fftlng, itr1, itr2, thr);

	double   *xw    : input sequence
	int      flng   : frame length
	double   *c     : MGC coefficient c(m)
	int      order  : order of MGC coefficients
	double   alpha  : alpha
        int      fftlng : FFT length
	int      itr1   : minimum number of iteration
	int      itr2   : maximum number of iteration
	double   thr    : threshold

	return value : 	0 -> completed by end condition
			-1-> completed by maximum iteration

*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <SPTK.h>

#define min(x,y) (x < y ? x : y)
#define abs(x)   (x < 0 ? -x : x)

double exp(), pow(), log(), log10(), sqrt(), fabs();
double calckg();

int mgcep2(double *xw, int flng, double *c, int order, double alpha, int minitr, int maxitr, double thr)
{
    register int     i, j, k; 
    register double  *pt1, *pt2, *pt3, *pt4, *pt5, *pt6;
    int              order1, order2, flag = 0;
    double           ac1, ac2, gain, oldgain;
    static double    *x = NULL, *x0, *x1, *pp, *p, *q, *r, *c0, *c1, *aa, *s;
    static int       size_c, size_x;

    order1 = order + 1;
    order2 = order + order;

    if(x == NULL){
	x  = dgetmem(order2+order2+1);
        x0 = dgetmem(flng+flng); x1 = x0 + flng;

        pp = dgetmem(5*order2+5);
        p  = pp + order+order2+1; q  = p + order2+1;
        r  = q + order2+1;        aa = r + order+1;

        c0 = dgetmem(order2+order2+2);
        c1 = c0 + order2+1;

        s = dgetmem(order1*order1);
        /* shift origin */
        x  += order2; pp += order; p  += order;

	size_x = flng; size_c = order;
    }
    if(flng > size_x){
        free(x0);
        x0 = dgetmem(flng+flng); x1 = x0 + flng;
	size_x = flng;
    }
    if(order > size_c){
        free(x); free(pp); free(c0); free(s);
	x  = dgetmem(order2+order2+1);

        pp = dgetmem(5*order2+5);
        p  = pp + order+order2+1; q  = p + order2+1;
        r  = q + order2+1;        aa = r + order+1;

        c0 = dgetmem(order2+order2+2);
        c1 = c0 + order2+1;

        s = dgetmem(order1*order1);
        /* shift origin */
        x  += order2; pp += order; p  += order;

	size_c = order;
    }

    /* calculate (-alpha)^m */
    pt1 = pt2 = aa;
    *pt1++ = 1.0;
    for(i=1; i<=order2; i++)
        *pt1++ = -alpha * *pt2++;

    /* calculate x^(n) */
    pt1 = x; pt2 = pt3 = pt5 = xw;
    *pt1 = 0.0;
    for(j=0; j<flng; j++,pt2++)
        *pt1 += *pt2 * *pt2;

    pt1++;
    for(i=1; i<=order2; i++){
        pt2 = xw; pt4 = pt6 = x1;
        *pt4 = -alpha * *pt3++;
        *pt1 = *pt2++ * *pt4++;
        for(j=1; j<flng; j++){
            *pt4 = *pt5++ + alpha * (*pt6++ - *pt3++);
            *pt1 += *pt2++ * *pt4++;
	}
        x[-i] = *pt1++;
        pt4 = x1; x1 = x0; x0 = pt4;
        pt3 = pt5 = x0;
    }

    if(toeplitz(x, c1, aa, order2+1, -1.0) != 0){
        fprintf(stderr,"equation can't be solved!\n");
        exit(1);
    }

    /* c' -> c */
    gc2c(c1, c0, order2, -1.0);

    /* ac1 = 1.0 - a^T c */
    ac1 = calckg(c0, aa, order2, -1.0);

    /* gain normalization */
    ac1 = 1.0 / sqrt(ac1);
    for(i=0; i<=order2; i++)
        c1[i] *= ac1;

    /* c' -> c */
    gc2c(c1, c0, order2, -1.0);   /* c0 : MEL-LPC coefficients */

    /* generalized cepstral transformation */
    /* gamma=-1.0 -> gamma=-1/2 */
    gnorm(c0, c1, order2, -1.0);
    gc2gc(c1, order2, -1.0, c0, order, -0.5);
    ignorm(c0, c0, order, -0.5);

    gain = 1e23;

    /* Newton-Raphson method */ 
    for(k=0; k<maxitr; k++){

        c2gc(c0, c1, order, -0.5);

        ac1 = calckg(c0, aa, order, -0.5);

        oldgain = gain;

        ac1 = 1.0 / ac1;
        ac2 = ac1 * ac1;
        gain = log(ac2);

        if(k >= minitr)
            if(fabs((gain-oldgain)/gain) < thr){
	        flag = 1;
		break;
            }

        /* convolution for p(k) */
        pt1 = pp - order;
        for(i=-order; i<=order2; i++){
            *pt1 = 0.0; pt2 = c1; pt4 = x + i;
            for(j=0; j<=order; j++)
                *pt1 += *pt2++ * *pt4--;
            pt1++;
        }

        pt1 = p;
        for(i=0; i<=order; i++){
            *pt1 = 0.0; pt2 = c1; pt3 = pp + i;
            for(j=0; j<=order; j++)
                *pt1 += *pt2++ * *pt3++;
            p[-i] = *pt1;
            pt1++;
        }

        /* convolution for r(k) */
        pt1 = r;
        for(i=0; i<=order; i++){
            *pt1 = 0.0; pt2 = c1; pt3 = p + i;
            for(j=0; j<=order; j++)
                *pt1 += *pt2++ * *pt3--;
            pt1++;
        }

        /* convolution for q(k) */
        pt1 = q;
        for(i=0; i<=order2; i++){
            *pt1 = 0.0; pt2 = c1; pt3 = pp + i;
            for(j=0; j<=order; j++)
                *pt1 += *pt2++ * *pt3--;
            pt1++;
        }

        pt1 = q; pt2 = r; pt3 = aa;
        for(i=0;i<=order; i++){
            *pt1 = 0.5 * (*pt1 + *pt3 * ac2);
            *pt2 = *pt2 - *pt3 * ac1;
            pt1 ++; pt2++; pt3++;
	}
        for(; i<=order2; i++){
            *pt1 = 0.5 * (*pt1 + *pt3 * ac2);
            pt1++; pt3++;
	}

        /* generate matrix for Cholesky decomposition */
        pt1 = s;
        for(i=0; i<=order; i++){
            pt2 = p + i; pt3 = q + i; pt4 = s + i;
            for(j=0; j<=i; j++){
                *pt1++ = *pt4 = *pt2-- + *pt3++;
                pt4 += order1;
            }
            pt1 += order1 - i;
        }

	/* update the coefficients */
        if(cholesky(s, c1, r, order1, -1.0)!= 0){
            fprintf(stderr,"iteration=%d: Equation can't be solved!\n",k);
            exit(1);
        }

        pt1 = c0; pt2 = c1;
        for(i=0; i<=order; i++)
            *pt1++ += *pt2++;
    }

    movem(c0,c,sizeof(*c0),order1);

    if(flag) return(0);
    else     return(-1);
}

void c2gc(double *c, double *gc, int order, double gamma)
{
    *gc++ = 1.0 + gamma * *c++;
    for(; order>0; order--)
        *gc++ = gamma * *c++;
}

void gc2c(double *gc, double *c, int order, double gamma)
{
    double       g1;

    g1 = 1.0 / gamma;

    *c++ = (*gc++ - 1.0) * g1;
    for(; order>0; order--)
        *c++ = g1 * *gc++;
}

double calckg(double *c, double *aa, int order, double gamma)
{
    double        ac;

    ac = *c++;
    aa++;
    for(; order>0; order--)
        ac += *c++ * *aa++;

    return(1 + gamma*ac);
}



