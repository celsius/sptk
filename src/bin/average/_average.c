/***************************************************************
    $Id: _average.c,v 1.1.1.1 2000/03/01 13:58:29 yossie Exp $

    Calculation of Average

	double	average(x, n)

	double	*x  : data sequence
	int	n   : data length

	return value :  average of data

***************************************************************/

double average(x, n)
double *x;
int n;
{
    register int  i;
    double	  sum = 0.0;

    for(i=0; i<n; i++) sum += x[i];

    return(sum / n);
}

void vaverage(x, l, num, ave)
double *x, *ave;
int l, num;
{
    register int    i, j;
    
    fillz(ave, sizeof(*ave), l);
    for(i=0; i<num; i++)
	for(j=0; j<l; j++)
	    ave[j] += *x++;

    for(j=0; j<l; j++)
	ave[j] /= (double) num;
}

