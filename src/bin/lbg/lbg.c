/************************************************************************
*									*
*    	LBG Algorithm for Vector Qauntizer Design			*
*									*
*					1996. 4  K.Koishida		*
*									*
*	usage:								*
*		lbg [ options ] [ indexfile ] < stdin > stdout		*
*	options:							*
*		-l l      :  length of vector	     	[26]		*
*		-n n      :  order of vector	     	[25]		*
*		-t t      :  number of training vector 	[N/A]		*
*		-s s  	  :  initial codebook size   	[1]		*
*		-e e  	  :  final codebook size     	[256]		*
*		-f f  	  :  initial codebook filename 	[NULL]		*
*		-v	  :  verbose mode 		[FALSE]		*
*		(level 2)						*
*		-d d	  :  end condition		[0.0001]	*
*		-r r	  :  splitting factor		[0.0001]	*
*	infile:								*
*		training vector (stdin)					*
*		    x(0), x(1), ... x(t*l-1)				*
*	stdout:								*
*		trained codebook					*
*		    cb'(0), cb'(1), ... cb(l*e-1)			*
*	ifile:	(if s > 1)						*
*		initial codebook (cbfile) 				*
*		    cb(0), cb(1), ... cb(l*s-1)				*
*	indexfile:							*
*		VQ index of training vector (int)			*
*		    index(0), index(1), ... index(t-1)			*
*	notice:								*
*		codebook size (s and e) must be power of 2		*
*		-t option can be omitted, when input from redirect	*
*	require:							*
*		vq(), edist(), vaverage()				*
*									*
************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <SPTK.h>
#include <stdlib.h>
#include <string.h>


typedef enum _Boolean {FA, TR} Boolean;
char *BOOL[] = {"FALSE", "TRUE"};


/*  Required Functions  */
int	vq();
void	vaverage();
double  edist();
int	nrand();


/*  Default Values  */
#define LENG		26
#define TNUMBER		-1
#define	ICBSIZE		1
#define	ECBSIZE		256
#define	VERBOSE		FA
#define	DELTA		0.0001
#define	END		0.0001

#define	MAXVALUE	1e23
#define abs(x)          ( (x < 0) ? (-(x)) : (x) )


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - LBG algorithm for vector quantizer design \n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ ifile ] < stdin > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -l l  : length of vector          [%d]\n", LENG);
    fprintf(stderr, "       -n n  : order of vector           [%d]\n", LENG-1);
    fprintf(stderr, "       -t t  : number of training vector [N/A]\n");
    fprintf(stderr, "       -s s  : initial codebook size     [%d]\n", ICBSIZE);
    fprintf(stderr, "       -e e  : final codebook size       [%d]\n", ECBSIZE);
    fprintf(stderr, "       -f f  : initial codebook filename [NULL]\n");
    fprintf(stderr, "       -v    : verbose mode              [%s]\n", BOOL[VERBOSE]);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "     (level 2)\n");
    fprintf(stderr, "       -d d  : end condition             [%g]\n", END);
    fprintf(stderr, "       -r r  : splitting factor          [%g]\n", DELTA);
    fprintf(stderr, "  stdin:\n");
    fprintf(stderr, "       data sequence (float)\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       codebook (float)\n");
    fprintf(stderr, "  ifile:\n");
    fprintf(stderr, "       index (int)\n");
    fprintf(stderr, "  notice:\n");
    fprintf(stderr, "       codebook size (s and e) must be power of 2\n");
    fprintf(stderr, "       -t option can be omitted, when input from redirect\n");
    fprintf(stderr, "\n");
    exit(status);
}


