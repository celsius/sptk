/************************************************************************
*									*
*    2nd Order Standard Form Digital Filter				*
*									*
*					1985.8  K.Tokuda		*
*					1995.12 N.Isshiki modified	*
*									*
*	usage:								*
*		df2 [ options ] [ infile ] > stdout			*
*	options:							*
*		-f f	:  sampling frequency		[10K]		*
*		-p f b	:  center frequency f(Hz)			*
*			     and band width b(Hz) of pole		*
*		-z f b  :  center frequency f(Hz)			*
*			     and band width b(Hz) of zero		*
*	note:								*
*		-p and -z option can be specified repeatedly.		*
*									*
************************************************************************/ 

static char *rcs_id = "$Id";

/* Standard C Libraries */
#include	<stdio.h>
#include	<math.h>
#include	<SPTK.h>
#include	<string.h>


/* Required Function */
double 	df2();


/* Default Values */
#define	SAMPLEF 	10000
#define SIZE		2048


/* Command Name */
char	*cmnd;


void usage(status)
int status;
{
	fprintf(stderr, "\n");
	fprintf(stderr, " %s - 2nd order standard form digital filter\n", cmnd);
	fprintf(stderr, "\n");
	fprintf(stderr, "  usage:\n");
	fprintf(stderr, "       %s [ options ] [ infile ] > stdout \n",cmnd);
	fprintf(stderr, "  options:\n");           
	fprintf(stderr, "       -f f   : sampling frequency(Hz)         [%d]\n",SAMPLEF);
	fprintf(stderr, "       -p f b : center frequency f(Hz)\n");
	fprintf(stderr, "                 and band width b(Hz) of pole\n");
	fprintf(stderr, "       -z f b : center frequency f(Hz)\n");
	fprintf(stderr, "                 and band width b(Hz) of zero\n");
	fprintf(stderr, "       -h     : print this message\n");
	fprintf(stderr, "  infile:\n");
	fprintf(stderr, "       filter input (float)                    [stdin]\n");
	fprintf(stderr, "  stdout:\n");
	fprintf(stderr, "       filter output (float)\n");
	fprintf(stderr, "  note:\n");
	fprintf(stderr, "       -p and -z option can be specified repeatedly.\n");
	fprintf(stderr, "\n");

	exit(status);
}

main(argc, argv)

int	argc;
char	*argv[];

{
        FILE    *fpp = stdin;
	int 	bufp[SIZE], fp = 0, fz = 0,i,n,fp1,fz1;
	double 	x;
	double 	sf = SAMPLEF, af0[SIZE], bf0[SIZE], ab[SIZE], bb[SIZE];
	double	buf[SIZE][3];
	double atof();
	double af01,bf01,ab1,bb1;

	if ((cmnd = strrchr(argv[0], '/')) == NULL)
	    cmnd = argv[0];
	else
	    cmnd++;	
	while (--argc)
	        if (**++argv == '-') {
           	        argc--;
			switch (*(*argv+1)) {
			case 'f':
				sf = atof(*++argv);
				break;
			case 'z':
				af0[fz] = atof(*++argv);
				argc--;
				ab[fz] = atof(*++argv);
				fz ++;
				break;
			case 'p':
				bf0[fp] = atof(*++argv);
				argc--;
				bb[fp] = atof(*++argv);
				fp ++;
				break;
			case 'h':
				usage(0);
			default:
			        fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
			        usage(1);
			}
		}
		else
		    fpp = getfp(*argv, "r");

	n = (fp>fz)?fp:fz;
	for(i=0;i<n;i++){
		fillz(buf[i], sizeof(**buf), 3);
		bufp[i]=0;
	}

	for (;;){
		if (freadf(&x, sizeof(x), 1, fpp) != 1)
			break;

		for(i = 0;i < n;i++){
			bf01=(i>=fp)?0:bf0[i];
			bb1=(i>=fp)?0:bb[i];
			af01=(i>=fz)?0:af0[i];
			ab1=(i>=fz)?0:ab[i];
			fp1 = (i>=fp)?0:1;
			fz1 = (i>=fz)?0:1;
			x = df2(x, sf, bf01, bb1, af01, ab1, fp1, fz1, buf[i], &bufp[i]);
		}
		fwritef(&x, sizeof(x), 1, stdout);
	}
	exit(0);

}
