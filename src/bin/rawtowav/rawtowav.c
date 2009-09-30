/* ----------------------------------------------------------------- */
/*             The Speech Signal Processing Toolkit (SPTK)           */
/*             developed by SPTK Working Group                       */
/*             http://sp-tk.sourceforge.net/                         */
/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 1984-2007  Tokyo Institute of Technology           */
/*                           Interdisciplinary Graduate School of    */
/*                           Science and Engineering                 */
/*                                                                   */
/*                1996-2009  Nagoya Institute of Technology          */
/*                           Department of Computer Science          */
/*                                                                   */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/* - Redistributions of source code must retain the above copyright  */
/*   notice, this list of conditions and the following disclaimer.   */
/* - Redistributions in binary form must reproduce the above         */
/*   copyright notice, this list of conditions and the following     */
/*   disclaimer in the documentation and/or other materials provided */
/*   with the distribution.                                          */
/* - Neither the name of the SPTK working group nor the names of its */
/*   contributors may be used to endorse or promote products derived */
/*   from this software without specific prior written permission.   */
/*                                                                   */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS */
/* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          */
/* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY    */
/* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           */
/* POSSIBILITY OF SUCH DAMAGE.                                       */
/* ----------------------------------------------------------------- */

/************************************************************************
*                                                                       *
*    convert raw file format to wav file format                         *
*                                                                       *
*                                       2009.9 A.Saito modified         *
*       usage:                                                          *
*               rawtowav [ fs(Hz) ] [ infile ] [ outfile ]              *
*       infile:                                                         *
*               raw file format                                         *
*       outfile:                                                        *
*               wav file format                                         *
*                                                                       *
************************************************************************/
#include<stdio.h>
#include<stdlib.h>



void write_file(long fs, char *rawfile, char *wavfile)
{
  FILE *fpi,*fpo;
  char RIFF[]="RIFF";
  char WAVE[]="WAVE";
  char fmt_chunk[]="fmt ";
  char data_chunk[]="data";
  long file_size,rawfile_size;
  long chunk_size=16;
  long data_speed;
  short formatID=1;
  short channel=1;  /* mono:1$B!$(Bstereo:2 */
  short block_size; /* 16bit, mono => 16bit*1=2byte */
  short bit; 
  int c;

  if((fpi=fopen(rawfile,"rb"))==NULL){
    printf("can't open rawfile");
    exit(0);
  }
  if((fpo=fopen(wavfile,"wb"))==NULL){
    printf("can't open wavfile");
    exit(0);
  }

  fseek(fpi,0,SEEK_END);
  rawfile_size=ftell(fpi);
  file_size=rawfile_size+36;
  fseek(fpi,0,SEEK_SET);


  /* RIFF header */
  fwrite(RIFF,sizeof(char),4,fpo);
  /* file size */
  fwrite(&file_size,sizeof(long),1,fpo);
  /* WAVE header */
  fwrite(WAVE,sizeof(char),4,fpo);
  /* fmt chunk */
  fwrite(fmt_chunk,sizeof(char),4,fpo);
  /* chunk size */
  fwrite(&chunk_size,sizeof(long),1,fpo);
  /* formatID */
  fwrite(&formatID,sizeof(short),1,fpo);
  /* channel (mono:1$B!$(Bstereo:2) */
  fwrite(&channel,sizeof(short),1,fpo);
  /* sampling frequency */
  fwrite(&fs,sizeof(long),1,fpo);
  /* data speed */
  data_speed=fs*16/8*formatID;
  fwrite(&data_speed,sizeof(long),1,fpo);
  /* block size */
  block_size=16/8*formatID;
  fwrite(&block_size,sizeof(short),1,fpo);
  /* bit number */
  bit=16;
  fwrite(&bit,sizeof(short),1,fpo);
  /* data chunk */
  fwrite(data_chunk,sizeof(char),4,fpo);
  /* file size of data */
  fwrite(&rawfile_size,sizeof(long),1,fpo);

  while((c=fgetc(fpi))!=EOF)
    fputc(c,fpo);

  fclose(fpi);
  fclose(fpo);
}


int main(int argc,char **argv)
{
  
  if(argc != 4){
    printf("error : failed to convert raw to wav\n\n");
    printf("rawtowav : convert raw to wav\n");
    printf("usage:\n");
    printf("        rawtowav [ fs(Hz) ] [ infile ] [ outfile ]\n");
    exit(0);
  }

  write_file(atol(argv[1]),argv[2],argv[3]);

  return(0);
}