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

/*
 * $Id$
 *  Headder for xgr
 */


/*  for SUN  */
   
#define DEFAULT_SHRINK   (254.0/75.0)
#define X0      558             /*  max -> 530   */
#define Y0      1               /*  max -> 23   */
#define X0_LAND 300

#if     NeXT
#define DEFAULT_SHRINK   3.6
#define X0      561
#define Y0      1

#elif   DGAV
#define DEFAULT_SHRINK   3.0   /*  max -> 2.9  */
#define X0      610            /*  max -> 590  */
#define Y0      1
#endif

#define XLENG   1995           /*  A4 Width  [x0.1 mm](1950,max=2095)  */
#define YLENG   2870           /*  A4 Height [x0.1 mm](2850,max=2970)  */
#define XLENG_LAND   2870
#define YLENG_LAND   1995

typedef struct cord  {
        int   x;
        int   y;
} Cord;

void open_display(char *display_name );
void init_window(int argc,char *argv[]);
void init_pixmap(void);
void close_window(void);
void main_loop(void);
void realize(void);
void realize_part(int src_x,int src_y,int width,int height,int dest_x,int dest_y );
void set_all(void);
void beep(void);
void get_window_size(void);
unsigned long   get_color_pix(char *color_name );
void plot(void);
