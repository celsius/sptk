/************************************************************************
*                                                                       *
*    play 16-bit linear PCM data on LINUX and SS10                      *
*                                                                       *
*					1997.7  M.Tamura		*
*                                       1998.1  T.Kobayashi             *
*                                                                       *
*       usage:                                                          *
*               da [ options ] infile1 infile2 ... > stdout             *
*       options:                                                        *
*               -s s  :  sampling frequency (8,10,12,16,20,22 kHz)[10]  *
*               -c c  :  filename of low pass filter coef.   [Default]  *
*               -g g  :  gain (.., -2, -1, 0, 1, 2, ..)            [0]  *
*               -a a  :  amplitude gain (0..100)                 [N/A]	*
*               -o o  :  output port                               [s]  *
*                          s (speaker)    h (headphone)                 *
*               -H H  :  header size in byte                       [0]  *
*               -v    :  display filename                      [FALSE]  *
*               +x    :  data format                               [s]  *
*                          s (short)      f (float)                     *
*       infile:                                                         *
*               data                                           [stdin]  *
*       notice:                                                         *
*               number of infile < 128                                  *
*               Default LPF coefficients filename                       *
*                               -> /usr/local/cmnd/lib/da_10.fir        *
*		12kHz LPF coefficients file				*
*				-> /usr/local/cmnd/lib/da_12.fir	*
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id$";

/* Standard C Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "da.h"

typedef enum _Boolean {FA, TR} Boolean;
char *BOOL[] = {"FALSE","TRUE"};


/* Default Value */
#define	DECRATE10	5
#define	DECRATE12	3

#define	INTRATE10	8
#define	INTRATE12	4

#define	SIZE		256*400
#define	RBSIZE		512
#define	MAXFILES	128
#define	INITGAIN	0

#define OUTPORT		's'
#define GAIN		(0+INITGAIN)
#define FREQ		10
#define HEADERSIZE	0
#define VERBOSE		FA

#ifndef LIB
	#define LIB "/usr/local/cmnd/lib"
#endif

#define COEF10  LIB "/da_10.fir"
#define COEF12  LIB "/da_12.fir"

#define	mod(x)	((x) & (RBSIZE -1))


/* Command Name */
char *cmnd;

void usage(int status)
{
	fprintf(stderr, "\n");
	fprintf(stderr, " %s - play 16-bit linear PCM data\n\n",cmnd);
	fprintf(stderr, "  usage:\n");
	fprintf(stderr, "       %s [ options ] infile1 infile2 ... > stdout\n", cmnd);
	fprintf(stderr, "  options:\n");
	fprintf(stderr, "       -s s  : sampling frequency (8,10,12,16,20,22,32,44,48 kHz) [%d]\n", FREQ);
	fprintf(stderr, "       -c c  : filename of low pass filter coefficients  [Default]\n");
	fprintf(stderr, "       -g g  : gain (..,-2,-1,0,1,2,..)                  [%d]\n",GAIN);
	fprintf(stderr, "       -a a  : amplitude gain (0..100)                   [N/A]\n");
#ifdef SPARC
	fprintf(stderr, "       -o o  : output port                               [%c]\n",OUTPORT);
	fprintf(stderr,	"                  s(speaker)    h(headphone)\n");
#endif /* SPARC */
	fprintf(stderr, "       -H H  : header size in byte                       [%d]\n",HEADERSIZE);
	fprintf(stderr, "       -v    : display filename                          [%s]\n",BOOL[VERBOSE]);
	fprintf(stderr, "       -w    : byteswap                                  [FALSE]\n");
	fprintf(stderr, "       +x    : data format                               [s]\n");
	fprintf(stderr, "                  s(short)    f(float)\n");
	fprintf(stderr, "       -h    : print this message\n");
	fprintf(stderr, "  infile:\n");
	fprintf(stderr, "       data                                              [stdin]\n");
	fprintf(stderr, "  notice:\n");
	fprintf(stderr, "       number of infile < %d\n",MAXFILES);
	fprintf(stderr, "       Default LPF coefficients File:%s\n",COEF10);
	fprintf(stderr, "       12kHz LPF coefficients File:%s\n",COEF12);
	fprintf(stderr, "\n");
	exit(status);
}

