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
#include <stdio.h>

struct bbmargin {      /*  Bounding Box Margin  */
   int top;
   int bottom;
   int left;
   int right;
} bbm;

struct page_media {
   char *size;
   int width;
   int height;
};

#define PU_PT (72.0/254.0)
#define SHIFT 15
#define LAND_OFFSET 254
#define SCALE 10

#define MIN_OFFSET 12
#define MAX_OFFSET 17
#define CHAR_HEIGHT 10

#define norm(x) (int)(x)

typedef struct cord {
   int x;
   int y;
} Cord;

extern char *filename;
extern char *title;
extern char *progname;

extern struct page_media paper[];
extern char *orientations[];

extern char *media;
extern int xleng;
extern int yleng;
extern int resolution;
extern int paper_num;
extern char *orientation;

extern int psmode;
extern int landscape;
extern int font_no;
extern int clip_mode;

void epsf_setup(FILE *fp, float shrink, int xoffset, int yoffset, struct bbmargin bbm, int ncopy );
void epsf_end(void);
void plot(FILE *fp);
void dict (void);
