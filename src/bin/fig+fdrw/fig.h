/********************************************************
* NAME :	fig.h - Header for fig			*
********************************************************/

#include <stdio.h>
#include <ctype.h>

#define	BUFLNG	512
#define	SBUFLNG	81
#define	LSCALE	1.5
#define	MSCALE	1.5
#define	NSCALE	3
#define	MADJ	1.0
#define	LADJ	0.8
#define	SSIZE	0.6

#define	is_number(c)	(isdigit(c) || c == '-')

double	atof(), atan2(), sin(), cos(), log10();