static char	*coef = NULL, outport = OUTPORT;
static short	*y = NULL, *xs;
static int	gain = GAIN, ampgain = -1, is_verbose = VERBOSE;
static int	hdr_size = HEADERSIZE, data_size = sizeof(short);
static int	freq = FREQ, intrate = INTRATE10, decrate = DECRATE10;
static int	fleng, indx = 0;
static float	*x, rb[RBSIZE], h[RBSIZE + 1], fgain = 1;
int		byteswap = 0;
size_t		abuf_size;

int main(argc,argv)
int	argc;
char	*argv[];
{
	FILE	*fp, *fopen();
	char	*s, *infile[MAXFILES], c, *getenv();
	int	i, nfiles = 0;
	void	firinit(), sndinit(), convert(), direct();
	double	atof();

	if((s = getenv("DA_FLOAT")) != NULL)
		data_size = sizeof(float);
	if((s = getenv("DA_SMPLFREQ")) != NULL)
		freq = atoi(s);
	if((s = getenv("DA_GAIN")) != NULL)
		gain = atoi(s) + INITGAIN;
	if((s = getenv("DA_AMPGAIN")) != NULL)
		ampgain = atoi(s);
	if((s = getenv("DA_PORT")) != NULL)
		outport = *s;
	if((s = getenv("DA_HDRSIZE")) != NULL)
		hdr_size = atoi(s);

	if (( cmnd = strrchr(argv[0], '/')) == NULL)
	    cmnd = argv[0];
	else
	    cmnd++;

	while(--argc)
	    if(*(s = *++argv) == '-') {
		c = *++s;
		switch(c) {
		    case 'c':
			coef = *++argv;
			--argc;
			break;
		    case 's':
			freq = atoi(*++argv);
			--argc;
			break;
	  	    case 'g':
			gain = atoi(*++argv) + INITGAIN;
			--argc;
			break;
		    case 'a':
			ampgain = atoi(*++argv);
			--argc;
			break;
	 	    case 'H':
			hdr_size = atoi(*++argv);
			--argc;
			break;
		    case 'v':
			is_verbose = 1 - is_verbose;
			break;
		    case 'w':
			byteswap = 1;
			break;
		    case 'o':
			outport = **++argv;
			--argc;
			break;
		    case 'h':
			usage(0);
		    default:
			fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
			usage(1);
		}
	    } else if ( *s == '+') {
		c = *++s;
		switch(c) {
		    case 's':
			data_size = sizeof(short);
			break;
		    case 'f':
			data_size = sizeof(float);
			break;
		    default:
			fprintf(stderr, "%s : Invalid option '%c' !\n",cmnd, *(*argv+1));
			usage(1);
		}
	    }
	    else{
		if (nfiles < MAXFILES)
		    infile[nfiles++] = s;
		else{
	  	    fprintf(stderr, "%s: Number of files exceed %d\n", cmnd, MAXFILES);
		    exit(1);
		}
	}

	if((x = (float *)calloc(SIZE, sizeof(float))) == NULL) {
		fprintf(stderr, "%s: cannot allocate memory\n", cmnd);
		exit(1);
	}
	xs = (short *)x;
	if((y = (short *)calloc(SIZE*2, sizeof(float))) == NULL) {
		fprintf(stderr, "%s: cannot allocate memory\n", cmnd);
		exit(1);
	}

		sndinit();
		i = (gain < 0) ? -gain : gain;
		while(i--)
			fgain *= 2;
		if(gain < 0)
			fgain = 1 / fgain;
		if(freq == 10 || freq == 12 || freq == 20)
			firinit();

	if (nfiles) {
		for(i = 0; i < nfiles; ++i) {
			if((fp = fopen(infile[i], "r")) == NULL) {
				fprintf(stderr, "%s: cannot open %s\n", cmnd, infile[i]);
			} else {
				if(is_verbose) {
					fprintf(stderr, "%s: %s\n", cmnd, infile[i]);
				}
				if(freq == 10 || freq == 20)
					convert(fp);
				else if(freq == 12) {
					intrate = INTRATE12;
					decrate = DECRATE12;
					convert(fp);
				}
				else
					direct(fp);
				fclose(fp);
			}
		}
	}
	else {
		if(freq == 10 || freq == 20)
			convert(stdin);
		else if(freq == 12) {
			intrate = INTRATE12;
			decrate = DECRATE12;
			convert(stdin);
		}
		else
			direct(stdin);
	}
	fclose( adfp);
	close( ACFD);
	exit(0);
}

