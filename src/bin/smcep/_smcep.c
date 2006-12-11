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

    $Id: smcep.c

    Mel-Cepstral Analysis (2nd order all-pass filter)

	int smcep(xw, flng, mc, m, fftsz, a, t, itr1, itr2, dd, e);

	double   *xw  : input sequence
	int      flng : frame length
	double   *mc  : mel cepstrum
	int      m    : order of mel cepstrum
	double   a    : alpha
	double   t    : theta
	int      itr1 : minimum number of iteration
	int      itr2 : maximum number of iteration
	double   dd   : end condition
	double   e    : initial value for log-periodgram

	return value : 	0 -> completed by end condition
			-1-> completed by maximum iteration

*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <SPTK.h>
#include <math.h>

#define M_PI    3.1415926535897932384626433832795
#define M_2PI   6.2831853071795864769252867665590

int smcep(double *xw, int flng,double *mc, int m, int fftsz, double a, double t, int itr1,int itr2,double dd,double e)
{
    register int   i, j;
    int		   flag = 0, f2, m2;
    double	   u, s, fabs(), exp(), log();
    static double  *x = NULL, *y, *c, *d, *al, *b;
    static int     size_x, size_d;
    void	   frqt_a(), freqt2(), ifreqt2(), frqtr2();
  
    if(x == NULL){
	x = dgetmem(3*flng);
	y = x + flng; c = y + flng;
	size_x = flng;
	
	d = dgetmem(3*m+3);
	al = d + (m+1); b = al + (m+1);
	size_d = m;
    }
    if(flng > size_x){
	free(x);
	x = dgetmem(3*flng);
	y = x + flng; c = y + flng;
	size_x = flng;
    }
    if(m > size_d){
	free(d);
	d = dgetmem(3*m+3);
	al = d  + (m+1); b  = al + (m+1);
	size_d = m;
    }

    f2 = flng / 2.;
    m2 = m + m;

    movem(xw, x, sizeof(*x), flng);

    /*  power spectrum  */
    fftr(x, y, flng);
    for(i=0; i<flng; i++){
	x[i] = x[i]*x[i] + y[i]*y[i];
	c[i] = log(x[i]+e);
    }
	
/*  1, (-a), (-a)^2, ..., (-a)^M  */

    al[0] = 1.0;
    for( i=1; i <=m; i++ ) al[i] = 0.0;
    
    frqt_a(al, m, fftsz, a, t);


    /*  initial value of cepstrum  */
    ifftr(c, y, flng);				/*  c : IFFT[x]  */
    
    c[0] /= 2.0;
    c[flng/2] /= 2.0;
    freqt2(c, f2, mc, m, fftsz, a, t);		/*  mc : mel cep.  */

    s = c[0];

    /*  Newton Raphson method  */
    for (j=1; j<=itr2; j++){
	fillz(c, sizeof(*c), flng);
	ifreqt2(mc, m, c, f2, fftsz, a, t);	/*  mc : mel cep.  */

	fftr(c, y, flng);			/*  c, y : FFT[mc]  */
	for(i=0; i<flng; i++)
	    c[i] = x[i] / exp(c[i] + c[i]);
	ifftr(c, y, flng);
	frqtr2(c, f2, c, m2, fftsz, a, t);	/*  c : r(k)  */

	u = c[0];
	if(j >= itr1){
	    if (fabs((u - s)/u) < dd){
		 flag = 1;
		 break;
	    }
	    s = u;
	}
	
	for(i=0; i<=m; i++)
	    b[i] = c[i] - al[i];
	for(i=0; i<=m2; i++)  y[i] = c[i];
	for(i=0; i<=m2; i+=2) y[i] -= c[0];
	for(i=2; i<=m;  i+=2) c[i] += c[0];
	c[0] += c[0];

	if (theq(c, y, d, b, m+1, -1.0)){
	    fprintf(stderr,"smcep : Error in theq() at %dth iteration !\n", j);
	    exit(1);
	}

	for (i=0; i<=m; i++) mc[i] += d[i];
    }

    if(flag) return(0);
    else     return(-1);


}


