/********************************************************
   $Id: _fft.c,v 1.1 2000/03/01 13:58:29 yossie Exp $					
	NAME:					
		fft - fast fourier transform    
	SYNOPSIS:					
		int	fft(x, y, m);			
							
		double	x[];	real part		
		double	y[];	imaginary part		
		int	m;	data size		
	
		return : success = 0
			 fault	 = -1
	Naohiro Isshiki 	Dec.1995    modified	
********************************************************/

#include <stdio.h>
#include <math.h>
#include <SPTK.h>


double	*_sintbl = 0;
int 	maxfftsize = 0;
int fft(x, y, m)
double	*x, *y;
int	m;
{
	register int	j, lmx, li;
	register double	*xp, *yp;
	register double	*sinp,*cosp;
	int	lf, lix,tblsize;
	int	mv2, mm1;
	double	t1, t2;
	double  arg;

	/**************
	* RADIX-2 FFT *
	**************/

	if(checkm(m))
		return(-1);

	/***********************
	* SIN table generation *
	***********************/

	if(_sintbl == 0 || maxfftsize < m ) {
		tblsize=m-m/4+1;
		arg=PI/m*2;
		if (_sintbl != 0)
			free(_sintbl);
		_sintbl = sinp = dgetmem(tblsize);
		*sinp++ = 0;
		for(j = 1 ; j < tblsize ; j++)
			*sinp++ = sin( arg * (double)j);
		_sintbl[m/2] = 0;
		maxfftsize = m;
	}

	lf = maxfftsize / m;
	lmx = m;

	for(;;) {
		lix = lmx;  
		lmx /= 2;
		if(lmx <= 1) break;
		sinp = _sintbl;
		cosp = _sintbl + maxfftsize/4;
		for(j = 0 ; j < lmx ; j++) {
			xp = &x[j];
			yp = &y[j];
			for(li = lix ; li <= m ; li += lix) {
				t1 = *(xp) - *(xp + lmx);
				t2 = *(yp) - *(yp + lmx);
				*(xp) += *(xp + lmx);
				*(yp) += *(yp + lmx);
				*(xp + lmx) = *cosp * t1 + *sinp * t2;
				*(yp + lmx) = *cosp * t2 - *sinp * t1;
				xp += lix;
				yp += lix;
			}
			sinp += lf;
			cosp += lf;
		}
		lf += lf;
	}

	xp = x;
	yp = y;
	for(li = m / 2; li--; xp += 2, yp += 2) {
		t1 = *(xp) - *(xp + 1);
		t2 = *(yp) - *(yp + 1);
		*(xp) += *(xp + 1);
		*(yp) += *(yp + 1);
		*(xp + 1) = t1;
		*(yp + 1) = t2;
	}
	/***************
	* bit reversal *
	***************/
	j = 0;
	xp = x;
	yp = y;
	mv2 = m / 2;
	mm1 = m - 1; 
	for(lmx = 0 ; lmx < mm1 ; lmx++) {
		if((li = lmx - j) < 0) {
			t1 = *(xp);
			t2 = *(yp);
			*(xp) = *(xp + li);
			*(yp) = *(yp + li);
			*(xp + li) = t1;
			*(yp + li) = t2;
		}
		li = mv2;
		while(li <= j) {
			j -= li;
			li /= 2;
		}
		j += li;
		xp = x + j;
		yp = y + j;
	}
	return(0);
}

int checkm(m)
int	m;
{
	register int	k;

	for(k = 4; k <= m; k <<= 1 ) {
		if(k == m)
			return(0);
	}
	fprintf(stderr, "fft : m must be a integer of power of 2\n");
	return(-1);
}
