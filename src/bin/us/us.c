/************************************************************************
*                                                                       *
*    sampling rate conversion                                           *
*                                                                       *
*                                       1998.3  T.Kobayashi             *
*                                                                       *
*       usage:                                                          *
*               srcnv [ options ] infile  > stdout                      *
*       options:                                                        *
*               -s s  :  conversion type                          [58]  *
*               -c c  :  filename of low pass filter coef.   [Default]  *
*               -u u  :  upsampling ratio                          [1]  *
*               -d d  :  downsampling ratio                        [1]	*
*       infile:                                                         *
*               data sequence (float)                          [stdin]  *
*       notice:                                                         *
*                                                                       *
************************************************************************/

/* Standard C Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

/* Default Value */
#define	RBSIZE		512
#define	DSIZE		512
#define	SIZE		256
#define LINEBUFSIZE	256
#define STYPE		"58"
#define	mod(x)	((x) & (RBSIZE -1))

#ifndef LIB
	#define LIB "/usr/local/SPTK/lib"
#endif

#define COEF23F	LIB "/lpfcoef.2to3f"
#define COEF23S	LIB "/lpfcoef.2to3s"
#define COEF34	LIB "/lpfcoef.3to4"
#define COEF45	LIB "/lpfcoef.4to5"
#define COEF57	LIB "/lpfcoef.5to7"
#define COEF58	LIB "/lpfcoef.5to8"

/* Command Name */
char		*cmnd, *coef = NULL, *type = STYPE;

void usage(int status)
{
	fprintf(stderr, "\n");
	fprintf(stderr, " %s - sampling rate convert\n\n",cmnd);
	fprintf(stderr, "  usage:\n");
	fprintf(stderr, "       %s [ options ] infile1 > stdout\n", cmnd);
	fprintf(stderr, "  options:\n");
	fprintf(stderr, "       -s s  : conversion type                           [%s]\n",STYPE);
	fprintf(stderr, "                23F up sampling by 2:3\n");
	fprintf(stderr, "                23S up sampling by 2:3\n");
	fprintf(stderr, "                34  up sampling by 3:4\n");
	fprintf(stderr, "                45  up sampling by 4:5\n");
	fprintf(stderr, "                57  up sampling by 5:7\n");
	fprintf(stderr, "                58  up sampling by 5:8\n");
	fprintf(stderr, "       -c c  : filename of low pass filter coefficients  [Default]\n");
	fprintf(stderr, "       -u u  : upsampling ratio                          [N/A]\n");
	fprintf(stderr, "       -d d  : downsampling ratio                        [N/A]\n");
	fprintf(stderr, "       -h    : print this message\n");
	fprintf(stderr, "  infile:\n");
	fprintf(stderr, "       data                                              [stdin]\n");
	fprintf(stderr, "  notice:\n");
	fprintf(stderr, "       Default LPF coefficients File\n");
	fprintf(stderr, "        2:3 -> %s\n", COEF23F);
	fprintf(stderr, "        2:3 -> %s\n", COEF23S);
	fprintf(stderr, "        3:4 -> %s\n", COEF34);
	fprintf(stderr, "        4:5 -> %s\n", COEF45);
	fprintf(stderr, "        5:7 -> %s\n", COEF57);
	fprintf(stderr, "        5:8 -> %s\n", COEF58);
	fprintf(stderr, "\n");
	exit(status);
}

static int	start = -1, u_rate=0, d_rate=0;

