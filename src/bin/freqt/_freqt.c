/***************************************************************

    $Id: _freqt.c,v 1.1 2000/03/01 13:58:34 yossie Exp $

    Frequency Transformation

	void	freqt(c1, m1, c2, m2, a)

	double	*c1   : minimum phase sequence
	int	m1  : order of minimum phase sequence
	double	*c2   : warped sequence
	int	m2  : order of warped sequence
	double	a     : all-pass constant

***************************************************************/

#include <stdio.h>
#include <SPTK.h>

void freqt(c1, m1, c2, m2, a)
double *c1, *c2, a;
int m1, m2;
{
    register int 	i, j;
    double		b;
    static double	*d = NULL, *g;
    static int		size;
    
    if(d == NULL){
	size = m2;
	d = dgetmem(size+size+2);
	g = d + size + 1;
    }

    if(m2 > size){
	free(d);
	size = m2;
	d = dgetmem(size+size+2);
	g = d + size + 1;
    }
    
    b = 1 - a*a;
    fillz(g, sizeof(*g), m2+1);

    for (i=-m1; i<=0; i++){
	if (0 <= m2)
	    g[0] = c1[-i] + a*(d[0] = g[0]);
	if (1 <= m2)
	    g[1] = b*d[0] + a*(d[1] = g[1]);
	for (j=2; j<=m2; j++)
	    g[j] = d[j-1] + a*((d[j]=g[j]) - g[j-1]);
    }
    
    movem(g, c2, sizeof(*g), m2+1);
}
