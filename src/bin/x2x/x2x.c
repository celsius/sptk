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
*    Data Type Transformation						*
*									*
*					1985.12 K.Tokuda		*
*					1996.5  K.Koishida		*
*									*
*	usage:								*
*		x2x [options] [infile] > stdout				*
*	options:							*
*		-r       :  specify rounding off when a real number 	*
*			    is substituted for a integer	[FALSE]	*
*		+type1   :  input data type 			[f]	*
*		+type2   :  output data type 			[type1]	*
*				c (char)           s (short)		*
*				i (int)            l (long)		*
*				f (float)          d (double)		*
*				a (ascii)				*
*				C (unsigned char)  S (unsigned short)   *
*				I (unsigned int)   L (unsigned long)	*
*		+a a     :  column number 			[1]	* 
*		%format  :  specify output format similar to 	[FALSE]	*
*                           "printf()" of C function, 			*
*                           if type2 is ascii formant.			* 
*									*
************************************************************************/

static char *rcs_id = "$Id: x2x.c,v 1.8 2006/12/21 07:23:22 mr_alex Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <SPTK.h>


typedef enum _Boolean {FA, TR} Boolean;
char *BOOL[] = {"FALSE", "TRUE"};

/*  Default Values  */
#define ROUND		FA
#define COL		1
#define FORM_INT	"%d"
#define FORM_FLOAT	"%g"


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - data type transformation\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       +type1  : input data type                             [f]\n");
    fprintf(stderr, "       +type2  : output data type                            [type1]\n");
    fprintf(stderr, "                 c (char)           s (short)\n");
    fprintf(stderr, "                 i (int)            l (long)\n");
    fprintf(stderr, "                 f (float)          d (double)\n");
    fprintf(stderr, "                 a (ascii)\n");
    fprintf(stderr, "                 C (unsigned char)  S (unsigned short)\n");
    fprintf(stderr, "                 I (unsigned int)   L (unsigned long)\n");
    fprintf(stderr, "       +a a    : column number                               [%d]\n",COL);
    fprintf(stderr, "       -r      : specify rounding off when a real number\n");
    fprintf(stderr, "                 is substituted for a integer                [%s]\n",BOOL[ROUND]);
    fprintf(stderr, "       %%format : specify output format similar to 'printf()', \n");
    fprintf(stderr, "                 if type2 is ascii.                          [N/A]\n");
    fprintf(stderr, "       -h      : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       data sequence                                    [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       transformed data sequence\n");
#ifdef SPTK_VERSION
    fprintf(stderr, "\n");
    fprintf(stderr, " SPTK: version %s",SPTK_VERSION);
#endif
    fprintf(stderr, "\n");
    exit(status);
}

double r = 0.0;

