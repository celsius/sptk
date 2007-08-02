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
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
#ifdef SPTK_VERSION
    fprintf(stderr, "\n");
    fprintf(stderr, " SPTK: version %s",SPTK_VERSION);
#endif
    fprintf(stderr, "\n");
    exit(status);
}


int main(int argc, char **argv)
{
    int		l = LENG, period = PERIOD, i, normal = NORMAL;
    double	zero, pulse;
    
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

