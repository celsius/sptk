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
*    Block Copy                                                         *
*                                                                       *
*                                       1988.7  T.Kobayashi             *
*                                       1996.5  K.Koishida              *
*                                                                       *
*       usage:                                                          *
*               bcp [options] [infile] > stdout                         *
*       options:                                                        *
*               -l l     :  number of items contained 1 block   [512]   *
*               -L L     :  number of destination block size    [N/A]   *
*               -n n     :  order of items contained 1 block    [l-1]   *
*               -N N     :  order of destination block size     [N/A]   *
*               -s s     :  start number                        [0]     *
*               -S S     :  start number in destination block   [0]     *
*               -e e     :  end number                          [EOF]   *
*               -f f     :  fill into empty block               [0.0]   *
*               +type    :  data type                           [f]     *
*                               c (char)     s (short)                  *
*                               i (int)      l (long)                   *
*                               f (float)    d (double)                 *
*                               a (ascii)                               *
*       infile:                                                         *   
*               data sequence                                   [stdin] *
*       stdout:                                                         *
*               copied data sequence                                    *
*       note:                                                           *
*               When both (-L and -N) or (-l and -n) are specified,     *
*               latter argument is adopted.                             *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#if defined(WIN32)
#include "SPTK.h"
#else
#include <SPTK.h>
#endif

/*  Default Values  */
#define START 0
#define END -1
#define DSTART 0
#define ITEM 512
#define DITEM 0
#define FILL 0.0


/*  Command Name  */
char   *cmnd;


void usage (int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - block copy\n",cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -l l  : number of items contained 1 block      [%d]\n",ITEM);
   fprintf(stderr, "       -L L  : number of destination block size       [N/A]\n");
   fprintf(stderr, "       -n n  : order of items contained 1 block       [l-1]\n");
   fprintf(stderr, "       -N N  : order of destination block size        [N/A]\n");
   fprintf(stderr, "       -s s  : start number                           [%d]\n",START);
   fprintf(stderr, "       -S S  : start number in destination block      [%d]\n",DSTART);
   fprintf(stderr, "       -e e  : end number                             [EOF]\n");
   fprintf(stderr, "       -f f  : fill into empty block                  [%g]\n",FILL);
   fprintf(stderr, "       +type : data type                              [f]\n");
   fprintf(stderr, "                c (char)      s (short)\n");
   fprintf(stderr, "                i (int)       l (long)\n");
   fprintf(stderr, "                f (float)     d (double)\n");
   fprintf(stderr, "                a (ascii)\n");
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       data sequence                                  [stdin]\n");
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       copied data sequence\n");
   fprintf(stderr, "  note:\n");
   fprintf(stderr, "       When both (-L and -N) or (-l and -n) are specified,\n");
   fprintf(stderr, "       latter argument is adopted.\n");
#ifdef PACKAGE_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n",PACKAGE_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(status);
}

int sno=START, eno=END, dsno=DSTART, size=sizeof(float), nitems=ITEM, dnitems=DITEM;
double fl=FILL;
char type='f';

union typex{
   char c;
   short s;
   long l;
   int i;
   float f;
   double d;
} fillx;

int main (int argc, char **argv)
{
   FILE *fp=stdin;
   char *s, c;
   
   void bcp (FILE *fp);
    
   if ((cmnd = strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;
   while (--argc)
      if (*(s = *++argv)=='-') {
         c = *++s;
         switch(c) {
         case 'n':
            nitems = atoi(*++argv)+1;
            --argc;
            break;
         case 'N':
            dnitems = atoi(*++argv)+1;
            --argc;
            break;
         case 'b':
            nitems = atoi(*++argv);
            --argc;
            break;
         case 'l':
            nitems = atoi(*++argv);
            --argc;
            break;
         case 'B':
            dnitems = atoi(*++argv);
            --argc;
            break;
         case 'L':
            dnitems = atoi(*++argv);
            --argc;
            break;
         case 's':
            sno = atoi(*++argv);
            --argc;
            break;
         case 'S':
            dsno = atoi(*++argv);
            --argc;
            break;
         case 'e':
            eno = atoi(*++argv) + 1;
            --argc;
            break;
         case 'f':
            fl = atof(*++argv);
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
         type = *++s;
   
         switch (type) {
         case 'a':
            size = 0;
            break;
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
         fp = getfp(*argv, "r");

   if (eno<0)
      eno = nitems;
   if (sno<0 || sno>=nitems || sno>eno || eno>nitems || dsno<0)
      return(1);

   if (dnitems==0)
      dnitems = eno - sno + dsno;

   bcp(fp);
    
   return(0);
}

void bcp (FILE *fp)
{
   char *buf, *lz=NULL, *fz=NULL;
   int ibytes, obytes, offset, nlz, nfz;
   void acopy (FILE *fp);
   void filln (char *ptr, int size, int nitem);

   switch (type) {
   case 'c':
      fillx.c = (char)fl;
      break;
   case 's':
      fillx.s = (short)fl;
      break;
   case 'l':
      fillx.l = (long)fl;
      break;
   case 'i':
      fillx.i = (int)fl;
      break;
   case 'f':
      fillx.f = (float)fl;
      break;
   case 'd':
      fillx.d = (double)fl;
      break;
   case 'a':
      break;
   }
    
   if (size==0) {
      acopy(fp);
      return;
   }

   ibytes = size * nitems;
   offset = size * sno;
   obytes = size * (eno - sno);
   nlz    = size * dsno;
   nfz    = ((nfz = size * dnitems - nlz - obytes) < 0) ? 0 : nfz;
   
   if ((buf = (char *)dgetmem(ibytes + nlz + nfz))==NULL)
      return;

   if (nlz) {
      lz = buf + ibytes;
      filln(lz, size, nlz);
   }

   if (nfz) {
      fz = buf + ibytes + nlz;
      filln(fz, size, nfz);
   }

   while (freadx(buf, sizeof(*buf), ibytes, fp)==ibytes) {
      if (nlz)
      fwritex(lz, sizeof(*lz), nlz, stdout);
      fwritex(buf + offset, sizeof(*buf), obytes, stdout);

      if (nfz)
         fwritex(fz, sizeof(*fz), nfz, stdout);
   }
}

void acopy (FILE *fp)
{
   char s[512];
   int n, dn;
   int getstr (FILE *fp, char *s);

   for (dn=0; !feof(fp); ) {
      for (n=0; n<sno; n++)
         if (getstr(fp, s)==0) break;

      for ( ; n<eno; n++) {
         if (getstr(fp, s)==0)
            break;
         if (dn++)
            putchar(' ');
         printf("%s", s);
         if (dn==dnitems) {
            putchar('\n');
            dn = 0;
         }
      }
      for( ; n<nitems; n++)
         if (getstr(fp, s)==0)
            break;
   }
}

int getstr (FILE *fp, char *s)
{
   int c;
    
   while ((c=getc(fp))==' ' || c=='t' || c=='\n' || c==',');
    
   if (c == EOF)
      return(0);
      
   for (*s++=c; (c=getc(fp))!=EOF && c!=' ' && c!='\t' && c!='\n' && c!=','; )
      *s++ = c;
    
   *s = '\0';
   return(1);
}

void filln (char *ptr, int size, int nitem)
{
   int i;
   char *c;
    
   nitem=nitem/size;
   while (nitem--) {
      c = &fillx.c;
      for (i=1; i<=size; i++) {
         *ptr++ = *c++;
      }
   }
}
