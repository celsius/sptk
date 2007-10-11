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

/********************************************************************
    $Id: fileio.c,v 1.11 2007/10/11 07:36:13 heigazen Exp $

    File I/O Functions

    int freada  (p, bl, fp)
    int fwritex (ptr, size, nitems, fp)
    int freadx  (ptr, size, nitems, fp)
    int fwritef (ptr, size, nitems, fp)
    int freadf  (ptr, size, nitems, fp)

**********************************************************************/

#include <stdio.h>
#include <stdlib.h>

#if defined(WIN32)
#  include <fcntl.h>
#  include <io.h>
#  include "SPTK.h"
#else
#  include <SPTK.h>
#endif

#define LINEBUFSIZE 256

/* freada: read ascii */
int freada (double *p, const int bl, FILE *fp)
{
   int c;
   char buf[LINEBUFSIZE];

#if defined(WIN32)
   _setmode( _fileno(fp), _O_TEXT );
#endif

   c = 0;
   while (c<bl) {
      if (fgets(buf,LINEBUFSIZE,fp)==NULL) break;
      p[c] = atof(buf);
      c++;
   }
   return(c);
}

/* fritex: wrapper function for fwrite */
int fwritex (void *ptr, const size_t size, const int nitems, FILE *fp)
{
#if defined(WIN32)
   _setmode( _fileno(fp), _O_BINARY );
#endif
   return(fwrite(ptr, size, nitems, fp));
}

/* freadx: wrapper function for fread */
int freadx (void *ptr, const size_t size, const int nitems, FILE *fp)
{
#if defined(WIN32)
   _setmode( _fileno(fp), _O_BINARY );
#endif
   return(fread(ptr, size, nitems, fp));
}

/* --------------- double I/O compile --------------- */ 
#ifndef DOUBLE

static float *f;
static int items;

/* fwritef : convert double type data to float type and write */
int fwritef (double *ptr, const size_t size, const int nitems, FILE *fp)
{
   int i;
   if (items < nitems) {
      if (f != NULL)
         free(f);
      items = nitems;
      f = fgetmem(items);
   }
   for (i=0; i<nitems; i++)
      f[i] = ptr[i];

#if defined(WIN32)
   _setmode( _fileno(fp), _O_BINARY );
#endif
	
   return fwrite(f, sizeof(float), nitems, fp);
}

/* freadf : read float type data and convert to double type */
int freadf (double *ptr, const size_t size, const int nitems, FILE *fp)
{
   int i, n;
   if (items < nitems) {
      if (f != NULL)
         free(f);
      items = nitems;
      f = fgetmem(items);
   }

#if defined(WIN32)
   _setmode( _fileno(fp), _O_BINARY );
#endif

   n = fread(f, sizeof(float), nitems, fp);
   for (i=0; i<n; i++)
      ptr[i] = f[i];
   
   return n;
}

/* --------------- float I/O compile --------------- */
#else  /* DOUBLE */

/* fwritef : write float type data */
int fwritef (float *ptr, const size_t size, const int nitems, FILE *fp)
{
   return(fwritex(ptr, size, nitems, fp));
}

/* freadf : read float type data */
int freadf (float *ptr, const size_t size, const int nitems, FILE *fp)
{
   return(freadx(ptr, size, nitems, fp));
}

#endif	/* DOUBLE */
