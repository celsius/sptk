/****************************************************************
  $Id: _nrand.c,v 1.1 2000/03/01 13:58:32 yossie Exp $

    Generate Normal Distributed Random Value
        nrand(p, leng, seed)

        double *p     :  generated normal distributed random value
        int    leng   :  length
        int    seed   :  seed of random value

****************************************************************/

#include	<stdio.h>
#include	<math.h>
#include	<SPTK.h>


#define	RAND_MAX	32767

double	rnd();
unsigned long	srnd();
double	nrandom();
int	nrand();


int nrand(p, leng, seed)
double	*p;
int leng, seed;
{
    int i;
    unsigned long next;

	if (seed != 1)
		next = srnd((unsigned)seed);
	for (i=0;i<leng;i++)
		p[i] = (double)nrandom(&next);

	return(0);
}

double nrandom(next)
unsigned long *next;
{
	static int	sw = 0;
	static double	r1, r2, s;

	if (sw == 0)  {
		sw = 1;
		do  {
			r1 = 2 * rnd(next) - 1;
			r2 = 2 * rnd(next) - 1;
			s = r1 * r1 + r2 * r2;
		}  while (s > 1 || s == 0);
		s = sqrt(-2 * log(s) / s);
		return ( r1 * s );
	}
	else  {
		sw = 0;
		return ( r2 * s );
	}
}

double rnd(next)
unsigned long *next;
{
	double	r;

	*next = *next * 1103515245L + 12345;
	r = (*next / 65536L) % 32768L;

	return ( r / RAND_MAX ); 
}

unsigned long srnd( seed )
	unsigned	seed;
{
	return(seed);
}
