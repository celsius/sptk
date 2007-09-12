/*
  ---------------------------------------------------------------  
            Speech Signal Processing Toolkit (SPTK)

                      SPTK Working Group                           
                                                                   
                  Department of Computer Science                   
                  Nagoya Institute of Technology                   
                               and                                 
   Interdisciplinary Graduate School of Science and Engineering    
                  Tokyo Institute of Technology                    
                                                                   
                     Copyright (c) 1984-2007                       
                       All Rights Reserved.                        
                                                                   
  Permission is hereby granted, free of charge, to use and         
  distribute this software and its documentation without           
  restriction, including without limitation the rights to use,     
  copy, modify, merge, publish, distribute, sublicense, and/or     
  sell copies of this work, and to permit persons to whom this     
  work is furnished to do so, subject to the following conditions: 
                                                                   
    1. The source code must retain the above copyright notice,     
       this list of conditions and the following disclaimer.       
                                                                   
    2. Any modifications to the source code must be clearly        
       marked as such.                                             
                                                                   
    3. Redistributions in binary form must reproduce the above     
       copyright notice, this list of conditions and the           
       following disclaimer in the documentation and/or other      
       materials provided with the distribution.  Otherwise, one   
       must contact the SPTK working group.                        
                                                                   
  NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSTITUTE OF TECHNOLOGY,   
  SPTK WORKING GROUP, AND THE CONTRIBUTORS TO THIS WORK DISCLAIM   
  ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL       
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT   
  SHALL NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSTITUTE OF         
  TECHNOLOGY, SPTK WORKING GROUP, NOR THE CONTRIBUTORS BE LIABLE   
  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY        
  DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,  
  WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTUOUS   
  ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR          
  PERFORMANCE OF THIS SOFTWARE.                                    
                                                                   
  ---------------------------------------------------------------  
*/

/* $Id$ */

#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#if defined(LINUX) || defined(FreeBSD)
#include <sys/soundcard.h>
#define AUDIO_DEV "/dev/dsp"
#define MIXER_DEV "/dev/mixer"
#define MAXAMPGAIN 100
#define AVAILABLE_FREQ "8,11.025,22.05,44.1"
#define DEFAULT_FREQ 11
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
#define AUDIO_DEV "/dev/audio"
#define AUDIO_CTLDEV "/dev/audioctl"
#define MAXAMPGAIN 255
#define AVAILABLE_FREQ "8,11.025,16,22.05,32,44.1,48"
#define DEFAULT_FREQ 16
#endif /* SPARC */

#define U_LAW 1
#define A_LAW 2
#define LINEAR 3

typedef struct _MENU {
   int value;
   unsigned int sample;
   unsigned int precision;
   unsigned int encoding;
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
   {12, 48000,  16, LINEAR}
};

#define _8000_8BIT_ULAW     1
#define _8000_8BIT_ALAW     2
#define _8000_16BIT_LINEAR  3
#define _9600_16BIT_LINEAR  4 
#define _11025_16BIT_LINEAR 5
#define _16000_16BIT_LINEAR 6
#define _18900_16BIT_LINEAR 7
#define _22050_16BIT_LINEAR 8
#define _32000_16BIT_LINEAR 9
#define _37800_16BIT_LINEAR 10
#define _44100_16BIT_LINEAR 11
#define _48000_16BIT_LINEAR 12

int ACFD;
int ADFD;
FILE *adfp;


#define SPEAKER   (0x01)
#define HEADPHONE (0x02)
#define LINE_OUT  (0x04)
#define MUTE      (0x08)

