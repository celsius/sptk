/************************************************************************
*									*
*    Find Minimum and Maximum Values					*
*									*
*					1988.7	T.Kobayashi		*
*					1996.1	N.Isshiki		*
*					1998.7	M.Tamura		*
*									*
*	usage:								*
*		minmax [ options ] [ infile ] > stdout			*
*	options:							*
*		-l l	 :  length of vector		[1]		*
*		-n n	 :  order of vector		[l-1]		*
*		-b b	 :  find n-best values		[1]		*
*		-d d	 :  output data number 		[FALSE]		*
*	infile:								*
*		data sequence (float)					*
*	stdout:								*
*		minimum & maximum(float)				*
*		n-best minimums & n-best maximum(float)			*
*				,when -n option is specified		*
*		minimum:datanumber,...	(ascii)				*
*		maximum:datanumber,...  (ascii)				*
*				,when -d option is specified		*
*									*
************************************************************************/


/* Standard C Libraries */
#include <stdio.h>
#include <SPTK.h>
#include <string.h>

typedef enum _Boolean {FA, TR} Boolean;
char *BOOL[] = {"FALSE", "TRUE"};


/* Defualt Values */
#define DIM	1
#define NBEST	1
#define FLT_MAX 3.4e+38
#define FLT_MIN -3.4e+38
#define OUTNUM	FA


/* Command Name */
char	*cmnd;


void usage(status)
int status;
{
	fprintf(stderr, "\n");
	fprintf(stderr, " %s - find minimum and maximum values\n", cmnd);
	fprintf(stderr, "\n");
	fprintf(stderr, "  usage:\n");
	fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
	fprintf(stderr, "  options:\n");
	fprintf(stderr, "       -l l  : length of vector   [%d]\n",DIM);
	fprintf(stderr, "       -n n  : order of vector    [l-1]\n");
	fprintf(stderr, "       -b b  : find n-best values [%d]\n",NBEST);
	fprintf(stderr, "       -d    : output data number [%s]\n",BOOL[OUTNUM]);
	fprintf(stderr, "       -h    : print this message\n");
	fprintf(stderr, "  infile:\n");
	fprintf(stderr, "       data sequence (float)      [stdin]\n");
	fprintf(stderr, "  stdout:\n");
	fprintf(stderr, "       minimum and maximum values (float)\n");
	fprintf(stderr, "       or \"values:datanumber,...\" (ascii)\n");
	fprintf(stderr, "       ,if -d option is specified\n");
	fprintf(stderr, "\n");
	exit(status);
}

main(argc,argv)
int	argc;
char	*argv[];
{
	FILE	*fp;
	char	*s, *infile = NULL, c;
	Boolean outnum = OUTNUM;
	int	dim = DIM,nbest = NBEST;

        if ((cmnd = strrchr(argv[0], '/')) == NULL)
		cmnd = argv[0];
        else
		cmnd++;
	while (--argc){
		if(*(s = *++argv) == '-') {
			c = *++s;
			if(*++s == '\0' && ((c == 'l') || (c == 'n') || ( c == 'b'))) {
				s = *++argv;
				--argc;
			}
			switch(c) {
			case 'l':
				dim = atoi(s);
				break;
			case 'n':
				dim = atoi(s)+1;
				break;
			case 'b':
				nbest = atoi(s);
				break;
			case 'd':
				outnum = 1 - outnum;
				break;
			case 'h':
				usage(0);
			default:
				fprintf(stderr,
					"%s: unknown option '%c'\n", cmnd, c);
				usage(1);
				break;
			}
		}
		else
			infile = s;
	}
	if(infile) {
		fp = getfp(infile, "r");
	}
	else
		fp = stdin;
	minmax(fp,dim,nbest, outnum);
	exit(0);
}