/***************************************************************

  No.1  frqt_a    static : *l, size1

    Frequency Transformation of "al" (second term of dE/dc)

	void	frqt_a(al, m, fftsz, a, t)

	double	*al   : sequence which will be warped 
	int	m     : order of warped sequence
	int	fftsz : ifft size
	double	a     : all-pass constant
        double	t     : emphasized frequency (t * pi)

***************************************************************/

void frqt_a(double *al,int m,int fftsz,double a,double t)
{
    register int 	i, j;
    double		w, b, *ww, *f, 
                        *re, *im, *pf, *pl, *next, warp();
    int                 size_l, size_f, fftsz2;
    static double	*l = NULL;
    static int		size1, flag_l = 1;

    b = M_2PI / (double)fftsz;

    size_l =  m + 1 ;

    if( l == NULL ){
        flag_l = 0;
        size1 = size_l;
        l = dgetmem( size1 );
    }
    else if( size_l != size1 ){
        free(l);
        flag_l = 0;       
        size1 = size_l;
        l = dgetmem( size1 );
    }

    /*-------  if "l" is not defined  ----------*/    

    if( flag_l == 0 ){

        ww = dgetmem( fftsz );

        for( j = 0, w = 0.; j < fftsz; j++, w+=b )
            ww[j] = warp( w, a, t );

        fftsz2 = fftsz + fftsz;         /* size of (re + im) */
        size_f = ( m + 1 ) * fftsz2;    /* size of array "f" */
        f = dgetmem( size_f );
        
        for( i = 0, re = f, im = f + fftsz; i <= m; i++ ){

            for( j = 0; j < fftsz; j++ )
	      *(re++) =  cos( ww[j] * i );
            for( j = 0; j < fftsz; j++ )
	      *(im++) = -sin( ww[j] * i );

	    re -= fftsz;
            im -= fftsz;

            ifft( re, im, fftsz );

            re += fftsz2;
            im += fftsz2;
        }

        free( ww );


        /*-------  copy "f" to "l" ----------*/            

        for( i = 0, next = f, pf = f, pl = l; i <= m; i++ ){
	    *(pl++) = *pf;
            next += fftsz2;
	    pf = next;
        }

        free( f );
        flag_l = 1;
    }

    movem(l, al, sizeof(*al), m+1);
}


/***************************************************************

  No.2  freqt2    static : *g, size2

    Frequency Transformation

	void	freqt2(c1, m1, c2, m2, fftsz, a, t)

	double	*c1   : minimum phase sequence
	int	m1    : order of minimum phase sequence
	double	*c2   : warped sequence
	int	m2    : order of warped sequence
	int	fftsz : ifft size
	double	a     : all-pass constant
        double	t     : emphasized frequency (t * pi)

***************************************************************/

void freqt2(double *c1,int m1,double *c2,int m2,int fftsz,double a,double t)
{
    register int 	i, j;
    double		w, b, *ww, *dw, *f,
                        *re, *im, *pf, *pg, *next, warp(), derivw();
    int                 size_g, size_f, fftsz2, fftszh;
    static double	*g = NULL;
    static int		size2, flag_g = 1;

    b = M_2PI / (double)fftsz;

    size_g = ( m2 + 1 ) * ( m1 + 1 );

    if( g == NULL ){
        flag_g = 0;
        size2 = size_g;
        g = dgetmem( size2 );
    }
    else if( size_g != size2 ){
        free(g);
        flag_g = 0;        
        size2 = size_g;
        g = dgetmem( size2 );
    }

    /*-------  if "g" is not defined  ----------*/    

    if( flag_g == 0 ){

        ww = dgetmem( fftsz );
        dw = dgetmem( fftsz );        

        fftszh = (int)( fftsz /2. );
        
        for( j = 0, w = 0.; j < fftsz; j++, w+=b )
	    ww[j] = warp( w, a, t );

        for( j = 0, w = 0.; j < fftsz; j++, w+=b )
	    dw[j] = derivw( w, a, t );

	
        fftsz2 = fftsz + fftsz;       /* size of (re + im) */
        size_f = ( m2 + 1 ) * fftsz2; /* size of array "f" */
        f = dgetmem( size_f );
        
        for( i = 0, re = f, im = f + fftsz; i <= m2; i++ ){

            for( j = 0; j < fftsz; j++ )
	        *(re++) =  cos( ww[j] * i ) * dw[j];
            for( j = 0; j < fftsz; j++ )
	        *(im++) = -sin( ww[j] * i ) * dw[j];

	    re -= fftsz;
            im -= fftsz;

            ifft( re, im, fftsz );

            for( j = 1; j <= m1; j++ )
	      re[j] += re[fftsz-j];

            re += fftsz2;
            im += fftsz2;
        }

        free( ww );
        free( dw );


        /*-------  copy "f" to "g" ----------*/            

        for( i = 0, next = f, pf = f, pg = g; i <= m2; i++ ){
            for( j = 0; j <= m1; j++ )
                *(pg++) = *(pf++);
            next += fftsz2;
            pf = next;
        }
        free( f );
        flag_g = 1;

        for( j = 1; j <= m1; j++ )
            g[j] *= 0.5;

        for( i = 1; i <= m2; i++ )
            g[i*(m1+1)] *= 2.0;        
    }

    for( i = 0, pg = g; i <= m2; i++ )
        for( j = 0, c2[i] = 0.; j <= m1; j++ )
            c2[i] += *(pg++) * c1[j];
}



