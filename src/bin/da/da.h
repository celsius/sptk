#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#if defined(LINUX) || defined(FreeBSD)
#include <sys/soundcard.h>
#define AUDIO_DEV    "/dev/dsp"
#define MIXER_DEV    "/dev/mixer"
#define	MAXAMPGAIN	100
#define AVAILABLE_FREQ "8,11.025,22.05,44.1"
#define DEFAULT_FREQ	11
#endif /* LINUX or FreeBSD */

#ifdef SUNOS
#define SPARC
#include <sun/audioio.h>
#endif /* SUNOS */

#ifdef SOLARIS
#define SPARC
#include <sys/audioio.h>
#endif /* SOLARIS */

#ifdef SPARC
#define AUDIO_DEV    "/dev/audio"
#define AUDIO_CTLDEV "/dev/audioctl"
#define	MAXAMPGAIN	255
#define AVAILABLE_FREQ "8,11.025,16,22.05,32,44.1,48"
#define DEFAULT_FREQ	16
#endif /* SPARC */

#define U_LAW 1
#define	A_LAW 2
#define	LINEAR 3

typedef struct _MENU {
        int      value;
        unsigned sample;
        unsigned precision;
        unsigned encoding;
} MENU;

static MENU data_type [] = {
  { 0, 0,       0, 0},
  { 1, 8000,    8, U_LAW},
  { 2, 8000,    8, A_LAW},
  { 3, 8000,   16, LINEAR},
  { 4, 9600,   16, LINEAR},
  { 5, 11025,  16, LINEAR},
  { 6, 16000,  16, LINEAR},
  { 7, 18900,  16, LINEAR},
  { 8, 22050,  16, LINEAR},
  { 9, 32000,  16, LINEAR},
  {10, 37800,  16, LINEAR},
  {11, 44100,  16, LINEAR},
  {12, 48000,  16, LINEAR},
  NULL,
};

#define _8000_8BIT_ULAW      1
#define _8000_8BIT_ALAW      2
#define _8000_16BIT_LINEAR   3
#define _9600_16BIT_LINEAR   4 
#define _11025_16BIT_LINEAR  5
#define _16000_16BIT_LINEAR  6
#define _18900_16BIT_LINEAR  7
#define _22050_16BIT_LINEAR  8
#define _32000_16BIT_LINEAR  9
#define _37800_16BIT_LINEAR  10
#define _44100_16BIT_LINEAR  11
#define _48000_16BIT_LINEAR  12

int	ACFD;
int	ADFD;
FILE	*adfp;


#define SPEAKER	  (0x01)
#define HEADPHONE (0x02)
#define LINE_OUT  (0x04)
#define MUTE	  (0x08)

