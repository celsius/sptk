/***********************************************************
	Speech Signal Processing Toolkit
		SPTK.h
***********************************************************/

#ifndef PI
#define	PI	3.14159265358979323846
#define PI2	6.28318530717958647692
#endif

/* #ifndef ABS(x) */
#define ABS(x)	(x < 0. ? -x : x)
/* #endif */

typedef enum _Window {
	BLACKMAN,
	HAMMING,
	HANNING,
	BARTLETT,
	TRAPEZOID,
	RECTANGULAR
} Window;

void	fillz();
void	movem()	;
short	*sgetmem();
long	*lgetmem();
float	*fgetmem();
float	**ffgetmem();
/*real	*rgetmem();*/
double	*dgetmem();
char	*getmem();

FILE	*getfp();

#ifdef DOUBLE
#define	fwritef	fwrite
#define	freadf	fread
#else	/* DOUBLE */
int	fwritef();
int	freadf();
#endif	/* DOUBLE */

int	theq();

