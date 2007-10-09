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

/************************************************************************
*                                                                       *
*    Simple APIs for playing audio data in windows                      *
*                                                                       *
*                                            2007.10  Jong-Jin Kim      *
*                                                                       *
*    notice: just for PCM, MONO                                         *
*                                                                       *
*************************************************************************/   

static char *rcs_id = "$Id: winplay.c,v 1.1 2007/10/09 09:06:21 heigazen Exp $";

#include <windows.h>
#include <mmsystem.h>
#include "winplay.h"

#define WO_MAX_BUCKETS 8     /* number of buckets of the ring-buffer */
#define WO_BUCKET_SIZE 1024  /* bucket size */

HWAVEOUT wOutDev;
WAVEFORMATEX wOutFmt;
WAVEHDR wOutHdr[WO_MAX_BUCKETS];
short wOutBuf[WO_MAX_BUCKETS * WO_BUCKET_SIZE];

int wi;  /* writing index */
int ri;  /* reading index */

static DWORD CALLBACK win32_audio_callback(HWAVEOUT hwo, UINT uMsg, DWORD data, DWORD dwParam1, DWORD dwParam2)
{         
   if (MM_WOM_DONE == uMsg) {
      ri = (ri + 1) % WO_MAX_BUCKETS;
   }
   return 0;
}

int win32_audio_open (int sr, int sb)
{
   int i;
      
   wOutFmt.wFormatTag = 1;                /* PCM */
   wOutFmt.nChannels = 1;                 /* MONO */
   wOutFmt.nSamplesPerSec = sr;
   wOutFmt.nAvgBytesPerSec = sr * (sb/8); /* 16kHz * 16bits * Mono */
   wOutFmt.nBlockAlign = (sb/8);
   wOutFmt.wBitsPerSample = sb;
   wOutFmt.cbSize = 0;
   
   /* Does the specified audio format is supported by device? */
   if (MMSYSERR_NOERROR != waveOutOpen(NULL,                       /* NULL for query */
                                       WAVE_MAPPER,                /* device identifier */
                                       (LPWAVEFORMATEX)&(wOutFmt), /* requested format */
                                       0,                          /* no callback */
                                       0,                          /* no instance data */
                                       WAVE_FORMAT_QUERY)){        /* query only */
      return(WIN32AUDIO_FAILED);
   }
      
   /* Initialize waveout header & buffer */
   for (i=0; i<WO_MAX_BUCKETS; i++) {
      wOutHdr[i].lpData = (LPSTR)&(wOutBuf[i*WO_BUCKET_SIZE]);
      wOutHdr[i].dwBufferLength = WO_BUCKET_SIZE * sizeof(short);
      wOutHdr[i].dwBytesRecorded = 0L;
      wOutHdr[i].dwUser = 0;
      wOutHdr[i].dwFlags = 0L;
      wOutHdr[i].dwLoops = 1L;
      wOutHdr[i].lpNext = NULL;
      wOutHdr[i].reserved = 0L;
   }
   
   /* Open the waveout device */
   if (MMSYSERR_NOERROR != waveOutOpen(&(wOutDev),                  /* [out] waveout device handle */
                                       WAVE_MAPPER,                 /* provid by OS */
                                       &(wOutFmt),                  /* audio format to play */
                                       (DWORD)win32_audio_callback, /* callback function pointer */
                                       (DWORD)0,                    /* argument of the callback function */
                                        CALLBACK_FUNCTION)) {       /* done-message processing method */
      return(WIN32AUDIO_FAILED);
   }

   wi = 0;
   ri = 0;
   
   return(WIN32AUDIO_NO_ERROR);
}

int win32_audio_play (short *buffer, int buffer_len)
{
   int i, current_to_play, remained_to_play;
   short *des, *src;

   remained_to_play = buffer_len;

   while (remained_to_play > 0) {
      while (ri == ((wi+1) % WO_MAX_BUCKETS))
         Sleep(100); /* wait until there are available buckets */      
      
      if (WHDR_DONE == (wOutHdr[wi].dwFlags & WHDR_DONE))
         waveOutUnprepareHeader(wOutDev, &wOutHdr[wi], sizeof(WAVEHDR));
   
      current_to_play = (remained_to_play > WO_BUCKET_SIZE) ? WO_BUCKET_SIZE : remained_to_play;
      if (0 < current_to_play) {
         des = (short *)wOutHdr[wi].lpData;
         src = (short *)&(buffer[buffer_len - remained_to_play]);
         for (i=0; i<current_to_play; i++) des[i] = src[i];          
         wOutHdr[wi].dwBufferLength = current_to_play * sizeof(short);
         
         waveOutPrepareHeader (wOutDev, &wOutHdr[wi], sizeof(WAVEHDR));
         waveOutWrite (wOutDev, &wOutHdr[wi], sizeof(WAVEHDR));
         
         remained_to_play -= current_to_play;
         wi = (wi+1) % WO_MAX_BUCKETS;
      }
   }

   return(WIN32AUDIO_NO_ERROR);
}

void win32_audio_set_volume (int vol)
{
   DWORD val;
   
   val = ((unsigned short)vol << 16) + (unsigned short)vol;
   waveOutSetVolume(wOutDev, val);
}

void win32_audio_close (void)
{
   while (WAVERR_STILLPLAYING == waveOutClose(wOutDev))
      Sleep(10);
}

