
#include 	<stdio.h>
#include 	<SPTK.h>

#define	MAXVALUE	1e23
#define abs(x)          ( (x < 0) ? (-(x)) : (x) )

int vq();
double edist();

void lbg(x, l, tnum, icb, icbsize, cb, ecbsize, delta, end)
double *x, *icb, *cb, delta, end;
int l, tnum, icbsize, ecbsize;
{
    int		     i, j, k, maxindex;
    static int	     *cntcb, *tindex, size, sizex, sizecb;
    double           d0, d1, dl, err;
    static double    *rnd = NULL, *cb1;
    register double  *p, *q, *r;
    
    if(rnd == NULL){
	rnd = dgetmem(l);
	cb1 = dgetmem(ecbsize*l);
	tindex = (int *)dgetmem(tnum);
	cntcb  = (int *)dgetmem(ecbsize);
	size = l;
	sizex = tnum;
	sizecb = ecbsize;
    }
    if(l > size){
	free(rnd); free(cb1);
	rnd = dgetmem(l);
	cb1 = dgetmem(ecbsize*l);
	size = l;
    }
    if(tnum > sizex) {
	free(tindex);
	tindex = (int *)dgetmem(tnum);
	sizex = tnum;
    }
    if(sizecb > ecbsize){
	free(cb1); free(cntcb);
	cb1 = dgetmem(ecbsize*l);
	cntcb = (int *)dgetmem(ecbsize);
    }
    
    movem(icb, cb, sizeof(*icb), icbsize*l);
    
    for( ; icbsize*2 <= ecbsize; ){
	q = cb; r = cb + icbsize*l; 		/* splitting */
	for(i=0; i<icbsize; i++){
	    nrand(rnd, l, i);
	    for(j=0; j<l; j++){
		dl = delta * rnd[j];
		*r = *q - dl;
		*q = *q + dl;
		r++;
		q++;
	    }
	}
	icbsize *= 2;
	
	d0 = MAXVALUE;
	for( ;; ){
	    fillz(cntcb, sizeof(*cntcb), icbsize);
	    d1 = 0.0; p = x;
	    for(i=0; i<tnum; i++,p+=l){
		tindex[i] = vq(p, cb, l, icbsize);
		cntcb[tindex[i]]++;

		q = cb + tindex[i] * l;
		d1 += edist(p, q, l);
	    }

	    d1 /= tnum;
	    err = abs((d0 - d1) / d1);
	    
	    if(err < end)  break;		/* check distortion */

	    d0 = d1;
	    fillz(cb1, sizeof(*cb), icbsize*l);
	    
	    p = x;				/* get new centroid */
	    for(i=0; i<tnum; i++){
		q = cb1 + tindex[i] * l;
		for(j=0; j<l; j++) *q++ += *p++;
	    }
	    
	    k = maxindex = 0;
	    for(i=0; i<icbsize; i++)
		if(cntcb[i] > k){
		    k = cntcb[i];
		    maxindex = i;
		}
	    
	    q = cb; r = cb1;
	    for(i=0; i<icbsize; i++,r+=l,q+=l)
		if(cntcb[i] > 0)
		    for(j=0; j<l; j++)
			q[j] = r[j] / (double) cntcb[i];
		else{
		    nrand(rnd, l, i);
		    p = cb + maxindex * l;
		    for(j=0; j<l; j++)
			q[j] = p[j] + delta * rnd[j];
		}
	}
	if(icbsize == ecbsize) break;
    }
}
