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
*  Swap Bytes                                                           *
*                                                                       *
*                                   1996    T.Kobayashi                 *
*                                   1997.3  G.Hirabayashi               *
*                                                                       *
*     usage:                                                            *
*             swab [ options ] [ infile ] > stdout                      *
*     options:                                                          *
*             -S S     :  start address                [0]              *
*             -s s     :  start offset number          [0]              *
*             -E E     :  end address                  [EOF]            *
*             -e e     :  end offset number            [0]              *
*             +type    :  input and output data type   [s]              *
*                          s (short)    l (long)                        *
*                          f (float)    d (double)                      *
*     infile:                                                           *
*             data sequence                            [stdin]          * 
*     stdout:                                                           *
*             swapped data sequence                                     *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id: swab.c,v 1.11 2007/09/21 15:18:55 heigazen Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SPTK.h>


/*  Command Name  */
char *cmnd;


/*  Default Values  */
#define START 0
#define SNO   0
#define END   0x7fffffff
#define ENO   0x7fffffff


void usage (int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - Swap Bytes\n",cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -S S   : start address                [%d]\n",START);
   fprintf(stderr, "       -s s   : start offset number          [%d]\n",SNO);
   fprintf(stderr, "       -E E   : end address                  [EOF]\n");
   fprintf(stderr, "       -e e   : end offset number            [0]\n");
   fprintf(stderr, "       +type  : input and output data format [s]\n");
   fprintf(stderr, "                 s (short)     l (long)\n");
   fprintf(stderr, "                 f (float)     d (double)\n");
   fprintf(stderr, "       -h     : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       data sequence                [stdin]\n");
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       swapped data sequence\n");
#ifdef SPTK_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n",SPTK_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(status);
}

static long start=START, _end=END, sno=SNO, eno=ENO;

int main (int argc,char *argv[])
{
   FILE *fp=stdin;
   char *s;
   int c;
   size_t iosize=2;
   void conv (FILE *fp, size_t iosize);
   

   if ((cmnd=strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;
   while (--argc)
      if (*(s=*++argv)=='-') {
         c = *++s;
         switch (c) {
         case 'S':
            start = atol(*++argv);
            --argc;
            break;
         case 's':
            sno = atol(*++argv);
            --argc;
            break;
         case 'E':
            _end = atol(*++argv);
            --argc;
            break;
         case 'e':
            eno = atol(*++argv);
            --argc;
            break;
         case 'h':
            usage (0);
         default:
            fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
            usage (1);
         }
      }
      else if (*s=='+') {
         c = *++s;
         switch (c) {
         case 's':
            iosize = sizeof(short);
            break;
         case 'l':
            iosize = sizeof(long);
            break;
         case 'f':
            iosize = sizeof(float);
            break;
         case 'd':
            iosize = sizeof(double);
            break;
         default:
            fprintf(stderr, "%s : Invalid option '%c' !\n",
                    cmnd, *(*argv+1));
            usage (1);
         }
      }
      else
         fp = getfp(*argv, "r");

   conv(fp, iosize);
   
   return(0);
}

void conv (FILE *fp, size_t iosize)
{
   long adrs, n;
   int i;
   char ibuf[8], obuf[8];
   int ffseek (FILE *fp, long off);

   if (ffseek(fp, adrs=start + iosize * sno))
      return;

   for (n=sno; (adrs<=_end) && (n<=eno); adrs+=iosize,++n) {
      fread(ibuf, iosize, 1, fp);
      if (feof(fp))
         break;
      for (i=0; i<iosize; ++i)
         obuf[i] = ibuf[iosize - 1 - i];
      fwrite(obuf, iosize, 1, stdout);
   }
 
   return;
}

int ffseek (FILE *fp, long off)
{
   int n;

   if (fp!=stdin)
      fseek(fp, off, 0);
   else {
      for (n=off; n; --n)
         if (getc(fp)==EOF)
            return(1);
   }
   return(0);
}
