/****************************************************************

    $Id$

    Multi Stage Vector Quantization

	void msvq(x, cb, l, cbsize, stage, index)

	double *x      : input vector
	double *cb     : codebook vector
	int    l       : order of vector
	int    *cbsize : size of codebook
	int    stage   : number of stage
	int    *index  : index of codebook

*****************************************************************/

#include	<stdio.h>
#include	<SPTK.h>

void msvq(x, cb, l, cbsize, stage, index)
double *x, *cb;
int *cbsize, l, stage, *index;
{
    int  	  i, j, vq();
    double 	  *p, min = 1e23, dist;
    static double *xx = NULL;
    static int    size;
    
    if(xx == NULL){
	xx = dgetmem(l);
	size = l;
    }
    if(size > l){
	free(xx);
	xx = dgetmem(l);
	size = l;
    }


    movem(x, xx, sizeof(*x), l);

    for(i=0; i<stage; i++){
	index[i] = vq(xx, cb, l, cbsize[i]);
	
	p = cb + index[i] * l;
	for(j=0; j<l; j++) 
	    xx[j] -= p[j];

	cb += cbsize[i] * l;
    }
}