main(argc,argv)
int	argc;
char	*argv[];
{
	FILE	*fp, *fopen();
	char	*s, *infile = NULL, c;
	int	def_d_rate, def_u_rate;

	if ((cmnd = strrchr(argv[0], '/')) == NULL)
		cmnd = argv[0];
	else
		cmnd++;

	while(--argc) {
		if(*(s = *++argv) == '-') {
			c = *++s;
			if(*++s == '\0') {
				s = *++argv;
				--argc;
			}
			switch(c) {
			case 'c':
				coef = s;
				type = NULL;
				break;
			case 's':
				type = s;
				break;
			case 'u':
				u_rate = atoi(s);
				break;
			case 'd':
				d_rate = atoi(s);
				break;
			case 'h':
				usage(0);
			default:
				fprintf(stderr, "%s: Invalid option '%c'\n", cmnd, (*argv+1));
				usage(1);
			}
		}
		else
			infile = s;
	}
	if( type == NULL){
		;
	}
	else if( strcmp( type, "23F") == 0){
		coef = COEF23F;
		def_d_rate = 2;
		def_u_rate = 3;
	}
	else if( strcmp(type, "23S") == 0){
		coef = COEF23S;
		def_d_rate = 2;
		def_u_rate = 3;
	}
	else if( strcmp(type, "34") == 0){
		coef = COEF34;
		def_d_rate = 3;
		def_u_rate = 4;
	}
	else if( strcmp(type, "45") == 0){
		coef = COEF45;
		def_d_rate = 4;
		def_u_rate = 5;
	}
	else if( strcmp(type, "57") == 0){
		coef = COEF57;
		def_d_rate = 5;
		def_u_rate = 7;
	}
	else if( strcmp(type, "58") == 0){
		coef = COEF58;
		def_d_rate = 5;
		def_u_rate = 8;
	}
	else{
		fprintf(stderr, "%s: Invalid option '%s'\n", cmnd, type);
		usage(1);
	}
	
	if( d_rate == 0) d_rate = def_d_rate;
	if( u_rate == 0) u_rate = def_u_rate;

	if(coef == NULL) {
		fprintf(stderr, "%s: LPF-coef file not specified\n", cmnd);
		exit(1);
	}
	if(infile) {
		if((fp = fopen(infile, "r")) == NULL) {
			fprintf(stderr, "%s: cannot open %s\n", cmnd, infile);
			exit(1);
		}
	}
	else {
		fp = stdin;
	}
	srcnv(fp);
	fclose(fp);

	return(0);
}

srcnv(fp)
FILE	*fp;
{
	int	is_cont, i, k, nread, count, nwr, delay;
	float	x[SIZE], y[DSIZE];
	void	firin(), firinit();
	double	firout();

	firinit();
	delay = start;
	for(count = is_cont = 1; is_cont; ) {
		if((nread = fread(x, sizeof(*x), SIZE, fp)) == 0) {
			is_cont = 0;
			nread = (d_rate * start) / u_rate;
			for(k = 0; k < nread; ++k)
				x[k] = 0;
		}
		for(nwr = k = 0; k < nread; ++k) {
			firin(x[k]);
			for(i = 0; i < u_rate; ++i) {
				if(--count == 0) {
					y[nwr++] = firout(i);
					count = d_rate;
				}
			}
		}
		if(delay) {
			if(nwr > delay) {
				nwr -= delay;
				if(fwrite(y + delay, sizeof(*y), nwr, stdout) != nwr){
					fprintf(stderr, "%s: write error\n", cmnd);
					exit(1);
				}
				delay = 0;
			} else {
				delay -= nwr;
			}
		} else {
			if(fwrite(y, sizeof(*y), nwr, stdout) != nwr){
				fprintf(stderr, "%s: write error\n", cmnd);
				exit(1);
			}
		}
	}
}

static float	rb[RBSIZE], hdn[RBSIZE + 1];
static int	flengdn = -1, indx = 0;

void firin(in)
double	in;
{
	indx = mod(indx - 1);
	rb[indx] = in;
}

double firout(os)
int	os;
{
	double	out;
	int	k, l;

	out = 0;
	for(k = os, l = indx ; k <= flengdn; k += u_rate, l = mod(l + 1))
		out += rb[l] * hdn[k];

	return(out);
}

void firinit()
{
	FILE	*fp, *fopen();

	if((fp = fopen(coef, "r")) == NULL) {
		fprintf(stderr, "%s: cannot open %s\n", cmnd, coef);
		exit(1);
	}
	flengdn = freada(hdn, RBSIZE + 1, fp);
	fclose(fp);
	if(--flengdn < 0) {
		fprintf(stderr, "%s: cannot read filter coefficients\n", cmnd);
		exit(1);
	}
	if(start) {
		start = flengdn / (2 * d_rate);
	}
}

int freada( p, bl, fp)
float *p;
int bl;
FILE *fp;
{
	int c;
	char buf[LINEBUFSIZE];

	c = 0;
	while( c < bl ){
		if( fgets( buf, LINEBUFSIZE, fp) == NULL) break;
		p[c] = (float)atof( buf);
		c+=1;
	}
	return c;
}
