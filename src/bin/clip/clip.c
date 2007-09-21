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
*    data clipping                                                      *
*                                                                       *
*                                     1989.8.31   K.Tokuda              *
*                                                                       *
*       usage:                                                          *
*               clip [option] [infile] > stdout                         *
*       options:                                                        *
*               -y    ymin ymax : lower bound & upper bound [-1.0 1.0]  *
*               -ymin ymin      : lower bound (ymax = inf)  [N/A]       *
*               -ymax ymax      : upper bound (ymin = -inf) [N/A]       *
*       infile:                                                         *
*               data sequence (float)                                   *
*       stdout:                                                         *
*               clipped data sequence (float)                           *
*       require:                                                        *
*               clip()                                                  *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id: clip.c,v 1.11 2007/09/21 15:18:53 heigazen Exp $";


/* Standard C Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SPTK.h>


/* Default Value */
#define FLT_MAX 3.4e+38
#define FLT_MIN -3.4e+38
#define YMIN -1.0
#define YMAX 1.0

#ifdef DOUBLE
char *FORMAT = "double";
#else
char *FORMAT = "float";
#endif /* DOUBLE */

/* Command Name */
char *cmnd;


void usage (int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - data clipping\n", cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ option ] [ infile ] > outfile\n", cmnd);
   fprintf(stderr, "  option:\n");
   fprintf(stderr, "       -y    ymin ymax : lower bound & upper bound [-1.0 1.0]\n");
   fprintf(stderr, "       -ymin ymin      : lower bound (ymax = inf)  [N/A]\n");
   fprintf(stderr, "       -ymax ymax      : upper bound (ymin = -inf) [N/A]\n");
   fprintf(stderr, "       -h              : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       data sequence (%s)                       [stdin]\n", FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       clipped data sequence (%s)\n", FORMAT);
#ifdef SPTK_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n", SPTK_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(status);
}

int main (int argc, char *argv[])
{
   FILE *fp=stdin;
   int size;
   double ymin=YMIN, ymax=YMAX;
   double *x, *y;

   if ((cmnd = strrchr(argv[0],'/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;
   while (--argc)
      if (**++argv=='-') {
         argc--;
         switch (*(*argv+1)) {
         case 'y':
            if ((*(*argv+2))=='m') {
               switch (*(*argv+3)) {
               case 'i':
                  ymin = atof(*++argv);
                  ymax = FLT_MAX;
                  break;
               case 'a':
                  ymax = atof(*++argv);
                  ymin = FLT_MIN;
                  break;
               }
            }
            else {
               ymin = atof(*++argv);
               argc--;
               ymax = atof(*++argv);
            }
            break;
         case 'h':
         default:
            usage(1);
         }
      } 
      else 
         fp = getfp(*argv, "r");

   x = dgetmem(2*256);
   y = x + 256;

   while ((size = freadf(x, sizeof(*x), 256, fp))!=0) {
      clip (x, size, ymin, ymax, y);
      fwritef(y, sizeof(*y), size, stdout);
   }
   
   return 0;
}
