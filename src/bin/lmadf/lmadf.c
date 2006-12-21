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
*    LMA Digital Filter for Speech Synthesis				*
*									*
*					1986.6  K.Tokuda		*
*					1996.1  K.Koishida		*
*									*
*	usage:								*
*		lmadf [ options ] [ infile ] > stdout			*
*	options:							*
*		-m m     :  order of cepstrum		[25]		*
*		-p p     :  frame period		[100]		*
*		-i i     :  interpolation period	[1]		*
*		-P P	 :  order of pade approximation [4]		*
*		-k	 :  filtering without gain  [FALSE]		*
*	infile:								*
*		cepstral coefficients					*
*		    , c~(0), c~(1), ..., c~(M),				*
*		excitation sequence					*
*		    , x(0), x(1), ..., 					*
*	stdout:								*
*		filtered sequence					*
*		    , y(0), y(1), ...,					*
*	note:								*
*		P = 4 or 5						*
*	require:							*
*		lmadf()							*
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
double	exp();


/*  Default Values  */
#define ORDER		25
#define	FPERIOD		100
#define	IPERIOD		1
#define PADEORD 	4
#define NGAIN		FA


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - LMA digital filter for speech synthesis\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] cfile [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -m m  : order of cepstrum           [%d]\n", ORDER);
    fprintf(stderr, "       -p p  : frame period                [%d]\n", FPERIOD);
    fprintf(stderr, "       -i i  : interpolation period        [%d]\n", IPERIOD);
    fprintf(stderr, "       -P P  : order of pade approximation [%d]\n", PADEORD);
    fprintf(stderr, "       -k    : filtering without gain      [%s]\n", BOOL[NGAIN]);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       filter input (float)                [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       filter output (float)\n");
    fprintf(stderr, "  cfile:\n");
    fprintf(stderr, "       cepstrum (float)\n");
    fprintf(stderr, "  note:\n");
    fprintf(stderr, "       P = 4 or 5\n");
#ifdef SPTK_VERSION
    fprintf(stderr, "\n");
    fprintf(stderr, " SPTK: version %s",SPTK_VERSION);
#endif
    fprintf(stderr, "\n");
    exit(status);
}


int main(int argc, char **argv)
{
    int		m = ORDER, fprd = FPERIOD, iprd = IPERIOD, i, j, pd = PADEORD;
    FILE	*fp = stdin, *fpc = NULL;
    double	*c, *inc, *cc, *d, x;
    Boolean	ngain = NGAIN;
    
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
		case 'p':
		    fprd = atoi(*++argv);
		    --argc;
		    break;
		case 'i':
		    iprd = atoi(*++argv);
		    --argc;
		    break;
		case 'P':
		    pd = atoi(*++argv);
		    --argc;
		    break;
		case 'k':
		    ngain = 1 - ngain;
		    break;
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
		}
	}
	else if (fpc == NULL)
	    fpc = getfp(*argv, "r");
	else
	    fp = getfp(*argv, "r");

    if((pd < 4)||(pd > 5)){
	fprintf(stderr,"%s : Order of pade approximation is 4 or 5!\n",cmnd);
	exit(1);
    }

    if(fpc == NULL){
	fprintf(stderr,"%s : Cannot open cepstrum file!\n",cmnd);
	exit(1);
    }
	
    c = dgetmem(m+m+m+3+(m+1)*pd*2);
    cc  = c  + m + 1;
    inc = cc + m + 1;
    d   = inc+ m + 1;
    
    if(freadf(c, sizeof(*c), m+1, fpc) != m+1) exit(1);

    for(;;){
	if(freadf(cc, sizeof(*cc), m+1, fpc) != m+1) exit(0);
	
	for(i=0; i<=m; i++)
	    inc[i] = (cc[i] - c[i])*iprd / fprd;

	for(j=fprd, i=(iprd+1)/2; j--;){
	    if (freadf(&x, sizeof(x), 1, fp) != 1) exit(0);

	    if (!ngain) x *= exp(c[0]);
	    x = lmadf(x, c, m, pd, d);
	    
	    fwritef(&x, sizeof(x), 1, stdout);
			
	    if (!--i){
		for (i=0; i<=m; i++) c[i] += inc[i];
		i = iprd;
	    }
	}

	movem(cc, c, sizeof(*cc), m+1);
    }
    exit(0);
}

