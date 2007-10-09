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

/* $Id$ */

#ifndef __WINPLAY_H__
#define __WINPLAY_H__

#ifdef __cplusplus
extern "C" {
#endif

#define WIN32AUDIO_NO_ERROR 0
#define WIN32AUDIO_FAILED -1

/*
   open audio device

   @param sr  [in] sampling frequency(Hz)
   @param sb  [in] sampling bits(bits)
   @return int
            WIN32AUDIO_FAILED    failed to open device
            WIN32AUDIO_NO_ERROR  ok
*/
int win32_audio_open (int sr, int sb);

/*
   play audio data
   
   @param [in] buffer audio data buffer pointer to play
   @param [in] buffer_len number of audio samples to play
   @return int
            WIN32AUDIO_FAILED    failed to open device
            WIN32AUDIO_NO_ERROR  ok
*/
int win32_audio_play (short *buffer, int buffer_len); 

/*
   set volume
   
   @param [in] vol input volume value (0 ~ 65535)
   @return none
*/
void win32_audio_set_volume (int vol);

/*
   close audio device
   
   @param none
   @return none
*/   
void win32_audio_close (void);

#ifdef __cplusplus
}
#endif

#endif /* __WINPLAY_H__ */