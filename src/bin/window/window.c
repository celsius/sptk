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
*    Data Windowing                                                     *
*                                                                       *
*                                      1996.1  N.Miyazaki               *
*                                      1998.11 T.Masuko                 *
*                                                                       *
*       usage:                                                          *
*               window [ options ] [ infile ] > outfile                 *
*       options:                                                        *
*               -l l  :  frame length of input    [256]                 *
*               -L L  :  frane length of output   [l]                   *
*               -n n  :  type of normalization    [1]                   *
*                          0 none                                       *
*                          1 normalize by power                         *
*                          2 normalize by magnitude                     *
*               -w w  :  type of window           [0]                   *
*                          0 (blackman)                                 *
*                          1 (hamming)                                  *
*                          2 (hanning)                                  *
*                          3 (bartlett)                                 *
*                          4 (trapezoid)                                *
*                          5 (rectangul)                                *
*       infile:                                                         *
*               stdin for default                                       *
*               input is assumed to be double                           *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SPTK.h>


/*  Default Values */
#define FLENG  256
#define WINTYPE  BLACKMAN
#define NORMFLG  1

/*  Command Name  */
char* cmnd;


int usage (void)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - data windowing\n", cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > outfile\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -l l  : frame length of input  [%d]\n", FLENG);
   fprintf(stderr, "       -L L  : frame length of output [l]\n");
   fprintf(stderr, "       -n n  : type of normalization  [%d]\n", NORMFLG);
   fprintf(stderr, "                 0 none\n");
   fprintf(stderr, "                 1 normalize by power\n");
   fprintf(stderr, "                 2 normalize by magnitude\n");
   fprintf(stderr, "       -w w  : type of window         [%d]\n", WINTYPE);
   fprintf(stderr, "                 0 (blackman)\n");
   fprintf(stderr, "                 1 (hamming)\n");
   fprintf(stderr, "                 2 (hanning)\n");
   fprintf(stderr, "                 3 (bartlett)\n");
   fprintf(stderr, "                 4 (trapezoid)\n");
   fprintf(stderr, "                 5 (rectangular)\n");
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       data sequence (%s)          [stdin]\n", FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       windowed data sequence (%s)\n", FORMAT);
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
   FILE *fp=stdin;
   char *s, c;
   int fleng=FLENG, outl=-1, normflg=NORMFLG;
   Window wintype=WINTYPE;
   double  *x;

   if ((cmnd=strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;

   while (--argc) {
      if (*(s=*++argv)=='-') {
         c = *++s;
         if (*++s=='\0' && (c=='n' || c=='l' || c=='w' || c=='L')) {
            s = *++argv;
            --argc;
         }
         switch (c) {
         case 'w':
            wintype = (Window) atoi(s);
            break;
         case 'l':
            fleng = atoi(s);
            break;
         case 'L':
            outl = atoi(s);
            break;
         case 'n':
            normflg = atoi(s);
            break;
         case 'h':
         default:
            usage ();
         }
      }
      else
         fp = getfp(*argv, "r");
   }

   if (outl<0)
      outl = fleng;

   x = dgetmem(fleng>outl ? fleng : outl);

   while (freadf(x, sizeof(*x), fleng, fp)==fleng) {
      window(wintype, x, fleng, normflg);
      fwritef(x, sizeof(*x), outl, stdout);
   }

   return(0);
}

