/************************************************************************
*									*
*    Plot a Graph							*
*									*
*					1990.9  T.Kobayashi		*
*					1996.5  K.Koishida		*
*									*
*	usage:								*
*		fig [options] [infile] > stdout				*
*	options:							*
*		-F F     :  factor				[1]	*
*		-R R     :  rotation angle                      [0]	*
*		-W W     :  width of figure                     [1]	*
*		-H H     :  height of figure                    [1]	*
*		-o xo yo :  origin in mm                        [20 20] *
*		-g g     :  draw grid                           [1]	*
*		-p p     :  pen number                          [1]	*
*		-s s     :  font size                           [0]	*
*		-f f     :  first opened file			[NULL]  *
*		-t       :  transpose x and y axes              [FALSE] *
*									*
************************************************************************/

/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>

typedef enum _Boolean {FA, TR} Boolean;
char *BOOL[] = {"FALSE", "TRUE"};

/*  Default Values  */
#define	LENG	100
#define CH_NRML	2.5
#define CH_BIG	5.0
#define CW_NRML	2.2
#define CW_BIG	2.6
#define CW_HUGE	4.4

#define TYPE		2
#define FCT		1.0
#define TH		0.0
#define FNTSIZE		0
#define IS_T		FA
#define XO		20.0
#define YO		20.0
#define PENNO		1
#define XL		LENG
#define YL		LENG

/*  Command Name  */
char	*cmnd;

void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - plot a graph\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n", cmnd);
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -F F     : factor                 [%g]\n",FCT);
    fprintf(stderr, "       -R R     : rotation angle         [%g]\n",TH);
    fprintf(stderr, "       -W W     : width of figure        [1]\n");
    fprintf(stderr, "       -H H     : height of figure       [1]\n");
    fprintf(stderr, "       -o xo yo : origin in mm           [%g %g]\n",XO,YO);
    fprintf(stderr, "       -g g     : draw grid              [%d]\n",TYPE);
    fprintf(stderr, "       -p p     : pen number             [%d]\n",PENNO);
    fprintf(stderr, "       -s s     : font size              [%d]\n",FNTSIZE+1);
    fprintf(stderr, "       -f f     : first opened file      [NULL]\n");
    fprintf(stderr, "       -t       : transpose x and y axes [%s]\n",BOOL[IS_T]);
    fprintf(stderr, "       -h       : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       command\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       plotter command\n");
    fprintf(stderr, "\n");

    exit(status);
}

double	atof();
int	ltype = 0, penno = PENNO, type = TYPE, is_t = IS_T;
float	xo = XO, yo = YO;
float	xl = XL, yl = YL, x0 = 0, y0 = 0;
float	mh = 2, mw = 2, h = CH_NRML, w = CW_NRML;
float	xclip0 = 0, yclip0 = 0, xclip1 = 0, yclip1 = 0;

main(argc, argv)
int	argc;
char	*argv[];
{
	FILE	*fp;
	char	*infile[16], *file = NULL;
	register char	*s;
	float	th = TH, fct = FCT;
	int	c, n, nfiles, fntsize = FNTSIZE;
	
        if ((cmnd = strrchr(argv[0], '/')) == NULL)
	    cmnd = argv[0];
        else
	    cmnd++;

	for(nfiles = 0; --argc;) {
		if(*(s = *++argv) == '-') {
			c = *++s;
			if(*++s == '\0' && c != 't') {
				s = *++argv;
				--argc;
			}
			switch(c) {
			    case 'g':
				type = atoi(s);
				break;
			    case 'F':
				fct = atof(s);
				break;
			    case 'R':
				th = atof(s);
				break;
			    case 's':
				fntsize = atoi(s) - 1;
				break;
			    case 't':
				is_t = 1 - is_t;
				break;
			    case 'o':
				xo = atof(s);
				yo = atof(*++argv);
				--argc;
				break;
			    case 'f':
				file = s;
				break;
			    case 'p':
				penno = atoi(s);
				break;
			    case 'W':
				xl = atof(s) * LENG;
				break;
			    case 'H':
				yl = atof(s) * LENG;
				break;
			    case 'h':
				usage(0);
			    default:
				usage(1);
			}
		}
		else
			infile[nfiles++] = *argv;
	}

	if(fntsize < 0 || fntsize > 3)
		fntsize = 0;
	if(fntsize & 1)
		h = CH_BIG;
	if(fntsize & 2)
		w = CW_HUGE;
	if(fntsize == 1)
		w = CW_BIG;

	plots(0x81); rstbnd();
	font(fntsize);font(0);		/* valid only LBP */
	plot(xo, yo, -3);
	rotate(th); pen(1); italic(0.0); mode(0, 10);
	xl *= fct; yl *= fct;
	xclip1 = xl; yclip1 = yl; swap(&xclip1, &yclip1);
	if(type--) {
		plot(0.0, 0.0, -3);
		plot(xl, 0.0, 2);
		plot(xl,  yl, type ? 2 : 3);
		plot(0.0, yl, type ? 2 : 3);
		plot(0.0, 0.0, 2);
	}
	pen(penno);

	if(file) {
		if(fp = fopen(file, "r"))
			graph(fp);
	}
	if(nfiles == 0)
		graph(stdin);
	else {
		for(n = 0; n < nfiles; ++n)
			if(fp = fopen(infile[n], "r")) {
				graph(fp);
				fclose(fp);
			}
	}
	plote();
	exit(0);
}
