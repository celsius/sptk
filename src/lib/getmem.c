/********************************************************************
    $Id: getmem.c,v 1.1 2000/03/01 13:58:55 yossie Exp $

    Memory Allocation Fanctions

	short	*sgetmem(leng)
	long	*lgetmem(leng)
	double	*dgetmem(leng)
	float	*fgetmem(leng)
	real	*rgetmem(leng)
	float	**fgetmem(leng)

	int leng : data length

	char	*getmem(leng, type)

	int     leng : data length
	unsignd type : size of data type

**********************************************************************/

#include	<stdio.h>

#ifdef	DOUBLE
#define	real	double
#else
#define	real	float
#endif


short *sgetmem(leng)
int leng;
{
    char *getmem();

    return ( (short *)getmem(leng, sizeof(short)) );
}


long *lgetmem(leng)
int leng;
{
    char *getmem();

    return ( (long *)getmem(leng, sizeof(long)) );
}


double *dgetmem(leng)
int leng;
{
    char *getmem();

    return ( (double *)getmem(leng, sizeof(double)) );
}


float *fgetmem(leng)
int leng;
{
    char *getmem();

    return ( (float *)getmem(leng, sizeof(float)) );
}


real *rgetmem(leng)
int leng;
{
    char *getmem();

    return ( (real *)getmem(leng, sizeof(real)) );
}


float **ffgetmem(leng)
int leng;
{
    char *getmem();

    return ( (float **)getmem(leng, sizeof(float *)) );
}


char *getmem(leng, size)
int	 leng;
unsigned size;
{
    char *p = NULL;

    if ((p = (char *)calloc(leng, size)) == NULL){
	fprintf(stderr, "Memory allocation error !\n");
	exit(3);
    }
    return (p);
}
