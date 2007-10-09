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
*    Reverse the order of Data in Each Block                            *
*                                                                       *
*                                        1993.7 K.Tokuda                *
*                                        1996.1 N.miyazaki modified     *
*                                                                       *
*       usage:                                                          *
*               reverse [ options ] [ infile ] > stdout                 *
*       options:                                                        *
*               -l l  : length of block  [EOF]                          *
*               -n n  : order of block   [EOF-1]                        *
*       infile:                                                         *
*                       x0, x1, ..., xN, (real)                         *
*       stdout:                                                         *
*                       xN, ..., x1, x0, (real)                         *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(WIN32)
#include "SPTK.h"
#else
#include <SPTK.h>
#endif

/*  Default Values  */
#define MAX 32767 /* 0x7fff */
int l_max = MAX;

/*   Command Name  */
char* cmnd;


void usage (void)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - reverse the order of data in each block\n", cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -l l  : length of block       [EOF]\n");
   fprintf(stderr, "       -n n  : order of block        [EOF-1]\n");
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       data sequence (%s)      [stdin]\n", FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       reversed data sequence (%s)\n", FORMAT);
#ifdef PACKAGE_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n",PACKAGE_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(1);
}

int main (int argc,char *argv[])
{
   int l=-1;
   char *s, c;
   double *x;
   int max;
   FILE *fp=stdin;

   if ((cmnd=strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;

   while (--argc) {
      if (*(s=*++argv)=='-') {
         c = *++s;
         if (*++s=='\0') {
            s = *++argv;
            --argc;
         }
         switch (c) {
         case 'l':
            l = atoi(s);
            break;
         case 'n':
            l = atoi(s) + 1;
            break;
         case 'h':
            usage ();
         default :
            usage ();
         }
      }
      else
         fp = getfp(*argv, "rb");
   }

   if (l>MAX) {
      fprintf(stderr, "%s : -l L should be L <= %d!\n", cmnd, l_max);
      return(1);
   }

   max = (l<0) ? MAX : l;

   x = dgetmem(max);

   if (l<0) {
      max = freadf(x, sizeof(*x), max, fp);
      reverse(x, max);
      fwritef(x, sizeof(*x), max, stdout);
   }
   else {
      while (freadf(x, sizeof(*x), l, fp)==l) {
         reverse(x, l);
         fwritef(x, sizeof(*x), l, stdout);
      }
   }
   
   return(0);
}

