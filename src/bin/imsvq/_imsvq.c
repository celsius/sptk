/****************************************************************

    $Id: _imsvq.c,v 1.1 2000/03/01 13:58:37 yossie Exp $

    Decoder of Multi Stage Vector Quantization

	imsvq(index, cb, l, cbsize, stage, x)

	int    *index  : index of codebook
	real   *cb     : codebook vector
	int    l       : order of vector
	int    *cbsize : size of codebook
	int    stage   : number of stage
	real   *x      : decoded vector

*****************************************************************/

#include	<stdio.h>
#include	<SPTK.h>

void imsvq(index, cb, l, cbsize, stage, x)
double *cb, *x;
int *index, *cbsize, l, stage;
{
    register int   i, j;
    double	   *p;
    static double  *xx = NULL;
    static int     size;
    
    if(xx == NULL){
	xx = dgetmem(l);
	size = l;
    }
    if(size > l){
	free(xx);
	xx = dgetmem(l);
	size = l;
    }

    fillz(x, sizeof(*x), l);

    for(i=0; i<stage; i++){
	ivq(index[i], cb, l, xx);

	for(j=0; j<l; j++)
	    x[j] += xx[j];

	cb += cbsize[i] * l;
    }
}
