/****************************************************************

    $Id$

    Decoder of Vector Quantization

	void ivq(index, cb, l, x)

	int    index  : index of codebook
	double *cb    : codebook vector
	int    l      : vector order
	double *x     : decoded vector

*****************************************************************/

void ivq(index, cb, l, x)
int index, l;
double *cb, *x;
{
    movem((cb+index*l), x, sizeof(*cb), l);
}