void main(int argc, char **argv)
{
    int		     l = LENG, icbsize = ICBSIZE, ecbsize = ECBSIZE, 
		     tnum = TNUMBER, ispipe, xsize, csize, maxindex, i, j, k,
		     *cntcb, *tindex;
    Boolean	     verbose = VERBOSE;
    FILE	     *fp = stdin, *fpi = NULL, *fpcb = NULL, *fpv;
    double	     delta = DELTA, minerr = END, 
		     *x, *cb, *cb1, *rnd, d0, d1, dl, err, atof();
    register double  *p, *q, *r;
    
    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc)
	if (**++argv == '-'){
	    switch (*(*argv+1)) {
		case 'l':
		    l = atoi(*++argv);
		    --argc;
		    break;
		case 'n':
		    l = atoi(*++argv)+1;
		    --argc;
		    break;
		case 't':
		    tnum = atoi(*++argv);
		    --argc;
		    break;
		case 's':
		    icbsize = atoi(*++argv);
		    --argc;
		    break;
		case 'e':
		    ecbsize = atoi(*++argv);
		    --argc;
		    break;
		case 'd':
		    minerr = atof(*++argv);
		    --argc;
		    break;
		case 'r':
		    delta = atof(*++argv);
		    --argc;
		    break;
		case 'f':
		    fpcb = getfp(*++argv, "r");
		    --argc;
		    break;
		case 'v':
		    verbose = 1 - verbose;
		    break;
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
		}
	}
	else 
	    fpi = getfp(*argv, "w");

    if(tnum == -1){
	ispipe = fseek(fp,0L,2);
#ifdef DOUBLE
	tnum = ftell(fp)/l/sizeof(double);
#else
	tnum = ftell(fp)/l/sizeof(float);
#endif
	rewind(fp);
	if(ispipe == -1){
	    fprintf(stderr,"%s: -t option must be specified, when input via pipe!\n",cmnd);
	    usage(1);
	}
    }
	
    xsize = tnum * l;
    csize = ecbsize * l;
    
    x = dgetmem(xsize+csize+csize+l);
    cb = x + xsize;
    cb1 = cb + csize;
    rnd = cb1 + csize;
    
    cntcb = (int *)dgetmem(ecbsize+tnum);
    tindex = cntcb + ecbsize;
    
    if(freadf(x, sizeof(*x), xsize, fp) != xsize){
	fprintf(stderr,"%s : Size error of training data !\n",cmnd);
	exit(1);
    }

    if(icbsize == 1)
	vaverage(x, l, tnum, cb);
    else if(freadf(cb, sizeof(*cb), icbsize*l, fpcb) != icbsize*l){
	fprintf(stderr,"%s : Size error of initial codebook !\n",cmnd);
	exit(1);
    }
    

    if(verbose)
	fpv = getfp("verbose.lbg", "w");
    
    for( ; icbsize*2 <= ecbsize; ){
	q = cb; r = cb + icbsize*l; 		/* splitting */
	for(i=0; i<icbsize; i++){
	    nrand(rnd, l, i);
	    for(j=0; j<l; j++){
		dl = delta * rnd[j];
		*r = *q - dl;
		*q = *q + dl;
		r++;
		q++;
	    }
	}
	icbsize *= 2;
	
	d0 = MAXVALUE;
	for( ;; ){
	    fillz(cntcb, sizeof(*cntcb), icbsize);
	    d1 = 0.0; p = x;
	    for(i=0; i<tnum; i++,p+=l){		/* clustering */
		tindex[i] = vq(p, cb, l, icbsize);
		cntcb[tindex[i]]++;

		q = cb + tindex[i] * l;
		d1 += edist(p, q, l);
	    }

	    d1 /= tnum;
	    err = abs((d0 - d1) / d1);
	    
	    if(verbose)
		fprintf(fpv,"size=%d error=%f d0=%f d1=%f\n",icbsize,err,d0,d1);
	    
	    if(err < minerr)  break;		/* check distortion */

	    d0 = d1;
	    fillz(cb1, sizeof(*cb), icbsize*l);
	    
	    p = x;				/* get new centroid */
	    for(i=0; i<tnum; i++){
		q = cb1 + tindex[i] * l;
		for(j=0; j<l; j++) *q++ += *p++;
	    }
	    
	    k = maxindex = 0;
	    for(i=0; i<icbsize; i++){
		if(cntcb[i] > k){
		    k = cntcb[i];
		    maxindex = i;
		}
	        if(verbose)
		    fprintf(fpv,"index=%3d  number=%7d\n",i,cntcb[i]);
	    }
	
	    
	    q = cb; r = cb1;
	    for(i=0; i<icbsize; i++,r+=l,q+=l)
		if(cntcb[i] > 0)
		    for(j=0; j<l; j++)
			q[j] = r[j] / (double) cntcb[i];
		else{
		    nrand(rnd, l, i);
		    p = cb + maxindex * l;
		    for(j=0; j<l; j++)
			q[j] = p[j] + delta * rnd[j];
		}
	}
	if(icbsize == ecbsize) break;
    }

    fwritef(cb, sizeof(*cb), csize, stdout);
    if(fpi != NULL)
	fwrite(tindex, sizeof(*tindex), tnum, fpi);
    exit(0);
}