/***************************************************************

  No.3  ifreqt2    static : *h, size3

   Inverse Frequency Transformation

	void	ifreqt2(c1, m1, c2, m2, fftsz, a, t)

	double	*c1   : minimum phase sequence
	int	m1    : order of minimum phase sequence
	double	*c2   : warped sequence
	int	m2    : order of warped sequence
        int	fftsz : ifft size
	double	a     : all-pass constant
        double	t     : emphasized frequency t * pi(rad)

***************************************************************/

void ifreqt2(double *c1,int m1,double *c2,int m2,int fftsz,double a,double t)
{
    register int 	i, j;
    double		w, b, *ww, *f,
                        *re, *im, *pl, *pr, *plnxt, *prnxt,
                        *pf, *ph, *next, warp(), derivw();
    int                 size_h, size_f, fftsz2, m12, m11;
    static double	*h = NULL;
    static int		size3, flag_h = 1;

    b = M_2PI / (double)fftsz;

    size_h = ( m2 + 1 ) * ( m1 + 1 );

    if( h == NULL ){
        flag_h = 0;
        size3 = size_h;
        h = dgetmem( size3 );
    }
    else if( size_h != size3 ){
        free(h);
        flag_h = 0;        
        size3 = size_h;
        h = dgetmem( size3 );
    }

    /*-------  if "h" is not defined  ----------*/    

    if( flag_h == 0 ){

        ww = dgetmem( fftsz );

        for( j = 0, w = 0.; j < fftsz; j++, w+=b )
            ww[j] = warp( w, a, t );

        fftsz2 = fftsz + fftsz;       /* size of (re + im) */

	m12 = m1 + m1 + 1;
        size_f = m12 * fftsz2;      /* size of array "f" */
        f = dgetmem( size_f );
        

        for( i = -m1, re = f, im = f + fftsz; i <= m1; i++ ){

	  for( j = 0; j < fftsz; j++ )
	    *(re++) =  cos( ww[j] * i );
	  
	  for( j = 0; j < fftsz; j++ )
	    *(im++) = -sin( ww[j] * i );

	  re -= fftsz;
	  im -= fftsz;

	  ifft( re, im, fftsz );

	  re += fftsz2;
	  im += fftsz2;
        }

        free( ww );

        /*------- b'(n,m)=b(n,m)+b(n,-m) ----------*/            

	pl = f;
	pr = f + ( m12 - 1 ) * fftsz2;

	for( i = 0, plnxt = pl, prnxt = pr; i < m1; i++ ){
	  plnxt += fftsz2;
	  prnxt -= fftsz2;

	  for( j = 0; j <= m2; j++ )
	    *(pr++) += *(pl++);

	  pl = plnxt;
	  pr = prnxt;
	}

        /*-------  copy "f" to "h" ----------*/            

	m11 = m1 + 1;
	pf = f + m1 * fftsz2;

	for( j = 0, next = pf;  j <= m1; j++ ){

	  next += fftsz2;

	  for( i = 0; i <= m2; i++ )
	    h[m11*i+j] = *(pf++);

	  pf = next;
	}
        free( f );
        flag_h = 1;

        for( j = 1; j <= m1; j++ )
            h[j] *= 0.5;

        for( i = 1; i <= m2; i++ )
            h[i*m11] *= 2.0;
    }

    for( i = 0, ph = h; i <= m2; i++ )
      for( j = 0, c2[i] = 0.; j <= m1; j++ )
	c2[i] += *(ph++) * c1[j];
}



