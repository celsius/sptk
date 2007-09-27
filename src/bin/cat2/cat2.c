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
*       concatenate and display files to the standard error             *
*                                                                       *
*                                             2000.6  S.Sako            *
*                                                                       *
*       usage:                                                          *
*               cat2 [ options ] [ infile1 ] [ infile2 ] ...            *
*       options:                                                        *
*               -n  :  output with line number                 [FALSE]  *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id: cat2.c,v 1.13 2007/09/27 03:36:47 heigazen Exp $";


/*  Standard C Libraries **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*  Command Name  */
char *cmnd;

void usage (int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - concatenate and display files to the standard error\n",cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile1 ] [ infile2 ] ...\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -n    : output with line number    [FALSE]\n");
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       data sequence                      [stdin]\n");
   fprintf(stderr, "  notice:\n");
#ifdef PACKAGE_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n", PACKAGE_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(status);
}

int main (int argc, char **argv)
{
   int linenum=0;
   int stdinmode=0;
   char buf[512];
   FILE *fp;

   if ((cmnd = strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;
   
   while (--argc) {
      if (**++argv=='-') {
         switch (*(*argv+1)) {
         case 'n':
            linenum = 1;
            break;
         case 'h':
            usage(0);
         default:
            fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
            usage(1);
         }
      }
      else {
         stdinmode = -1;
         if ( (fp = fopen( *argv, "r"))==NULL) {
            fprintf(stderr, "%s : cannot open %s\n", cmnd, *argv);
            return(1);
         }
         while(fgets(buf, 512, fp)!=0) {
            if (linenum>0) fprintf(stderr, "%6d  ", linenum++);
            fputs(buf, stderr);
         }
      }
   }

   if (stdinmode==0) {
      while (fgets( buf, 512, stdin)!=0) {
         if (linenum>0) fprintf( stderr, "%6d  ", linenum++);
         fputs(buf, stderr);
      }
   }
   
   return 0;
}
