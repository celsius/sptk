/********************************************************
    $Id: movem.c,v 1.1.1.1 2000/03/01 13:58:56 yossie Exp $

    Data Transfer Function

	movem(a, b, size, nitem)

	xxxx	*a    : intput data
	xxxx	*b    : output data
	int	size  : size of data type
	int	nitem : data length

*********************************************************/

void movem(a, b, size, nitem)
register char *a, *b;
int size, nitem;
{
    register long i;

    i = size * nitem;
    if (a > b)
	while (i--) *b++ = *a++;
    else{
	a += i; b += i;
	while (i--) *--b = *--a;
    }
}