void convert(fp)
FILE	*fp;
{
	int	i, k, nread, count, nwr, firout();
	void	sndout();

	if(hdr_size) fseek(fp, (long)hdr_size, 0);

	for(count = 1; nread = fread(x, data_size, SIZE, fp); ) {
		nwr = 0;
		if( byteswap == 1) byteswap_vec( x, data_size, nread);
		for(k = 0; k < nread; ++k) {
			for(i = 0; i < intrate; ++i) {
				if(i == 0) {
					indx = mod(indx - 1);
					if(data_size == sizeof(float))
						rb[indx] = x[k];
					else
						rb[indx] = *(xs + k);
				}
				if(--count == 0) {
					y[nwr++] = firout(i);
					count = decrate;
				}
			}
		}
		sndout(nwr);
	}

	for(nwr = 0, k = fleng / 2; k--; ) {
		for(i = 0; i < intrate; ++i) {
			if(i == 0) {
				indx = mod(indx - 1);
				rb[indx] = 0;
			}
			if(--count == 0) {
				y[nwr++] = firout(i);
				count = decrate;
			}
		}
	}
	sndout(nwr);
}

void direct(fp)
FILE	*fp;
{
	int	k, nread;
	double	d;
	void	sndout();

	if(hdr_size) fseek(fp, (long)hdr_size, 0);

	while(nread = fread(x, data_size, SIZE, fp)) {
		for(k = 0; k < nread; ++k) {
			if(data_size == sizeof(float))
				d = x[k];
			else
				d = *(xs + k);
			y[k] = d * fgain;
		}
		sndout(nread);
	}
}

int firout(os)
int	os;
{
	double	out;
	int	k, l;

	out = 0;
	for(k = os, l = indx ; k <= fleng; k += intrate, l = mod(l + 1))
		out += rb[l] * h[k];
	if(out < 0)
		out -= 0.5;
	else
		out += 0.5;
	return((int) out);
}

void firinit()
{
	FILE	*fp, *fopen();
	int	i;

	if(coef == NULL) {
		if(freq == 12)
			coef = COEF12;
		else
			coef = COEF10;
	}

	if((fp = fopen(coef, "r")) == NULL) {
		fprintf(stderr, "%s: cannot open %s\n", cmnd, coef);
		exit(1);
	}
	fleng = freada(h, RBSIZE + 1, fp);
	fclose(fp);
	if(--fleng < 0) {
		fprintf(stderr, "%s: cannot read filter coefficients\n", cmnd);
		exit(1);
	}

	for(i = 0; i <= fleng; ++i)
		h[i] *= fgain;
}


void sndinit()
{
	int	port, dtype;
	void	init_audiodev(), change_play_gain(), change_output_port();

	switch(freq) {	
	case 8:
		dtype =_8000_16BIT_LINEAR;
		break;
	case 10:
	case 12:
	case 16:
		dtype =_16000_16BIT_LINEAR;
		break;
	case 20:
		dtype =_32000_16BIT_LINEAR;
		break;
	case 22:
		dtype =_22050_16BIT_LINEAR;
		break;
	case 32:
		dtype =_32000_16BIT_LINEAR;
		break;
	case 44:
		dtype =_44100_16BIT_LINEAR;
		break;
	case 48:
		dtype =_48000_16BIT_LINEAR;
		break;
	default:
		fprintf(stderr,"%s: unavailable sampling frequency\n", cmnd);
		exit(1);
	}
	init_audiodev(dtype);

	if(ampgain >= 0)
		if( ampgain > 100) ampgain = 100;
		change_play_gain(ampgain);
#ifdef SPARC
	if(outport == 's')
		port = SPEAKER;
	else if(outport == 'h')
		port = HEADPHONE;
	change_output_port( port | LINE_OUT );
#endif /* SPARC */
}

