/****************************************************************
 
    $Id: _lpc2lsp.c,v 1.1 2000/03/01 13:58:39 yossie Exp $
 
    Transformation LPC to LSP
 
        int    lpc2lsp(lpc, lsp, order, numsp, maxp, maxq, eps)
 
        double *lpc   : LPC
        double *lsp   : LSP
        int    order  : order of LPC
        int    numsp  : split number of unit circle
        int    maxitr : maximum number of interpolation
        double eps    : end condition for interpolation

	return value  
	           0  : completed normally
		   -1 : completed irregularly

*****************************************************************/

#include <stdio.h>
#include <SPTK.h>

int lpc2lsp(double *lpc, double *lsp, int order, int numsp, int maxitr, double eps)
{
    register int    i, j;
    register double *p1, *p2;
    int             mh1, mh2, mh, mm, itr, flag_odd;
    double          delta, x0, x1, g0, g1, x, y;
    double          acos(), fabs(), chebpoly();
    static double   *c1 = NULL, *c2;
    static int      size_order;

    delta = 1.0 / (double) numsp;

    flag_odd = 0;
    if(order % 2 == 0)
      mh1 = mh2 = order / 2;
    else{
      mh1 = (order+1) / 2;
      mh2 = (order-1) / 2;
      flag_odd = 1;
    }

    if(c1 == NULL){
        c1 = dgetmem(2*(mh1+1)); c2 = c1 + (mh1+1);
	size_order = order;
    }
    if(order > size_order){
        free(c1);
        c1 = dgetmem(2*(mh1+1)); c2 = c1 + (mh1+1);
	size_order = order;
    }

    /* calculate symmetric and antisymmetrica polynomials */
    p1 = lpc + 1;  p2 = lpc + order;
    c1[mh1] = c2[mh2] = 1.0;
    if(flag_odd){
        c2[mh2+1] = 0.0;
        for(i=mh2-1; i>=0; i--){
	    c1[i+1] = *p1   + *p2;
            c2[i]   = *p1++ - *p2-- + c2[i+2];
	}
	c1[0] = *p1 + *p2;
    }
    else{
        for(i=mh1-1; i>=0; i--){
            c1[i] = *p1   + *p2   - c1[i+1];
	    c2[i] = *p1++ - *p2-- + c2[i+1];
	}
    }
    c1[0] *= 0.5;  c2[0] *= 0.5;

    /* root search */
    p1 = c1; mh = mh1;
    g0 = chebpoly(1.0, p1, mh);

    mm = 0;
    for(x=1.0-delta; x>-delta-1.0; x-=delta){
        g1 = chebpoly(x, p1, mh);

        if(g0*g1 <= 0.0){
           x0 = x + delta;
           x1 = x;

           itr = 0;
           do {
               x = (x0 + x1) / 2.0;
               y = chebpoly(x, p1, mh);

               if(y*g0 < 0.0) {x1 = x; g1 = y;}
               else           {x0 = x; g0 = y;}

               itr++;
           } while ((fabs(y) > eps) && (itr < maxitr));

           x = (g1*x0 - g0*x1) / (g1 - g0);
           lsp[mm] = acos(x) / PI2;

           mm++;
           if(mm == order) return(0);

           if(p1 == c1){ p1 = c2; mh = mh2; }
           else        { p1 = c1; mh = mh1; }

           g1 = chebpoly(x, p1, mh);
        }
        g0 = g1;
    }    
    return(-1);
}

double chebpoly(double x, double *c, int mh)
{
    register int    i;
    double          b[3];

    b[1] = b[2] = 0.0;
    for(i=mh; i>0; i--){
        b[0] = 2.0 * x * b[1] - b[2] + c[i];
        b[2] = b[1];
        b[1] = b[0];
    }
    b[0] = x * b[1] - b[2] + c[0];

    return(b[0]);
}