/***************************************************************

  No.4  frqtr2    static : *k, size4

    Frequency Transformation for Calculating Coefficients

	void	frqtr2(c1, m1, c2, m2, fftsz, a, t)

	double	*c1   : minimum phase sequence
	int	m1    : order of minimum phase sequence
	double	*c2   : warped sequence
	int	m2    : order of warped sequence
	int	fftsz  : frame length (fft size)
	double	a     : all-pass constant
	double	t     : emphasized frequency

***************************************************************/

void frqtr2(double *c1,int m1,double *c2,int m2,int fftsz,double a,double t)
{
    register int 	i, j;
    double		w, b, *ww, *f, *tc2,
                        *re, *im, *pf, *pk, *next, warp();
    int                 size_k, size_f, fftsz2;
    static double	*k = NULL;
    static int		size4, flag_k = 1;

    b = M_2PI / (double)fftsz;

    size_k = ( m2 + 1 ) * ( m1 + 1 );

    if( k == NULL ){
        flag_k = 0;
        size4 = size_k;
        k = dgetmem( size4 );
    }
    else if( size_k != size4 ){
        free(k);
        flag_k = 0;        
        size4 = size_k;
        k = dgetmem( size4 );
    }

    /*-------  if "k" is not defined  ----------*/    

    if( flag_k == 0 ){

        ww = dgetmem( fftsz );

        for( j = 0, w = 0.; j < fftsz; j++, w+=b )
            ww[j] = warp( w, a, t );

        fftsz2 = fftsz + fftsz;         /* size of (re + im) */
        size_f = ( m2 + 1 ) * fftsz2; /* size of array "f" */
        f = dgetmem( size_f );
        
        for( i = 0, re = f, im = f + fftsz; i <= m2; i++ ){

            for( j = 0; j < fftsz; j++ )
	      *(re++) =  cos( ww[j] * i );
            for( j = 0; j < fftsz; j++ )
	      *(im++) = -sin( ww[j] * i );

	    re -= fftsz;
            im -= fftsz;

            ifft( re, im, fftsz );

            for( j = 1; j <= m1; j++ )
	      re[j] += re[fftsz-j];
	    
            re += fftsz2;
            im += fftsz2;
        }

        free( ww );


        /*-------  copy "f" to "k" ----------*/            

        for( i = 0, next = f, pf = f, pk = k; i <= m2; i++ ){
            for( j = 0; j <= m1; j++ )
                *(pk++) = *(pf++);
            next += fftsz2;
            pf = next;
        }
        free( f );
        flag_k = 1;
    }

    tc2 = dgetmem( m2 + 1 );   /*  tmp of c2  */
    
    for( i = 0, pk = k; i <= m2; i++ )
        for( j = 0, tc2[i] = 0.; j <= m1; j++ )
            tc2[i] += *(pk++) * c1[j];

    movem(tc2, c2, sizeof(*c2), m2+1);

    free(tc2);
}


/***************************************************************

  Warping Function and Its Derivative
  
  double   warp(w, a, t)  &  derivw(w, a, t)

  double  w     : frequency
  double  a     : all-pass constant
  double  t     : emphasized frequency
  
***************************************************************/

double warp(double w,double a,double t)
{
    double              ww, x, y;

    x = w-t;
    y = w+t;
    
    ww = w + atan2( (a * sin(x)), (1. - a * cos(x)) )
           + atan2( (a * sin(y)), (1. - a * cos(y)) );
    return(ww);
}


/*============================================================*/

double derivw(double w,double  a,double t)
{
    double dw, x, y, a2, aa;
 
    x = w-t;
    y = w+t;

    a2 = a+a;
    aa = a*a;

    dw = 1. + ( a * cos(x) - aa )/( 1. - a2 * cos(x) + aa )
            + ( a * cos(y) - aa )/( 1. - a2 * cos(y) + aa );
    return(dw);
}



