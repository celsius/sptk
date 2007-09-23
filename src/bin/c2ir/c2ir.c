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
*    Cepstrum to minimum phase Impulse Response                         *
*                                       1988.8  T.Kobayashi             *
*                                       1995.12 N.Isshiki   modified    *
*                                                                       *
*       usage:                                                          *
*               c2ir [ options ] [ infile ] > stdout                    *
*       options:                                                        *
*               -m m    :  order of cepstrum            [25]            *
*               -M M    :  order of impulse response    [255]           *
*               -L L    :  length of impulse responce   [256]           *
*               -i      :  input minimum phase sequence [FALSE]         *
*       infile:                                                         *
*               stdin for default                                       *
*               input is assumed to be float                            *
*                                                                       *
************************************************************************/

static char *rcs_id="$Id$";


/* Standard C Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SPTK.h>


/* Default Values */
#define LENG 256
#define ORDER 25

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
   fprintf(stderr, " %s - cepstrum to minimum phase impulse response\n", cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -m m  : order of cepstrum            [%d]\n", ORDER);
   fprintf(stderr, "       -M M  : order of impulse response    [%d]\n", LENG-1);
   fprintf(stderr, "       -L L  : length of impulse response   [%d]\n", LENG);
   fprintf(stderr, "       -i    : input minimum phase sequence [FALSE]\n");
   fprintf(stderr, "       -h    : print this message\n\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       cepstrum (%s)                     [stdin]\n", FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       impulse response (%s)\n", FORMAT);
#ifdef PACKAGE_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n",PACKAGE_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(status);
}


int main (int argc,char *argv[])
{
   FILE *fp;
   double *buf, *x;
   char *s, *infile=NULL, c;
   int nr;
   int leng=LENG, nc=ORDER+1, is_i=0;

   if ((cmnd = strrchr(argv[0], '/')) == NULL)
      cmnd = argv[0];
   else
      cmnd++;
   
   while (--argc) {
      if (*(s = *++argv)=='-') {
         c = *++s;
         if (c!='i' && *++s=='\0') {
            s = *++argv;
            --argc;
         }
         switch (c) {
         case 'i':
            is_i = 1;
            break;
         case 'm':
            nc = atoi(s) + 1;
            break;
         case 'M':
            leng = atoi(s) + 1;
            break;
         case 'L':
            leng = atoi(s);
            break;
         case 'h':
            usage(0);            
         default:
            fprintf(stderr, "%s: unknown option '%c'\n", cmnd, c);
            usage(1);
         }
      }
      else
         infile = s;
   }
   
   if (infile) {
      fp = getfp(infile,"r");
   }
   else
      fp=stdin;

   nr = leng + nc;
   buf = dgetmem(nr);
   nr = (is_i) ? leng : nc;

   while (freadf(buf,sizeof(*buf),nr,fp)==nr) {
      if (!is_i) {
         x = buf + nc;
         c2ir(buf,nc,x,leng);
         fwritef(x, sizeof(*x), leng, stdout);
      } 
      else {
         x = buf + leng;
         ic2ir(buf,leng,x,nc);
         fwritef(x, sizeof(*x), nc, stdout);
      }
   }
   
   return(0);
}
