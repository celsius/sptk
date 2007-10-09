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

/********************************************************
*                                                       *
*   psgr:  xy-plotter simulator for EPSF                *
*                                                       *
*      Ver. 0.95  '92.3 T.Kanno                         *
*      Ver. 0.96  '92.8                                 *
*      Ver. 0.97  '92.10                                *
*      Ver. 0.98  '93.2                                 *
*      Ver. 0.99  '93.8                                 *
********************************************************/

static char *rcs_id = "$Id: psgr.c,v 1.13 2007/10/09 09:34:10 heigazen Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>
#include <SPTK.h>
#include "psgr.h"

char *BOOL[] = {"FALSE", "TRUE"};

#if HAS_STDLIB
#include <strings.h>
#define RINDEX(s, c)    rindex(s, c)
#else
#include <string.h>
#define RINDEX(s, c)    strrchr(s, c)
#endif

#define MaxPaperTypes 6 /*  Paper Media  */

struct page_media paper[] = {
   {"Letter",  612,  792}, 
   {"A3",      842, 1190}, 
   {"A4",      842,  842},
/* {"A4",      595,  842}, */ 
   {"A5",      420,  595}, 
   {"B4",      729, 1032}, 
   {"B5",      516,  729},
};

char *orientations[] = { /*  Orientation  */
   "Portrait",
   "Landscape",
};


/* Default Values */
#define MEDIA       "A4"
#define ORIENTATION "Portrait"
#define PSMODE      FA
#define PAPERNUM    2
#define XLENG       595
#define YLENG       842
#define LANDSCAPE   FA
#define RESOLUTION  600
#define FONTNO      1
#define CLIPMODE    FA
#define NCOPY       1
#define XOFFSET     0
#define YOFFSET     0
#define SHRINK      1.0
#define SCALE 10


void usage (int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - XY-plotter simulator for EPSF\n\n",progname);
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", progname);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -t t  : title of figure      [NULL]\n");
   fprintf(stderr, "       -s s  : shrink               [%g]\n",SHRINK);
   fprintf(stderr, "       -c c  : number of copy       [%d]\n",NCOPY);
   fprintf(stderr, "       -x x  : x offset <mm>        [%d]\n",XOFFSET);
   fprintf(stderr, "       -y y  : y offset <mm>        [%d]\n",YOFFSET);
   fprintf(stderr, "       -p p  : paper                [%s]\n",MEDIA);
   fprintf(stderr, "               (Letter,A3,A4,A5,B4,B5)\n");
   fprintf(stderr, "       -l    : landscape            [%s]\n",BOOL[LANDSCAPE]);
   fprintf(stderr, "       -r r  : resolution           [%d dpi]\n",RESOLUTION);
   fprintf(stderr, "       -b    : bold mode            [FALSE]\n");
   fprintf(stderr, "       -T T  : top    margin <mm>   [%d]\n",bbm.top);
   fprintf(stderr, "       -B B  : bottom margin <mm>   [%d]\n",bbm.bottom);
   fprintf(stderr, "       -L L  : left   margin <mm>   [%d]\n",bbm.left);
   fprintf(stderr, "       -R R  : right  margin <mm>   [%d]\n",bbm.right);
   fprintf(stderr, "       -P    : output PS            [%s]\n",BOOL[PSMODE]);
   fprintf(stderr, "       -h    : print this message \n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       plotter commands             [stdin]\n");
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       PostScript codes (EPSF)\n");
#ifdef PACKAGE_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n",PACKAGE_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(status);
}

char *progname,  *filename=NULL, *title=NULL;
char *media=MEDIA, *orientation=ORIENTATION;

int paper_num=PAPERNUM, xleng=XLENG, yleng=842, resolution=RESOLUTION;
int font_no=FONTNO, psmode=PSMODE, landscape=LANDSCAPE, clip_mode=CLIPMODE;


int main (int argc,char *argv[] )
{
   char *str, flg, c;
   FILE *fp=NULL;
   int i;
   int ncopy=NCOPY, xoffset=XOFFSET, yoffset=YOFFSET;
   float shrink=SHRINK;

   progname = *argv;
   if (RINDEX(progname, '/'))
      progname = (char *)(RINDEX(progname, '/') + 1);
   while (--argc)  {
      if (*(str=*++argv)=='-')  {
         flg = *++str;
         if ((flg!='P' && flg!='l' && flg!='b')
              && *++str=='\0')  {
            str = *++argv;
            argc--;
         }
         switch (flg)  {
         case 'P':
            psmode = 1 - psmode;
            break;
         case 't':
            title = str;
            break;
         case 'c':
            ncopy = atoi(str);
            break;
         case 's':
            shrink = atof(str);
            break;
         case 'x':
            xoffset = atoi(str)*SCALE;
            break;
         case 'y':
            yoffset = atoi(str)*SCALE;
            break;
         case 'p':
            media = str;
            break;
         case 'l':
            landscape = 1 - landscape;
            break;
         case 'r':
            resolution = atoi(str);
            break;
         case 'T':
            bbm.top    = atoi(str)*10;
            break;
         case 'B':
            bbm.bottom = atoi(str)*10;
            break;
         case 'L':
            bbm.left   = atoi(str)*10;
            break;
         case 'R':
            bbm.right  = atoi(str)*10;
            break;
         case 'b':
            font_no += 2;
            break;
         case 'h':
            usage (0);
            break;
         default :
            fprintf(stderr, "%s : Invalid option '%c'!\n", progname, flg);
            usage (1);
            break;
         }
      }
      else
         filename = str;
   }
   for (i=0; i<MaxPaperTypes; i++)  {
      if (strcmp(media, paper[i].size))  {
         paper_num = i;
         break;
      }
   }
   if (!landscape)  {  /*  Portrait  */
      xleng = paper[paper_num].width;
      yleng = paper[paper_num].height;
   }
   else  {    /*  Landscape  */
      xleng = paper[paper_num].height;
      yleng = paper[paper_num].width;
   }
   xleng *= SCALE;
   yleng *= SCALE;

   orientation = orientations[landscape];

   if (filename!=NULL) {
      fp = getfp(filename, "rt");
   }
   else  {
      fp = tmpfile();
      while ((c=getchar())!=(char)EOF)
         fputc(c, fp);
      rewind(fp);
   }

   ungetc(flg=fgetc(fp), fp);
   if (flg==(char)EOF)  {
      fprintf(stderr, "%s : Input file is empty!\n", progname);
      return(-1);
   }
   else if (flg!='=')  {
      fprintf(stderr, "%s : Unexpected data format!\n", progname);
      return(-1);
   }

   epsf_setup(fp, shrink, xoffset, yoffset, bbm, ncopy);
   plot(fp);
   epsf_end();

   fclose(fp);
   return (0);
}

