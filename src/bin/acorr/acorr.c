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
*    Obtain Autocorrelation Sequence                                    *
*                                                                       *
*       usage:                                                          *
*               acorr [ options ] [ infile ] > stdout                   *
*       options:                                                        *
*               -m m     :  order of sequence           [25]            *
*               -l l     :  frame length                [256]           *
*       infile:                                                         *
*               stdin for default                                       *
*               input is assumed to be real                             *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id$";


/* Standard C Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(WIN32)
#include "SPTK.h"
#else
#include <SPTK.h>
#endif

/* Default Values */
#define LENG   256
#define ORDER   25

/* Command Name  */
char   *cmnd;


void usage (int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - obtain autocorrelation sequence\n", cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -m m  : order of sequence  [%d]\n", ORDER);
   fprintf(stderr, "       -l l  : frame length       [%d]\n", LENG);
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       data sequence (%s)      [stdin]\n", FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       autocorrelation sequence (%s)\n", FORMAT);
#ifdef PACKAGE_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n",PACKAGE_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif      
   fprintf(stderr, "\n");
   exit(status);
}


int main (int argc, char **argv)
{
   FILE *fp=stdin;
   char *s, *infile=NULL, c;
   double *x, *r;
   int l=LENG, np=ORDER;

   if ((cmnd = strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;

   while (--argc) {
      if (*(s = *++argv)=='-') {
         c = *++s;
         if (*++s == '\0') {
            s = *++argv;
            --argc;
         }
         switch(c) {
         case 'm':
            np = atoi(s);
            break;
         case 'l':
            l = atoi(s);
            break;
         case 'h':
            usage(0);
         default:
            fprintf(stderr,"%s : Invalid option '%c'!\n", cmnd, c);
            usage(1);
         break;
         }
      }
      else
         infile = s;
   }
   if (infile)
      fp = getfp(infile,"r");
  
   x = dgetmem(l + np + 1);
   r = x + l;
   while (freadf(x, sizeof(*x), l, fp)==l) {
      acorr(x,l,r,np);
      fwritef(r, sizeof(*r), np + 1, stdout);
   }
   return(0);   
}
