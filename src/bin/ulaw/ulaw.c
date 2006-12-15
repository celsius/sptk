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
*    u-law PCM								*
*									*
*					1991.3  T.Kanno			*
*					1996.11 K.Koishida		*
*									*
*	usage:								*
*		u-law [ options ] [ infile ] > stdout			*
*	options:							*
*		-v v     :  maximum of input		  [32768]	*
*		-u u     :  compression ratio		  [256] 	*
*		-c       :  coder mode			  [TRUE]   	*
*		-d       :  decoder mode		  [FALSE]   	*
*	infile:								*
*		input sequence						*
*		    , x(0), x(1), ..., 					*
*	stdout:								*
*		compressed sequence					*
*		    , x'(0), x'(1), ...,				*
*	require:							*
*		ulaw_e(), ulaw_d()					*
*									*
************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SPTK.h>


typedef enum _Boolean {FA, TR} Boolean;
char *BOOL[] = {"FALSE", "TRUE"};

/*  Default Values  */
#define MAXVALUE	32768.0
#define MU		256.0
#define CODER		TR
#define DECODER		FA


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - u-law PCM\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -v v  : maximum of input    [%g]\n", MAXVALUE);
    fprintf(stderr, "       -u u  : compression ratio   [%g]\n", MU);
    fprintf(stderr, "       -c    : coder mode          [%s]\n", BOOL[CODER]);
    fprintf(stderr, "       -d    : decoder mode        [%s]\n", BOOL[DECODER]);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       input sequence      (float) [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       compressed sequence (float)\n");
    fprintf(stderr, "\n");
    exit(status);
}

int main(int argc, char **argv)
{
    double	x, y, max = MAXVALUE, mu = MU, atof();
    Boolean	decoder = DECODER;
    FILE	*fp = stdin;
    
    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc)
	if (**++argv == '-') {
	    switch (*(*argv+1)) {
		case 'v':
		    max = atof(*++argv);
		    --argc;
		    break;
		case 'u':
		    mu = atof(*++argv);
		    --argc;
		    break;
		case 'c':
		    decoder = FA;
		    break;
		case 'd':
		    decoder = TR;
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

    if(! decoder)
	while (freadf(&x, sizeof(x), 1, fp) == 1){
	    y = ulaw_c(x, max, mu);
	    fwritef(&y, sizeof(y), 1, stdout);
	}
    else
	while (freadf(&x, sizeof(x), 1, fp) == 1){
	    y = ulaw_d(x, max, mu);
	    fwritef(&y, sizeof(y), 1, stdout);
	}

    exit(0);
}
