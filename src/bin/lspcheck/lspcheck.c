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
*    Check stability and rearrange LSP                                  *
*                                                                       *
*                                         1996.6  K.Koishida            *
*                                                                       *
*       usage:                                                          *
*               lspcheck [ options ] [ infile ] > stdout                *
*       options:                                                        *
*               -m m     :  order of LSP                [25]            *
*               -s s     :  sampling frequency          [10]            *
*               -k       :  input & output gain         [FALSE]         *
*               -i i     :  input format                [0]             *
*               -o o     :  output format               [i]             *
*                             0 (normalized frequency <0...pi>)         *   
*                             1 (normalized frequency <0...0.5>)        *
*                             2 (frequency (kHz))                       *
*                             3 (frequency (Hz))                        *
*               -r       :  rearrange LSP               [FALSE]         *
*       infile:                                                         *
*               LSP                                                     *
*                       , f(1), ..., f(m),                              *
*       stdout:                                                         *
*               frame number of irregular LSP of                        *
*               rearranged LSP if -r option is specified                *
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
#define ORDER  25
#define ITYPE  0
#define OTYPE  -1
#define SAMPLING 10
#define ARRANGE  FA
#define GAIN  TR

char *BOOL[] = {"FALSE", "TRUE"};

/*  Command Name  */
char *cmnd;


void usage (int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - check stability and rearrange LSP\n",cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -m m  : order of LPC        [%d]\n", ORDER);
   fprintf(stderr, "       -s s  : sampling frequency  [%d]\n", SAMPLING);
   fprintf(stderr, "       -k    : input & output gain [%s]\n", BOOL[GAIN]);
   fprintf(stderr, "       -i i  : input format        [%d]\n", ITYPE);
   fprintf(stderr, "       -o o  : output format       [i]\n");
   fprintf(stderr, "                 0 (normalized frequency <0...pi>)\n");
   fprintf(stderr, "                 1 (normalized frequency <0...0.5>)\n");
   fprintf(stderr, "                 2 (frequency (kHz))\n");
   fprintf(stderr, "                 3 (frequency (Hz))\n");
   fprintf(stderr, "       -r    : rearrange LSP       [%s]\n", BOOL[ARRANGE]);
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       LSP (%s)                 [stdin]\n", FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       frame number of irregular LSP or\n");
   fprintf(stderr, "       rearranged LSP (%s) if -r option is specified\n", FORMAT);
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
   int m=ORDER, sampling=SAMPLING, itype=ITYPE, otype=OTYPE, i, num;
   Boolean arrange=ARRANGE, gain=GAIN;
   FILE *fp=stdin;
   double *lsp, *lsp1;

   if ((cmnd=strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;
   while (--argc)
      if (**++argv=='-') {
         switch (*(*argv+1)) {
         case 'm':
            m = atoi(*++argv);
            --argc;
            break;
         case 's':
            sampling = atoi(*++argv);
            --argc;
            break;
         case 'i':
            itype = atoi(*++argv);
            --argc;
            break;
         case 'o':
            otype = atoi(*++argv);
            --argc;
            break;
         case 'r':
            arrange = 1 - arrange;
            break;
         case 'k':
            gain = 1 - gain;
            break;
         case 'h':
            usage (0);
         default:
            fprintf(stderr, "%s : Invalid option '%c'!\n", cmnd, *(*argv+1));
            usage (1);
         }
      }
      else
         fp = getfp(*argv, "r");

   if (otype<0)
      otype = itype;

   lsp = dgetmem(m+m+gain);
   lsp1 = lsp + m + gain;

   num = 0;
   while (freadf(lsp, sizeof(*lsp), m+gain, fp)==m+gain) {
      if (itype==0)
         for (i=gain; i<m+gain; i++)
            lsp1[i] = lsp[i] / PI2;
      else if (itype==2 || itype ==3)
         for (i=gain; i<m+gain; i++)
            lsp1[i] = lsp[i] / sampling;

      if (itype==3)
         for (i=gain; i<m+gain; i++)
            lsp1[i] = lsp[i] / 1000;

      if (lspcheck(lsp1+gain, m)==-1) {
         if (! arrange) {
            printf("frame number : %d\n",num);
            for (i=0; i<m+gain; i++)
               printf("%f\n",lsp[i]);
            printf("\n");
         }
         else {
            lsparrange(lsp1+gain, m);

            if (otype==0)
               for (i=gain; i<m+gain; i++)
                  lsp1[i] *= PI2;
            else if (otype==2 || otype==3)
               for (i=gain; i<m+gain; i++)
                  lsp1[i] *= sampling;

            if (otype==3)
               for (i=gain; i<m+gain; i++)
                  lsp1[i] *= 1000;

            fwritef(lsp1, sizeof(*lsp1), m+gain, stdout);
         }
      }
      else if (arrange) {
         if (itype==otype)
            fwritef(lsp, sizeof(*lsp), m+gain, stdout);
         else {
            if (otype==0)
               for (i=gain; i<m+gain; i++)
                  lsp1[i] *= PI2;
            else if (otype==2 || otype==3)
               for (i=gain; i<m+gain; i++)
                  lsp1[i] *= sampling;

            if (otype==3)
               for (i=gain; i<m+gain; i++)
                  lsp1[i] *= 1000;
            fwritef(lsp, sizeof(*lsp), m+gain, stdout);
         }
      }
      num++;
   }
   
   return(0);
}

