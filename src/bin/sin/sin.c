/************************************************************************
*									*
*    Generate Sinusoidal Sequence					*
*									*
*					1989.7  K.Tokuda		*
*					1996.4  K.Koishida		*
*									*
*	usage:								*
*		sin [options] > stdout					*
*	options:							*
*		-l l     :  length		 	[256]		*
*		-p p     :  period		 	[10.0]		*
*		-m m     :  magnitude		 	[1.0]		*
*	stdout:								*
*		sinusoidal sequence					*
*		    , sin(2pi*0/p), sin(2pi*1/p), ..., sin(2pi*(l-1)/p	*
*	notice:								*
*		if l < 0, generate infinite sequence			*
*									*
************************************************************************/

static char *rcs_id = "$Id: sin.c,v 1.1.1.1 2000/03/01 13:58:50 yossie Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


/*  Default Values  */
#define LENG 		256
#define PERIOD 		10.0
#define MAGNITUDE	1.0


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - generate sinusoidal sequence\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n", cmnd);
    fprintf(stderr, "       %s [ options ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -l l  : length             [%d]\n", LENG);
    fprintf(stderr, "       -p p  : period             [%g]\n", PERIOD);
    fprintf(stderr, "       -m m  : magnitude          [%g]\n", MAGNITUDE);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       sinusoidal sequence (float)\n");
    fprintf(stderr, "  notice:\n");
    fprintf(stderr, "       if l < 0, generate infinite sequence\n");
    fprintf(stderr, "\n");
    exit(status);
}


void main(int argc, char **argv)
{
    int		l = LENG, i;
    double	mag = MAGNITUDE, period = PERIOD, x, sin(), atof();
    
    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc)
	if (**++argv == '-') {
	    switch (*(*argv+1)) {
		case 'l':
		    l = atoi(*++argv);
		    --argc;
		    break;
		case 'p':
		    period = atof(*++argv);
		    --argc;
		    break;
		case 'm':
		    mag = atof(*++argv);
		    --argc;
		    break;
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
		}
	}

    for(i=0;; i++){
	x = mag * sin(PI2 * (double) i / period);
	fwritef(&x, sizeof(x), 1, stdout);

	if(i == l-1) break;
    }

    exit(0);
}

