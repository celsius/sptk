/************************************************************************
*									*
*    clipping								*
*									*
*					1989.8.31   K.Tokuda		*
*									*
*	usage:								*
*		clip [ -y Ymin Ymax] [infile] > stdout			*
*	options:							*
*	        -y    ymin ymax : lower bound & upper bound [-1.0 1.0]  *
*	        -ymin ymin      : lower bound (ymax = inf)  [N/A]       *
*	        -ymax ymax      : upper bound (ymin = -inf) [N/A]       *
*	infile:								*
*		data sequence (float)					*
*	stdout:								*
*		clipped data sequence (float)				*
*	require:							*
*		clip()							*
*									*
************************************************************************/


/* Standard C Libraries */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


/* Default Value */
#define FLT_MAX 3.4e+38
#define FLT_MIN -3.4e+38
#define YMIN	-1.0
#define YMAX	1.0


/* Command Name */
char	*cmnd;

void usage()
{
	fprintf(stderr, "\n");
	fprintf(stderr, " %s - data clipping\n", cmnd);
	fprintf(stderr, "\n");
	fprintf(stderr, "  usage:\n");
	fprintf(stderr, "       %s [ option ] [ infile ] > outfile\n", cmnd);
	fprintf(stderr, "  option:\n");
	fprintf(stderr, "       -y    ymin ymax : lower bound & upper bound [-1.0 1.0]\n");
	fprintf(stderr, "       -ymin ymin      : lower bound (ymax = inf)  [N/A]\n");
	fprintf(stderr, "       -ymax ymax      : upper bound (ymin = -inf) [N/A]\n");
	fprintf(stderr, "       -h              : print this message\n");
	fprintf(stderr, "  infile:\n");
	fprintf(stderr, "       data sequence (float)                       [stdin]\n");
	fprintf(stderr, "  stdout:\n");
	fprintf(stderr, "       clipped data sequence (float)\n");
	fprintf(stderr, "\n");
	exit(1);
}

main(argc, argv)
	int	argc;
	char	*argv[];
{
        FILE    *fp = stdin;
	int	size;
	double	ymin = YMIN, ymax = YMAX;
	double	*x, *y;
	double	atof();

	if ((cmnd = strrchr(argv[0],'/')) == NULL)
	    cmnd = argv[0];
	else
	    cmnd++;
	while (--argc)
		if (**++argv == '-'){
			argc--;
			switch (*(*argv+1)) {
			case 'y':
				if((*(*argv+2))=='m'){
					switch (*(*argv+3)) {
						case 'i':
							ymin = atof(*++argv);
							ymax = FLT_MAX;
							break;
						case 'a':
							ymax = atof(*++argv);
							ymin = FLT_MIN;
							break;
					}
				} else{
					ymin = atof(*++argv);
					argc--;
					ymax = atof(*++argv);
				}
				break;
			case 'h':
			default:
				usage();
			}
		} 
		else 
		    fp = getfp(*argv, "r");

	x = dgetmem(2*256);
	y = x + 256;

	while ((size = freadf(x, sizeof(*x), 256, fp)) !=0){
		clip (x, size, ymin, ymax, y);
		fwritef(y, sizeof(*y), size, stdout);
	}
	exit(0);
}