minmax(fp,dim,nbest, outnum)
FILE	*fp;
int	dim,nbest;
Boolean outnum;
{
	double	*s;
	register int	k, n, i, j;
	int **minpos,**maxpos,*nmaxpos,*nminpos,*tmp, t = 0;
	double	*min,*max;

	min = dgetmem(nbest,sizeof(double));
	max = dgetmem(nbest,sizeof(double));
	s = dgetmem(dim, sizeof(double));
	minpos = (int **)calloc(sizeof(*minpos),nbest);
	maxpos = (int **)calloc(sizeof(*maxpos),nbest);
	nminpos = ( int *)calloc(sizeof(*nminpos),nbest);
	nmaxpos = ( int *)calloc(sizeof(*nminpos),nbest);
	
	for(k = 0; !feof(fp); ) {
		if((n = freadf(s, sizeof(*s), dim, fp)) == 0)
			break;
		if(dim != 1 || k == 0){
			for(i=1;i<nbest;i++){
				min[i] = FLT_MAX;
				max[i] = FLT_MIN;
			}
			min[0]=s[0];
			max[0]=s[0];
			for(i=0;i<nbest;i++){
				nminpos[i] = 1;
				nmaxpos[i] = 1;
				free(minpos[i]);
				free(maxpos[i]);
				minpos[i] = (int *)calloc(sizeof(**minpos),1);
				maxpos[i] = (int *)calloc(sizeof(**maxpos),1);
				minpos[i][0] = -1;
				maxpos[i][0] = -1;
			}
			minpos[0][0] = 0;
			maxpos[0][0] = 0;
		}
		else {
			for(i=0;i<nbest;i++){
				if(s[0] == min[i]){
					tmp = (int *)calloc(sizeof(*tmp),nminpos[i]);
					movem(minpos[i],tmp,sizeof(*tmp),nminpos[i]);
					free(minpos[i]);
					minpos[i]=(int *)calloc(sizeof(**minpos),++nminpos[i]);
					movem(tmp,minpos[i],sizeof(**minpos),nminpos[i]-1);
					minpos[i][nminpos[i]-1] = t;	
					free(tmp);
					break;
				}
				if(s[0] < min[i]){
					free(minpos[nbest-1]);
					for(j=nbest-1;j>i;j--){
						min[j]=min[j-1];
						minpos[j] = minpos[j-1];
						nminpos[j] = nminpos[j-1];
					}
					min[i] = s[0];
					minpos[i] = (int *)calloc(sizeof(**minpos),1);
					minpos[i][0] = t;
					nminpos[i] = 1;
					break;
				}
			}
			for(i=0;i<nbest;i++){
				if(s[0] == max[i]){
					tmp = (int *)calloc(sizeof(*tmp),nmaxpos[i]);
					movem(maxpos[i],tmp,sizeof(*tmp),nmaxpos[i]);
					free(maxpos[i]);
					maxpos[i]=(int *)calloc(sizeof(**maxpos),++nmaxpos[i]);
					movem(tmp,maxpos[i],sizeof(**maxpos),nmaxpos[i]-1);
					maxpos[i][nmaxpos[i]-1]=t;	
					free(tmp);
					break;
				}
				if(s[0] > max[i]){
					free(maxpos[nbest-1]);
					for(j=nbest-1;j>i;j--){
						max[j]=max[j-1];
						maxpos[j] = maxpos[j-1];
						nmaxpos[j] = nmaxpos[j-1];
					}
					max[i] = s[0];
					maxpos[i] = (int *)calloc(sizeof(**minpos),1);
					maxpos[i][0] = t;
					nmaxpos[i] = 1;
					break;
				}
			}
		}
		for(k = 1; k < n; ++k) {
			for(i=0;i<nbest;i++){
				if(s[k] == min[i]){
					tmp = (int *)calloc(sizeof(*tmp),nminpos[i]);
					movem(minpos[i],tmp,sizeof(*tmp),nminpos[i]);
					free(minpos[i]);
					minpos[i]=(int *)calloc(sizeof(**minpos),++nminpos[i]);
					movem(tmp,minpos[i],sizeof(**minpos),nminpos[i]-1);
					minpos[i][nminpos[i]-1]=k;	
					free(tmp);
					break;
				}
				if(s[k] < min[i]){
					free(minpos[nbest-1]);
					for(j=nbest-1;j>i;j--){
						min[j]=min[j-1];
						minpos[j] = minpos[j-1];
						nminpos[j] = nminpos[j-1];
					}
					min[i] = s[k];
					minpos[i] = (int *)calloc(sizeof(**minpos),1);
					minpos[i][0] = k;
					nminpos[i] = 1;
					break;
				}
			}
			for(i=0;i<nbest;i++){
				if(s[k] == max[i]){
					tmp = (int *)calloc(sizeof(*tmp),nmaxpos[i]);
					movem(maxpos[i],tmp,sizeof(*tmp),nmaxpos[i]);
					free(maxpos[i]);
					maxpos[i]=(int *)calloc(sizeof(**maxpos),++nmaxpos[i]);
					movem(tmp,maxpos[i],sizeof(**maxpos),nmaxpos[i]-1);
					maxpos[i][nmaxpos[i]-1]=k;	
					free(tmp);
					break;
				}
				if(s[k] > max[i]){
					free(maxpos[nbest-1]);
					for(j=nbest-1;j>i;j--){
						max[j]=max[j-1];
						maxpos[j] = maxpos[j-1];
						nmaxpos[j] = nmaxpos[j-1];
					}
					max[i] = s[k];
					maxpos[i] = (int *)calloc(sizeof(**maxpos),1);
					maxpos[i][0] = k;
					nmaxpos[i] = 1;
					break;
				}
			}
		}
		if(dim != 1){
			if(outnum){
				for(i=0;i<nbest;i++){
					printf("%g:",min[i]);
					printf("%d",minpos[i][0]);
					for(j=1;j<nminpos[i];j++)
						printf(",%d",minpos[i][j]);
					printf("\n");
				}
				for(i=0;i<nbest;i++){
					printf("%g:",max[i]);
					printf("%d",maxpos[i][0]);
					for(j=1;j<nmaxpos[i];j++)
						printf(",%d",maxpos[i][j]);
					printf("\n");
				}
			}
			else{
				fwritef(min, sizeof(*min), nbest, stdout);
				fwritef(max, sizeof(*max), nbest, stdout);
			}
		}
		else
			t++;

	}
	if(dim == 1){
		if(outnum){
			for(i=0;i<nbest;i++){
				printf("%g:",min[i]);
				printf("%d",minpos[i][0]);
				for(j=1;j<nminpos[i];j++)
					printf(",%d",minpos[i][j]);
				printf("\n");
			}
			for(i=0;i<nbest;i++){
				printf("%g:",max[i]);
				printf("%d",maxpos[i][0]);
				for(j=1;j<nmaxpos[i];j++)
					printf(",%d",maxpos[i][j]);
				printf("\n");
			}
		}
		else{
			fwritef(min, sizeof(*min), nbest, stdout);
			fwritef(max, sizeof(*max), nbest, stdout);
		}
	}
	return(0);
}
