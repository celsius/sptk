/************************************************************************
*                                                                       *
*    Obtain Autocorrelation Sequence                                    *
*					1988.7  T.Kobayashi             *
*                                       1995.12 I.Naohiro   modified    *
*                                                                       *
*       usage:                                                          *
*               acorr [ options ] [ infile ] > stdout                   *
*       options:                                                        *
*               -m m     :  order of sequence           [12]            *
*               -l l     :  frame length                [256]           *
*       infile:                                                         *
*               stdin for default                                       *
*               input is assumed to be real                             *
*									*
************************************************************************/

static char *rcs_id = "$Id$";


/* Standard C Libraries */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


/* Required Functions */
void	acorr();


/* Default Values */
#define LENG	256
#define ORDER	25


/* Command Name  */
char	*cmnd;


void usage(status)
int status;
{
	fprintf(stderr, "\n");
	fprintf(stderr, " %s - obtain autocorrelation sequence\n", cmnd);
	fprintf(stderr, "\n");
	fprintf(stderr, "  usage:\n");
	fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
	fprintf(stderr, "  options:\n");
	fprintf(stderr, "       -m m  : order of sequence  [%d]\n", ORDER);
	fprintf(stderr, "       -l l  : frame length       [%d]\n", LENG);
	fprintf(stderr, "       -h    : print this message\n");
	fprintf(stderr, "  infile:\n");
	fprintf(stderr, "       data sequence (float)      [stdin]\n");
	fprintf(stderr, "  stdout:\n");
	fprintf(stderr, "       autocorrelation sequence (float)\n");
	fprintf(stderr, "\n");
	exit(status);
}



main(argc,argv)
int	argc;
char	*argv[];
{
  	FILE	*fp = stdin;
	char	*s, *infile = NULL, c;
	double	*x, *r;
	int	l = LENG, np = ORDER;

	if ((cmnd = strrchr(argv[0], '/')) == NULL)
	    cmnd = argv[0];
        else
	    cmnd++;
	while (--argc) {
		if(*(s = *++argv) == '-') {
			c = *++s;
			if(*++s == '\0') {
				s = *++argv;
				--argc;
			}
			switch(c) {
			case 'm':
				np = atoi(s);
				break;
			case 'l':
				l = atoi(s);
				break;
			case 'h':
				usage(0);
			default:
				fprintf(stderr,
					"%s: unknown option '%c'\n", cmnd, c);
					usage(1);
				break;
			}
		}
		else
			infile = s;
	}
	if(infile)
		fp = getfp(infile,"r");

	x = dgetmem(l + np + 1);
	r = x + l;
	while(freadf(x, sizeof(*x), l, fp) == l) {
		acorr(x,l,r,np);
		fwritef(r, sizeof(*r), np + 1, stdout);
	}
	exit(0);	
}
