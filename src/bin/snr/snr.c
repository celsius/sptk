/************************************************************************
*									*
*    SNR, SNR_seg							*
*									*
*					1990.11 T.Kanno			*
*					1996.3  K.Koishida		*
*									*
*	usage:								*
*		snr [ options ] file1 [ file2 ] > stdout		*
*	options:							*
*		-l l     :  frame length	 		 [256]	*
*		+ab      :  input data type (a: file1, b: file2) [sf]	*
*                               s : short   				*
*				f : float				*
*		-o o     :  output format			 [0]	*
*	infile:								*
*		data sequence						*
*		    , x(0), x(1), ...,					*
*		    , y(0), y(1), ...,					*
*	stdout:								*
*		output format 	    output 				*
*                     0             SNR and SNRseg           		*
*                     1             SNR and SNRseg in detail 		*
*		      2             SNR                      		*
*                     3             SNRseg                              *
*									*
************************************************************************/

static char *rcs_id = "$Id: snr.c,v 1.1 2000/03/01 13:58:50 yossie Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


/*  Required Functions  */
double	log10();


/*  Default Values  */
#define LENG		256
#define OTYPE		0


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - SNR and SNR_seg\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] file1 [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "      -l l  : frame length                         [%d]\n", LENG);
    fprintf(stderr, "      +ab   : input data type (a: file1, b: file2) [sf]\n",LENG);
    fprintf(stderr, "                s (short)  f (float) \n");
    fprintf(stderr, "      -o o  : output type                          [%d]\n", OTYPE);
    fprintf(stderr, "                0 (SNR and SNRseg)\n");
    fprintf(stderr, "                1 (SNR and SNRseg in datail)\n");
    fprintf(stderr, "                2 (SNR)\n");
    fprintf(stderr, "                3 (SNRseg)\n");
    fprintf(stderr, "      -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "      data sequence                                [stdin]\n");
    fprintf(stderr, "  file1:\n");
    fprintf(stderr, "      data sequence\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "      SNR, SNRseg\n");
    fprintf(stderr, "\n");
    exit(status);
}

void main(int argc, char **argv)
{
    int		l = LENG, num = 0, f1w = 1, f2w = 0, otype = OTYPE, 
                count, countold, countseg, xn, yn, size, i;
    FILE	*fp2 = stdin, *fp1 = NULL;
    double	*x, *y, snr, snrseg, pw1, pw2, pw1snr, pw2snr, sub;
    short	*xs, *ys;
    char        *s, c, c1, c2;

    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc)
	if (*(s = *++argv) == '-') {
            c = *++s;
	    switch (c) {
		case 'l':
		    l = atoi(*++argv);
		    --argc;
		    break;
		case 'o':
		    otype = atoi(*++argv);
		    --argc;
		    break;
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
		}
	}
	else if (*s == '+'){
	    c1 = *++s;
	    c2 = *++s;
	    switch(c1){
		case 'f':
		    f1w = 0;
		    if(c2 == 's') f2w = 1;
		    else	  f2w = 0;
		    break;
		case 's':
		    f1w = 1;
		    if(c2 == 's') f2w = 1;
		    else	  f2w = 0;
		    break;
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
		}
	}
        else if (fp1 == NULL)
            fp1 = getfp(*argv, "r");
        else
            fp2 = getfp(*argv, "r");

    x = dgetmem(l+l);
    y = x + l;
    xs = sgetmem(l+l);
    ys = xs + l;

    pw1 = pw2 = snrseg = 0.0;
    count = countold = countseg = 0;

    if(otype == 1)
	printf("\n");

    while(! feof(fp1) && ! feof(fp2)){
	if(f1w == 0)
	    xn = freadf(x, sizeof(*x), l, fp1);
	else{
	    xn = fread(xs, sizeof(*xs), l, fp1);
	    for(i=0; i<xn; i++)
		x[i] = (double) xs[i];
	}
	if(f2w == 0)
	    yn = freadf(y, sizeof(*y), l, fp2);
	else{
	    yn = fread(ys, sizeof(*ys), l, fp2);
	    for(i=0; i<yn; i++)
		y[i] = (double) ys[i];
	}

	size = (xn > yn) ? yn : xn;
	pw1snr = pw2snr = 0.0;

	for(i=0; i<size; i++){
	    pw1snr += x[i] * x[i];
	    sub = x[i] - y[i];
	    pw2snr += sub * sub;
	    
	}

	count += size;
	
	if(size){
	    snr = 10.0 * log10(pw1snr/pw2snr);
	    if(otype == 1)
		printf("\t %8d   ~ %8d    : %f [dB]\n",countold, count, snr);
	}

	if(size == l){
	    countseg++;
	    snrseg += snr;
	}

	pw1 += pw1snr;
	pw2 += pw2snr;
	countold = count;
    }
    snr = 10.0 * log10(pw1/pw2);
    snrseg /= (double) countseg;

    if(otype == 0 || otype == 1){
	printf("\n");
	printf("%d samples\n",count);
	printf("  SNR      :  %f [dB]\n",snr);
	printf("  SNRseg   :  %f [dB]\n",snrseg);
	printf("\n");
    }
    
    if(otype == 2)
	fwritef(&snr, sizeof(snr), 1, stdout);
    if(otype == 3)
	fwritef(&snrseg, sizeof(snrseg), 1, stdout);

    exit(0);
}