int main(int argc, char **argv)
{
    char        c1, c2, *form = FORM_FLOAT;
    double      x;
    int         size1 = 0, size2 = 0, i = 1, col = COL, atoi();
    FILE	*fp = stdin;
    Boolean     round = ROUND;
    void        x2x();
    
    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc)
	if (**++argv == '+') {
	    (*argv)++;
	    while(**argv != '\0'){
		    switch (**argv) {
			case 's':
			    if(size1 == 0){
				c1 = 's';
				size1 = sizeof(short);
			    } else {
				c2 = 's';
				size2 = sizeof(short);
			    }
			    break;
			case 'S':
			    if(size1 == 0){
				c1 = 'S';
				size1 = sizeof(unsigned short);
			    } else {
				c2 = 'S';
				size2 = sizeof(unsigned short);
			    }
			    break;
			case 'i':
			    if(size1 == 0){
				c1 = 'i';
				size1 = sizeof(int);
			    } else {
				c2 = 'i';
				size2 = sizeof(int);
			    }
				break;
			case 'I':
			    if(size1 == 0){
				c1 = 'I';
				size1 = sizeof(unsigned int);
			    } else {
				c2 = 'I';
				size2 = sizeof(unsigned int);
			    }
				break;
			case 'l':
			    if(size1 == 0){
				c1 = 'l';
				size1 = sizeof(long);
			    } else {
				c2 = 'l';
				size2 = sizeof(long);
			    }
			    break;
			case 'L':
			    if(size1 == 0){
				c1 = 'L';
				size1 = sizeof(unsigned long);
			    } else {
				c2 = 'L';
				size2 = sizeof(unsigned long);
			    }
			    break;
			case 'f':
			    if(size1 == 0){
				c1 = 'f';
				size1 = sizeof(float);
			    } else {
				c2 = 'f';
				size2 = sizeof(float);
			    }
			    break;
			case 'd':
			    if(size1 == 0){
				c1 = 'd';
				size1 = sizeof(double);
			    } else {
				c2 = 'd';
				size2 = sizeof(double);
			    }
			    break;
			case 'c':
			    if(size1 == 0){
				c1 = 'c';
				size1 = sizeof(char);
			    } else {
				c2 = 'c';
				size2 = sizeof(char);
			    }
			    break;
			case 'C':
			    if(size1 == 0){
				c1 = 'C';
				size1 = sizeof(unsigned char);
			    } else {
				c2 = 'C';
				size2 = sizeof(unsigned char);
			    }
			    break;
			case 'a':
			    if(size1 == 0){
				c1 = 'a';
				size1 = -1;
			    } else {
				c2 = 'a';
				size2 = -1;
				if(*(argv+1) != '\0' && isdigit(**(argv+1))){
				    col = atoi(*++argv);
				    while( *(*argv+1) != '\0') (*argv)++;
				    argc--;
				}
				if( !((c1 == 'd') || (c1 == 'f'))){
					form = (char *)malloc( strlen( FORM_INT)+1);
					strcpy( form, FORM_INT);
				}
			    }
			    break;
			default:
			    fprintf(stderr, "%s : +Invalid option '%c' !\n", cmnd, *(*argv+1));
			    usage(1);
			}
		(*argv)++;
		}
	}
	else if(**argv == '-'){
	    switch (*(*argv+1)) {
                case 'r':
		    round = 1 - round;
		    break;
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : -Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
		}
	}
	else if(**argv == '%')
	    form = *argv;
    	else 
	    fp = getfp(*argv, "r");

    if(round)
	r = 0.5;

    if(size1 == 0){
	size1 = sizeof(float);
	c1 = 'f';
    }
    if(size2 == 0){
	size2 = size1;
	c2 = c1;
    }

    if (c1 == 'a'){
	if (c2 == 'a')
	    while (fscanf(fp, "%le", &x) != EOF){
		printf(form, x);
		if (i == col){
		    i = 1;
		    printf("\n");
		}else{
		    i++;
		    printf("\t");
		}
	    }
	else
	    while (fscanf(fp, "%le", &x) != EOF){
		x2x(&x, &x, 'd', c2);
		fwrite(&x, size2, 1, stdout);
	    }
    }
    else{
	if (c2 == 'a'){
	    while (fread(&x, size1, 1, fp) == 1){
		x2x(&x, &x, c1, 'd');
		switch( c1){
		   case 'd':
			printf( form, x);
			break;
		   case 'f':
			printf( form, x);
			break;
		   default:
			printf( form, (int)x);
		}
		if (i == col){
		    i = 1;
		    printf("\n");
		}else{
		    i++;
		    printf("\t");
		}
	    }
	}
	else
	    while (fread(&x, size1, 1, fp) == 1){
		x2x(&x, &x, c1, c2);
		fwrite(&x, size2, 1, stdout);
	    }
    }
    
    exit(0);
    
}

void x2x(char *x1,char *x2,char c1,char c2)
{
    double x;

    switch (c1){
        case 's' :
	    x = *(short *)x1;
	    break;
	case 'i' :
	    x = *(int *)x1;
	    break;
	case 'l' :
	    x = *(long *)x1;
	    break;
        case 'S' :
	    x = *(unsigned short *)x1;
	    break;
	case 'I' :
	    x = *(unsigned int *)x1;
	    break;
	case 'L' :
	    x = *(unsigned long *)x1;
	    break;
	case 'f' :
	    x = *(float *)x1;
	    break;
	case 'd' :
	    x = *(double *)x1;
	    break;
	case 'c' :
	    x = *(char *)x1;
	    break;
	case 'C' :
	    x = *(unsigned char *)x1;
	    break;
    }

    switch (c2){
        case 's' :
	    if(x > 0)
	      *(short *)x2 = x + r;
	    else
	      *(short *)x2 = x - r;
	    break;
	case 'i' :
	    if(x > 0)
	      *(int *)x2 = x + r;
	    else
	      *(int *)x2 = x - r;
            break;
	case 'l' :
	    if(x > 0)
	      *(long *)x2 = x + r;
	    else
	      *(long *)x2 = x - r;
	    break;
	case 'S' :
	    if(x > 0)
	      *(unsigned short *)x2 = x + r;
	    else
	      *(unsigned short *)x2 = x - r;
	    break;
	case 'I' :
	    if(x > 0)
	      *(unsigned int *)x2 = x + r;
	    else
	      *(unsigned int *)x2 = x - r;
	    break;
	case 'L' :
	    if(x > 0)
	      *(unsigned long *)x2 = x + r;
	    else
	      *(unsigned long *)x2 = x - r;
	    break;
	case 'f' :
	    *(float *)x2 = x;
	    break;
	case 'd' :
	    *(double *)x2 = x;
	    break;
	case 'c' :
	    if(x > 0)
	      *(char *)x2 = x + r;
	    else
	      *(char *)x2 = x - r;
	    break;
	case 'C' :
	    if(x > 0)
	      *(unsigned char *)x2 = x + r;
	    else
	      *(unsigned char *)x2 = x - r;
	    break;
    }
}

