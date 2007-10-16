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
*    Binary File Cut                                                    *
*                                                                       *
*               1990.3  K.Tokuda                                        *
*               1996.5  K.Koishida                                      *
*                                                                       *
*       usage:                                                          *
*               bcut [options] [infile] > stdout                        *
*       options:                                                        *
*               -s s     :  start number                        [0]     *
*               -e e     :  end number                          [EOF]   *
*               -l l     :  block length                        [1]     *
*               -n n     :  block order                         [l-1]   *
*               +type    :  data type                           [f]     *
*                               c (char)     s (short)                  *
*                               i (int)      l (long)                   *
*                               f (float)    d (double)                 *
*       infile:                                                         *
*               data sequence                                   [stdin] *
*       stdout:                                                         *
*               cut data sequence                                       *
*       note:                                                           *
*               When both -l and -n are specified,                      *
*               latter argument is adopted.                             *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id: bcut.c,v 1.17 2007/10/16 02:20:56 heigazen Exp $";


/*  Standard C Libraries  */
#include <stdio.h>

#ifdef HAVE_STRING_H
#  include <string.h>
#else
#  include <strings.h>
#  ifndef HAVE_STRRCHR
#     define strrchr rindex
#  endif
#endif

#include <stdlib.h>
#include <ctype.h>

#if defined(WIN32)
#  include "SPTK.h"
#else
#  include <SPTK.h>
#endif

/*  Default Values  */
#define START 0
#define END -1
#define LENG 1


/*  Command Name  */
char *cmnd;


void usage (int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - binary file cut\n",cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -s s  : start number [%d]\n",START);
   fprintf(stderr, "       -e e  : end number   [EOF]\n");
   fprintf(stderr, "       -l l  : block length [%d]\n",LENG);
   fprintf(stderr, "       -n n  : block order  [l-1]\n");
   fprintf(stderr, "       +type : data type    [f]\n");
   fprintf(stderr, "                c (char)      s (short)\n");
   fprintf(stderr, "                i (int)       l (long)\n");
   fprintf(stderr, "                f (float)     d (double)\n");
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       data sequence        [stdin]\n");
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       cut data sequence\n");
   fprintf(stderr, "  note:\n");
   fprintf(stderr, "       When both -l and -n are specified,\n");
   fprintf(stderr, "       latter argument is adopted.\n");
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
   size_t size=sizeof(float);
   int n=LENG;
   long start=START, end=END, ptr;
   FILE *fp=stdin;
   char *s, c;
   double x;
        
   if ((cmnd = strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;
   while (--argc)
      if (*(s = *++argv)=='-') {
         c = *++s;
         switch(c) {
         case 's':
            start = atol(*++argv);
            --argc;
            break;
         case 'e':
            end = atol(*++argv);
            --argc;
            break;
         case 'l':
            n = atol(*++argv);
            --argc;
            break;
         case 'n':
            n = atol(*++argv)+1;
            --argc;
            break;
         case 'h':
            usage(0);
         default:
            fprintf(stderr, "%s : Invalid option '%c'!\n", cmnd, *(*argv+1));
            usage(1);
         }
      }
      else if (*s=='+') {
         c = *++s;
         switch(c) {
         case 'c':
            size = sizeof(char);
            break;
         case 's':
            size = sizeof(short);
            break;
         case 'l':
            size = sizeof(long);
            break;
         case 'i':
            size = sizeof(int);
            break;
         case 'f':
            size = sizeof(float);
            break;
         case 'd':
            size = sizeof(double);
            break;
         default:
            fprintf(stderr, "%s : Invalid option '%c'!\n", cmnd, *(*argv+1));
            usage(1);
         }
      }
      else
         fp = getfp(*argv, "rb");
      
   ptr = start * n;
   while(ptr--)
      if (freadx(&x, size, 1, fp) != 1) return(0);
    
   ptr = (end - start + 1) * n;
   while (end==-1 || ptr--) {
      if (freadx(&x, size, 1, fp)!=1)
         break;
      fwritex(&x, size, 1, stdout);
   }

   return(0);
}
