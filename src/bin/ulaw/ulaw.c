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
*    u-law PCM                                                          *
*                                                                       *
*                                      1991.3  T.Kanno                  *
*                                      1996.11 K.Koishida               *
*                                                                       *
*       usage:                                                          *
*               ulaw [ options ] [ infile ] > stdout                    *
*       options:                                                        *
*               -v v     :  maximum of input     [32768]                *
*               -u u     :  compression ratio    [256]                  *
*               -c       :  coder mode           [TRUE]                 *
*               -d       :  decoder mode         [FALSE]                *
*       infile:                                                         *
*               input sequence                                          *
*                       , x(0), x(1), ...,                              *
*       stdout:                                                         *
*               compressed sequence                                     *
*                       , x'(0), x'(1), ...,                            *
*       require:                                                        *
*               ulaw_e(), ulaw_d()                                      *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id: ulaw.c,v 1.17 2007/10/16 02:20:59 heigazen Exp $";


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

#if defined(WIN32)
#  include "SPTK.h"
#else
#  include <SPTK.h>
#endif

/*  Default Values  */
#define MAXVALUE 32768.0
#define MU       256.0
#define CODER    TR
#define DECODER  FA

char *BOOL[] = {"FALSE", "TRUE"};

/*  Command Name  */
char *cmnd;


void usage (int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - u-law PCM\n",cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -v v  : maximum of input    [%g]\n", MAXVALUE);
   fprintf(stderr, "       -u u  : compression ratio   [%g]\n", MU);
   fprintf(stderr, "       -c    : coder mode          [%s]\n", BOOL[CODER]);
   fprintf(stderr, "       -d    : decoder mode        [%s]\n", BOOL[DECODER]);
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       input sequence      (%s) [stdin]\n", FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       compressed sequence (%s)\n", FORMAT);
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
   double x, y, max = MAXVALUE, mu = MU, atof();
   Boolean decoder = DECODER;
   FILE *fp = stdin;

   if ((cmnd = strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;
   while (--argc)
      if (**++argv=='-') {
         switch (*(*argv+1)) {
         case 'v':
            max = atof(*++argv);
            --argc;
            break;
         case 'u':
            mu = atof(*++argv);
            --argc;
            break;
         case 'c':
            decoder = FA;
            break;
         case 'd':
            decoder = TR;
            break;
         case 'h':
            usage (0);
         default:
            fprintf(stderr, "%s : Invalid option '%c'!\n", cmnd, *(*argv+1));
            usage (1);
         }
      }
      else
         fp = getfp(*argv, "rb");

   if (! decoder)
      while (freadf(&x, sizeof(x), 1, fp)==1) {
         y = ulaw_c(x, max, mu);
         fwritef(&y, sizeof(y), 1, stdout);
      }
   else
      while (freadf(&x, sizeof(x), 1, fp)==1) {
         y = ulaw_d(x, max, mu);
         fwritef(&y, sizeof(y), 1, stdout);
      }

   return(0);
}
