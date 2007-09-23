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
*    Zero Cross                                                         *
*                                                                       *
*                                       1996.3 N.Isshiki                *
*                                                                       *
*       usage:                                                          *
*               zcross [ options ] [ infile ] > stdout                  *
*       options:                                                        *
*               -l l  :  frame length                   [256]           *
*               -n    :  normalized by frame length     [FALSE]         *
*       infile:                                                         *
*               stdin for default                                       *
*               input is assumed to be real                             *
************************************************************************/

static char *rcs_id = "$Id$";


/* Standard C Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <SPTK.h>
#include <string.h>


/* Default Values */
#define FLENG 256
#define NORM FA

/* Command Name  */
char *cmnd;

void usage (status)
int status;
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - zero cross\n", cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -l l  : frame length               [%d]\n",FLENG);
   fprintf(stderr, "       -n    : normarized by frame length [FALSE]\n");
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       data sequence (%s)              [stdin]\n", FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       zero cross rate (%s)\n", FORMAT);
#ifdef PACKAGE_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n",PACKAGE_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(status);
}

int main (int argc,char **argv)
{

   FILE *fp = stdin;
   char *s, *infile = NULL, c;
   double *x;
   int flng = FLENG;
   double z;
   Boolean norm = NORM;

   if ((cmnd = strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;

   while (--argc) {
      if (*(s = *++argv)=='-') {
         c = *++s;
         if (c!='n' && *++s=='\0') {
            s = *++argv;
            --argc;
         }
         switch (c) {
         case 'l':
            flng = atoi(s);
            break;
         case 'n':
            norm = 1 - norm;
            break;
         case 'h':
            usage (0);
         default:
            fprintf(stderr,
                    "%s: unknown option '%c'\n", cmnd, c);
            usage (1);
            break;
         }
      }
      else
         infile = s;
   }
   if (infile)
      fp = getfp(infile,"r");

   x = dgetmem(flng);
   while (freadf(x, sizeof(*x), flng, fp)==flng) {
      z = zcross(x, flng, norm);
      fwritef(&z, sizeof(z), 1, stdout);
   }
   return(0);
}
