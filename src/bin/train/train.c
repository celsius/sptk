/************************************************************************
*									*
*    Generate Pulse Sequence						*
*									*
*					1989.6  K.Tokuda		*
*					1996.4  K.Koishida		*
*									*
*	usage:								*
*		train [options] > stdout				*
*	options:							*
*		-l l     :  length		 	[256]		*
*		-p p     :  period		 	[0]		*
*		-n n     :  type of normalization	[1]		*
*                       n=0: none                                       *
*                       n=1: normalize by power                         *
*                       n=2: normalize by magnitude                     *
*	stdout:								*
*		p > 0 pulse sequence with pitch p			*
*		p = 0 M-sequence					*
*									*
************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


/*  Default Values  */
#define LENG 		256
#define PERIOD 		0
#define NORMAL		1


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - generate pulse sequence\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -l l  : frame length          [%d]\n", LENG);
    fprintf(stderr, "       -p p  : frame period          [%d]\n", PERIOD);
    fprintf(stderr, "       -n n  : type of normalization [%d]\n",NORMAL);
    fprintf(stderr, "                 0 none\n");
    fprintf(stderr, "                 1 normalize by power\n");
    fprintf(stderr, "                 2 normalize by magnitude\n");
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       pulse sequence (float)\n");
    fprintf(stderr, "\n");
    exit(status);
}


void main(int argc, char **argv)
{
    int		l = LENG, period = PERIOD, i, normal = NORMAL;
    double	zero, pulse, sqrt();
    
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
		    period = atoi(*++argv);
		    --argc;
		    break;
		case 'n':
		    normal = atoi(*++argv);
		    --argc;
		    break;
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
		}
	}

    switch (normal){
	case 0:
	    pulse = 1;
	    break;
	case 1:
	    pulse = sqrt((double) period);
	    break;
	case 2:
	    pulse = period;
	    break;
    }
    zero = 0.0;
    i = 1;
    
    if(period)
	while(l--)
	    if(--i)
		fwritef(&zero, sizeof(zero), 1, stdout);
	    else{
		i = period;
		fwritef(&pulse, sizeof(pulse), 1, stdout);
	    }
    else
	while(l--){
	    pulse = mseq();
	    fwritef(&pulse, sizeof(pulse), 1, stdout);
	}
    
    exit(0);
}

