/*
  ----------------------------------------------------------------
	Speech Signal Processing Toolkit (SPTK): version 3.0
			 SPTK Working Group

		   Department of Computer Science
		   Nagoya Institute of Technology
				and
    Interdisciplinary Graduate School of Science and Engineering
		   Tokyo Institute of Technology
		      Copyright (c) 1984-2000
			All Rights Reserved.

  Permission is hereby granted, free of charge, to use and
  distribute this software and its documentation without
  restriction, including without limitation the rights to use,
  copy, modify, merge, publish, distribute, sublicense, and/or
  sell copies of this work, and to permit persons to whom this
  work is furnished to do so, subject to the following conditions:

    1. The code must retain the above copyright notice, this list
       of conditions and the following disclaimer.

    2. Any modifications must be clearly marked as such.

  NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSITITUTE OF TECHNOLOGY,
  SPTK WORKING GROUP, AND THE CONTRIBUTORS TO THIS WORK DISCLAIM
  ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT
  SHALL NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSITITUTE OF
  TECHNOLOGY, SPTK WORKING GROUP, NOR THE CONTRIBUTORS BE LIABLE
  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
  DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
  WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
  ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
  PERFORMANCE OF THIS SOFTWARE.
 ----------------------------------------------------------------
*/

/************************************************************************
*									*
*    Transform  Cepstrum to Spectrum					*
*									*
*					1986. 9  K.Tokuda		*
*					1996. 4  K.Koishida		*
*									*
*	usage:								*
*		c2sp [ options ] [ infile ] > stdout			*
*	options:							*
*		-m m     :  order of cepstrum		[25]		*
*		-l l     :  frame length		[256]		*
*		-p       :  output phase  (see stdout)	[FALSE]		*
*		-o o     :  output format (see stdout)	[0]		*
*	infile:								*
*		cepstrum						*
*		    , c(0), c(1), ..., c(M),				*
*	stdout:								*
*	        output format       scale				*
*		       0 :	      20 * log|H(z)|			*
*		       1 :            ln|H(z)|				*
*		       2 :            |H(z)|				*
*	        (-p option is used)					*
*		       0 :	      arg|H(z)| / pi       [pi rad]	*
*		       1 :            arg|H(z)|            [rad]	*
*		       2 :            arg|H(z)| * 180 / pi [deg]	*
*		spectrum						*
*		    , s(0), s(1), ..., s(L/2),				*
*	require:							*
*		c2sp()							*
*									*
************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SPTK.h>


typedef enum _Boolean {FA, TR} Boolean;
char *BOOL[] = {"FALSE", "TRUE"};


/*  Required Functions  */
double  exp(), log();


/*  Default Values  */
#define ORDER		25
#define LENG		256
#define PHASE		FA
#define OTYPE		0


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - transform cepstrum to spectrum\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -m m  : order of cepstrum    [%d]\n", ORDER);
    fprintf(stderr, "       -l l  : frame length         [%d]\n", LENG);
    fprintf(stderr, "       -p    : output phase         [%s]\n", BOOL[PHASE]);
    fprintf(stderr, "       -o o  : output format        [%d]\n", OTYPE);
    fprintf(stderr, "                0 (20*log|H(z)|)\n");
    fprintf(stderr, "                1 (ln|H(z)|)\n");
    fprintf(stderr, "                2 (|H(z)|)\n");
    fprintf(stderr, "               -p option is specified\n");
    fprintf(stderr, "                0 (arg|H(z)|/pi       [pi rad])\n");
    fprintf(stderr, "                1 (arg|H(z)|          [rad])\n");
    fprintf(stderr, "                2 (arg|H(z)|*180 / pi [deg])\n");
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       cepstrum (float)             [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       spectrum (float)\n");
#ifdef SPTK_VERSION
    fprintf(stderr, "\n");
    fprintf(stderr, " SPTK: version %s",SPTK_VERSION);
#endif
    fprintf(stderr, "\n");
    exit(status);
}


int main(int argc, char **argv)
{
    int     leng = LENG, m = ORDER, start = 0, end = 0, otype = OTYPE,
    	    i, no;
    FILE    *fp = stdin;
    char    phase = PHASE;
    double  logk, *x, *y, *c;

    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc)
	if (**++argv == '-') {
	    switch (*(*argv+1)) {
		case 'm':
		    m = atoi(*++argv);
		    --argc;
		    break;
		case 'l':
		    leng = atoi(*++argv);
		    --argc;
		    break;
		case 'p':
		    phase = 1 - phase;
		    break;
		case 'o':
		    otype = atoi(*++argv);
		    --argc;
		    break;
		case 's':
		    start = atoi(*++argv);
		    --argc;
		    break;
		case 'e':
		    end = atoi(*++argv);
		    --argc;
		    break;
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
		}
	}
	else 
	    fp = getfp(*argv, "r");

    if(end == 0) end = m;
    
    x = dgetmem(leng*2);
    y = x + leng;
    c = dgetmem(m+1);

    no = leng / 2 + 1;
    logk = 20.0 / log(10.0);
    
    while (freadf(c, sizeof(*c), m+1, fp) == m+1){
	fillz(c, sizeof(*c), start);
	for (i=end+1; i<=m; i++)  c[i] = 0.0;
	
	c2sp(c, m, x, y, leng);

	if (phase)
	    switch (otype){
	        case 1 :
		    for (i = no; i--;) x[i] = y[i];
		    break;
		case 2 :
		    for (i = no; i--;)
			x[i] = y[i] * 180 / PI;
		    break;
		default :		
		    for (i = no; i--;)
			x[i] = y[i] / PI;
		    break;
		}
	else
	    switch (otype){
	        case 1 :
		    break;
                case 2 :
		    for (i = no; i--;)
			x[i] = exp(x[i]);
	            break;
		default :
		    for (i = no; i--;)
			x[i] *= logk;
	            break;
		}

	fwritef(x, sizeof(*x), no, stdout);
    }
    exit(0);
}
