/************************************************************************
*									*
*    Find NaN & Infty & Indefinite Values				*
*									*
*					1997.7	G.Hirabayashi		*
*									*
*	usage:								*
*		nan [ options ] [ infile ] > stdout			*
*	options:							*
*		-d       :  input is assumed to be double       [FALSE]	*
*	infile:								*
*		stdin for default					*
*		input is assumed to be float				*
*									*
************************************************************************/
static char *rcs_id = "$Id: nan.c,v 1.1.1.1 2000/03/01 13:58:39 yossie Exp $";

/* Standard C Libraries */
#include <stdio.h>
#include <SPTK.h>
#include <string.h>


/* Defualt Values */
#define DFLAG			0
#define F_MASKEXP		0x7F800000L
#define F_MASKSIG		0x007FFFFFL
#define F_INDEFINITE		0xFFC00000L
#define	D_MASKEXP		0x7FF00000L
#define	D_MASKSIG		0x000FFFFFL
#define D_INDEFINITE		0xFFF80000L


/* Command Name */
char	*cmnd;


void usage(status)
int status;
{
	fprintf(stderr, "\n");
	fprintf(stderr, " %s - find NaN and Infty & Indefinite values\n", cmnd);
	fprintf(stderr, "\n");
	fprintf(stderr, "  usage:\n");
	fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
	fprintf(stderr, "  options:\n");
	fprintf(stderr, "       -d    : input is assumed to be double [FALSE]\n");
	fprintf(stderr, "       -h    : print this message\n");
	fprintf(stderr, "  infile:\n");
	fprintf(stderr, "       data sequence (float)        [stdin]\n");
	fprintf(stderr, "  stdout:\n");
	fprintf(stderr, "       result messages\n");
	fprintf(stderr, "\n");
	exit(status);
}

main(argc,argv)
int	argc;
char	*argv[];
{
	FILE	*fp;
	char	*s, *infile = NULL, c;
	int	dflag = DFLAG;
	
        if ((cmnd = strrchr(argv[0], '/')) == NULL)
		cmnd = argv[0];
        else
		cmnd++;
	while (--argc)
	{
	    if(*(s = *++argv) == '-')
	    {
		c = *++s;
		switch(c)
		{
		case 'd':
			dflag = 1 - dflag;
			break;
		case 'h':
			usage(0);
		default:
			fprintf(stderr, "%s: unknown option '%c'\n", cmnd, c);
			usage(1);
			break;
		}
	    }
	    else
		infile = s;
	}
	if(infile)	fp = getfp(infile, "r");
	else		fp = stdin;

	nan(fp, dflag);
	exit(0);
}

nan(fp, dflag)
    FILE	*fp;
    int		dflag;
{
	long	*upper, *lower, count = 0;
	double	xd;
	long	xf;
	
	if(dflag)
	{
	    while(fread(&xd, sizeof(xd), 1, fp))
	    {
		upper = (long*)&xd;
		lower = upper + 1;

		if(!((*upper & D_MASKEXP) ^ D_MASKEXP))
		{
		    if ((!(*upper & D_MASKSIG))&&(!*lower))
			fprintf(stdout, "[No. %ld] is Infty\n", count);
		    else if((*upper == D_INDEFINITE)&&(!*lower))
			fprintf(stderr, "[No. %ld] is Indifinite\n", count);
		    else
			fprintf(stdout, "[No. %ld] is NaN\n", count);
		}
		++count;
	    }
	}
	else
	{
	    while(fread(&xf, sizeof(xf), 1, fp))
	    {
		if(!((xf & F_MASKEXP) ^ F_MASKEXP))
		{
		    if(!(xf & F_MASKSIG))
			fprintf(stdout, "[No. %ld] is Infty\n", count);
		    else if(xf == F_INDEFINITE)
			fprintf(stderr, "[No. %ld] is Indifinite\n", count);
		    else
			fprintf(stdout, "[No. %ld] is NaN\n", count);
		}
		++count;
	    }
	}
}

