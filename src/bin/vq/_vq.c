/****************************************************************

    $Id: _vq.c,v 1.1 2000/03/01 13:58:38 yossie Exp $

    Vector Quantization

	int vq(x, cb, l, cbsize)

	double  *x     : input vector
	double  *cb    : codebook vector
	int     l      : vector order
	int     cbsize : size of codebook

	return value   : codebook index

*****************************************************************/

int vq(x, cb, l, cbsize)
double *x, *cb;
int cbsize, l;
{
    int           i, index;
    double	  min = 1e23, dist, edist();
    
    for(i=0; i<cbsize; i++){
	dist = edist(x, cb, l);
	if(dist < min){
	    index = i;
	    min = dist;
	}
	cb += l;
    }
    return(index);
}

double edist(x, y, m)
double *x, *y;
int m;
{
    register int  i;
    double	  sub, dist = 0.0;
    
    for(i=0; i<m; i++){
	sub = x[i] - y[i];
	dist += sub * sub;
    }
    return(dist / (double)m);
}

