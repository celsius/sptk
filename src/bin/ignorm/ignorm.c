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
*	Inverse Gain Normalization					*
*					1995.11  T.Masuko		*
*									*
*	usage:								*
*		ignorm [ options ] [ infile ] > stdout			*
*	options:							*
*		-m m     :  order of generalized cepstrum	[25]	*
*		-g g     :  gamma 				[0.0]	*
*	infile:								*
*		normalized generalized cepstral coefficients		*
*		    , K, c'(1), ..., c'(M),				*
*	stdout:								*
*		generalized cepstral coefficients			*
*		    , c~(0), c~(1), ..., c~(M),				*
*	notice:								*
*		if g > 1.0, g = -1 / g .				*
*	require:							*
*		ignorm(),						*
*									*
************************************************************************/

static char *rcs_id = "$Id: ignorm.c,v 1.2 2002/12/25 05:30:29 sako Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


/*  Required Functions  */
void ignorm();


/*  Default Values  */
#define	GAMMA		0.0
#define ORDER		25


/*  Command Name  */
char	*cmnd;


usage(status)
int	status;
{
	fprintf(stderr, "\n");
	fprintf(stderr, " %s - inverse gain normalization\n", cmnd);
	fprintf(stderr, "\n");
	fprintf(stderr, "  usage:\n");
	fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
	fprintf(stderr, "  options:\n");
	fprintf(stderr, "       -m m  : order of generalized cepstrum   [%d]\n", ORDER);
	fprintf(stderr, "       -g g  : gamma                           [%g]\n", GAMMA);
	fprintf(stderr, "       -h    : print this message\n");
	fprintf(stderr, "  infile:\n");
	fprintf(stderr, "       normalized generalized cepstrum (float) [stdin]\n");
	fprintf(stderr, "  stdout:\n");
	fprintf(stderr, "       generalized cepstrum (float)\n");
	fprintf(stderr, "  notice:\n");
	fprintf(stderr, "       if g > 1.0, g = -1 / g \n");
	fprintf(stderr, "\n");
	exit(status);
}

main(argc, argv)
int	argc;
char	**argv;
{
    double	g = GAMMA, *c, atof();
    int		m = ORDER;
    FILE	*fp = stdin;
    

    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    
    while (--argc)
	if (**++argv == '-') {
	    switch (*(*argv+1)) {
	        case 'g':
		    g = atof(*++argv);
		    --argc;
		    if (g > 1.0) g = -1.0 / g;
		    break;
		case 'm':
		    m = atoi(*++argv);
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
    
    c = dgetmem(m+1);
    
    while (freadf(c, sizeof(*c), m+1, fp) == m+1) {
	ignorm(c, c, m, g);
	fwritef(c, sizeof(*c), m+1, stdout);
    }
    exit(0);
}