void sndout(leng)
int	leng;
{
	fwrite( y, sizeof(short), leng, adfp);
	write( ADFD, y, 0);
}

void init_audiodev(dtype)
int	dtype;
{
#ifdef LINUX
	int arg;

	adfp = fopen( AUDIO_DEV, "w");
	ADFD = adfp->_fileno;
	ACFD = open( MIXER_DEV, O_RDWR, 0);
	ioctl(ADFD, SNDCTL_DSP_GETBLKSIZE, &abuf_size);
	arg = data_type[dtype].precision;
	ioctl(ADFD, SOUND_PCM_WRITE_BITS, &arg);
/*	arg = data_type[dtype].channel; */
	arg = 1;
	ioctl(ADFD, SOUND_PCM_WRITE_CHANNELS, &arg);
	arg = data_type[dtype].sample;
	ioctl(ADFD, SOUND_PCM_WRITE_RATE, &arg);
#endif /* LINUX */

#ifdef SPARC
	audio_info_t	data;

	ACFD = open(AUDIO_CTLDEV, O_RDWR, 0);
	adfp = fopen(AUDIO_DEV, "w");
	ADFD = adfp->_file;

	AUDIO_INITINFO(&data);
	ioctl(ACFD, AUDIO_GETINFO, &data);

	data.play.sample_rate = data_type[dtype].sample;
	data.play.precision   = data_type[dtype].precision;
	data.play.encoding    = data_type[dtype].encoding;

	ioctl(ADFD,AUDIO_SETINFO,&data);
#endif /* SPARC */
}

void change_output_port(port)
unsigned port;
{
#ifdef LINUX
	
#endif /* LINUX */

#ifdef SPARC
	audio_info_t	data;

	AUDIO_INITINFO(&data);
	ioctl(ACFD, AUDIO_GETINFO, &data);

	data.play.port=port;	
	
	ioctl(ACFD, AUDIO_SETINFO, &data);
#endif /* SPARC */
}

void change_play_gain(volume)
unsigned volume;
{
	int vol, arg;

#ifdef LINUX
	vol = (int) (volume * ((float)MAXAMPGAIN) / 100);
	arg = vol | (vol << 8 );
	ioctl( ACFD, MIXER_WRITE(SOUND_MIXER_PCM), &arg);
#endif /* LINUX */

#ifdef SPARC
	audio_info_t	data;

	vol = (int) (volume * ((float)MAXAMPGAIN) / 100);
	AUDIO_INITINFO(&data);
	ioctl(ACFD, AUDIO_GETINFO, &data);

	data.play.gain=vol;

	ioctl(ACFD, AUDIO_SETINFO, &data);
#endif /* SPARC */
}

int byteswap_vec( vec, size, blocks)
void *vec;
int size;
int blocks;
{
	char *q;
	register char t;
	int i, j;

	q = (char *)vec;
	for( i = 0; i < blocks; i++){
		for( j = 0; j < (size/2); j++){
			t = *(q+j);
			*(q+j) = *(q+(size-1-j));
			*(q+(size-1-j)) = t;
		}
		q += size;
	}

	return i;		/* number of blocks */
}

int freada( p, bl, fp)
float *p;
int bl;
FILE *fp;
{
	int c;
	char buf[256];

	c = 0;
	while( c < bl ){
		if( fgets( buf, 256, fp) == NULL) break;
		p[c] = (float)atof( buf);
		c+=1;
	}
	return c;
}
