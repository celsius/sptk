/************************************************************************
*									*
*    Obtain Zero Cross							*
*									*
*					1996.3	N.Isshiki		*
*									*
*	usage:								*
*		zcross [ options ] [ infile ] >stdout			*
*	options:							*
*		-l l	 :  frame length		[256]		*
*		-n	 :  normalized by frame langth			*
*	infile:								*
*		stdin for default					*
*		input is assumed to be real				*
************************************************************************/

static char *rcs_id = "$Id: zcross.c,v 1.1 2000/03/01 13:58:55 yossie Exp $";


/* Standard C Libraries */
#include <stdio.h>
#include <SPTK.h>
#include <string.h>

typedef enum _Boolean {FA, TR} Boolean;
char *BOOL[] = {"FALSE", "TRUE"};

/* Required Functions */
double	zcross();

/* Default Values */
#define FLENG	256
#define NORM	FA

/* Command Name  */
char	*cmnd;

void usage(status)
int status;
{
	fprintf(stderr, "\n");
	fprintf(stderr, " %s - zero cross\n", cmnd);
	fprintf(stderr, "\n");
	fprintf(stderr, "  usage:\n");
	fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
	fprintf(stderr, "  options:\n");
	fprintf(stderr, "       -l l  : frame length               [%d]\n",FLENG);
	fprintf(stderr, "       -n    : normarized by frame length\n");
	fprintf(stderr, "       -h    : print this message\n");
	fprintf(stderr, "  infile:\n");
	fprintf(stderr, "       data sequence (float)              [stdin]\n");
	fprintf(stderr, "  stdout:\n");
	fprintf(stderr, "       zero cross rate (float)\n");
	fprintf(stderr, "\n");
	exit(status);
}
	
main(argc, argv)
int	argc;
char	**argv;
{

  	FILE	*fp = stdin;
	char	*s, *infile = NULL, c;
	double	*x;
	int	flng = FLENG;
	double	z;
	Boolean norm = NORM;

        if ((cmnd = strrchr(argv[0], '/')) == NULL)
		cmnd = argv[0];
        else
		cmnd++;
	
	while(--argc) {
		if(*(s = *++argv) == '-') {
			c = *++s;
			if(c != 'n' && *++s == '\0') {
				s = *++argv;
				--argc;
			}
			switch(c) {
			case 'l':
				flng = atoi(s);
				break;
			case 'n':
				norm = 1 - norm;
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
	
	x = dgetmem(flng);
	while(freadf(x, sizeof(*x), flng, fp) == flng){
		z = zcross(x, flng, norm);
		fwritef(&z, sizeof(z), 1, stdout);
	}
	exit(0);
}
