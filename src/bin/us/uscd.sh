#!/bin/csh -f
#  ---------------------------------------------------------------  #
#            Speech Signal Processing Toolkit (SPTK)                #
#                      SPTK Working Group                           #
#                                                                   #
#                  Department of Computer Science                   #
#                  Nagoya Institute of Technology                   #
#                               and                                 #
#   Interdisciplinary Graduate School of Science and Engineering    #
#                  Tokyo Institute of Technology                    #
#                                                                   #
#                     Copyright (c) 1984-2007                       #
#                       All Rights Reserved.                        #
#                                                                   #
#  Permission is hereby granted, free of charge, to use and         #
#  distribute this software and its documentation without           #
#  restriction, including without limitation the rights to use,     #
#  copy, modify, merge, publish, distribute, sublicense, and/or     #
#  sell copies of this work, and to permit persons to whom this     #
#  work is furnished to do so, subject to the following conditions: #
#                                                                   #
#    1. The source code must retain the above copyright notice,     #
#       this list of conditions and the following disclaimer.       #
#                                                                   #
#    2. Any modifications to the source code must be clearly        #
#       marked as such.                                             #
#                                                                   #
#    3. Redistributions in binary form must reproduce the above     #
#       copyright notice, this list of conditions and the           #
#       following disclaimer in the documentation and/or other      #
#       materials provided with the distribution.  Otherwise, one   #
#       must contact the SPTK working group.                        #
#                                                                   #
#  NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSTITUTE OF TECHNOLOGY,   #
#  SPTK WORKING GROUP, AND THE CONTRIBUTORS TO THIS WORK DISCLAIM   #
#  ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL       #
#  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT   #
#  SHALL NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSTITUTE OF         #
#  TECHNOLOGY, SPTK WORKING GROUP, NOR THE CONTRIBUTORS BE LIABLE   #
#  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY        #
#  DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,  #
#  WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTUOUS   #
#  ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR          #
#  PERFORMANCE OF THIS SOFTWARE.                                    #
#                                                                   #
#  ---------------------------------------------------------------  #
#


#########################################################################
#                                                                       #
#       Sampling rate conversion from 8|10|12|16 kHz to                 #
#                                                 11.025|22.05|44.1 kHz #
#                                                                       #
#                                           1998    T.Kobayashi         #
#                                           2000.7  S.Sako              #
#                                                                       #
#########################################################################

set path        = ( /usr/local/SPTK/bin $path )
set libpath     = /usr/local/SPTK/lib
set sptkver     = 'SPTK_VERSION'
set cvsid       = '$Id: uscd.sh,v 1.13 2007/09/15 14:43:03 heigazen Exp $'

set cmnd        = `basename $0`

set itype       = +ff
set otype       = +ff
set iext        = 10
set oext        = 11.025
set stdinput    = 1
set file        = ""

@ n = 1
@ s = 1

@ i = 0
while($i < $#argv)
        @ i++
        switch($argv[$i])
        case +f:
                set itype = '+ff'
                set otype = '+ff'
                breaksw
        case +s:
                set itype = '+sf'
                set otype = '+fs'
                breaksw
        case -s:
                @ i++
                set iext = $argv[$i]
                breaksw
        case -S:
                @ i++
                set oext = $argv[$i]
                breaksw
        case -h:
                goto usage
                breaksw
        default
                if( -d $argv[$i]) then
                        set destdir = $argv[$i]
                else if( -e $argv[$i]) then
                        set file = ( $file $argv[$i] )
                        set stdinput = -1
                else
                        if( ${#file} > 1 ) then
                            echo2 "${cmnd}: too many input files"
                            exit 1
                        endif
                        set outfile = $argv[$i]
                endif
                breaksw
        endsw
end
goto cnvt


cnvt:

if(`echo "$iext == 8" | bc -l`) then
                @ osr = 4
else if(`echo "$iext == 10" | bc -l`) then
                @ osr = 5
else if(`echo "$iext == 12" | bc -l`) then
                @ osr = 6
else if(`echo "$iext == 16" | bc -l`) then
                @ osr = 8
else
                goto usage
endif

if(`echo "$oext == 44" | bc -l`) then   
else if(`echo "$oext == 44.1" | bc -l`) then
#               set oext = 44
else if(`echo "$oext == 22" | bc -l`) then
                @ osr *= 2
else if(`echo "$oext == 22.05" | bc -l`) then
#               set oext = 22
                @ osr *= 2
else if(`echo "$oext == 11" | bc -l`) then
                if (`echo "$iext == 10 || $iext == 8" | bc -l`) then
                        @ osr *= 4
                else
                        goto usage
                endif
else if(`echo "$oext == 11.025" | bc -l`) then
#               set oext = 11
                if (`echo "$iext == 10 || $iext == 8" | bc -l`) then
                        @ osr *= 4
                else
                        goto usage
                endif
else 
                goto usage
endif

if( $stdinput == 1) then
        if ($?outfile) then
                if( -e $outfile) then
                        echo2 "${cmnd}: $outfile - File exits."
                        exit 1
                endif
                x2x $itype |\
                us -c ${libpath}/lpfcoef.2to3f -u 3 -d 2 |\
                us -c ${libpath}/lpfcoef.2to3s -u 3 -d 2 |\
                us -c ${libpath}/lpfcoef.5to7 -u 7 -d 5 |\
                us -c ${libpath}/lpfcoef.5to7 -u 7 -d $osr |\
                x2x $otype > $outfile
        else
                x2x $itype |\
                us -c ${libpath}/lpfcoef.2to3f -u 3 -d 2 |\
                us -c ${libpath}/lpfcoef.2to3s -u 3 -d 2 |\
                us -c ${libpath}/lpfcoef.5to7 -u 7 -d 5 |\
                us -c ${libpath}/lpfcoef.5to7 -u 7 -d $osr |\
                x2x $otype
        endif
        exit 0
endif

foreach infile ($file)
        if ($?destdir) then
                set outfile = $infile:t
                set outfile = {$destdir}/{$outfile:r}.$oext
        endif

        if ($?outfile) then
                if ( -e $outfile ) then
                        echo2 "${cmnd}: $outfile - File exits."
                        exit 1
                endif
                x2x $itype $infile |\
                us -c ${libpath}/lpfcoef.2to3f -u 3 -d 2 |\
                us -c ${libpath}/lpfcoef.2to3s -u 3 -d 2 |\
                us -c ${libpath}/lpfcoef.5to7 -u 7 -d 5 |\
                us -c ${libpath}/lpfcoef.5to7 -u 7 -d $osr |\
                x2x $otype > $outfile
        else
                x2x $itype $infile |\
                us -c ${libpath}/lpfcoef.2to3f -u 3 -d 2 |\
                us -c ${libpath}/lpfcoef.2to3s -u 3 -d 2 |\
                us -c ${libpath}/lpfcoef.5to7 -u 7 -d 5 |\
                us -c ${libpath}/lpfcoef.5to7 -u 7 -d $osr |\
                x2x $otype
        endif
end
exit 0

usage:

cat2 <<EOF

 uscd - up/down-sampling from 8, 10, 12, or 16 kHz to 11.025, 22.05, or 44.1 kHz

  usage:
       $cmnd [ options ] [ infile ] [ outfile ]
       $cmnd [ options ] [ infile1 ] [ infile2 ] ... [ outdir ]

  options:
       -s s     : input sampling frequency (8, 10, 12, 16 kHz) [10]
       -S s     : output sampling frequency 
                                      (11.025, 22.05, 44.1kHz) [11.025]
       +x       : input and output data format                 [f]
                     s(short)   f(float)
       -h       : print this message
  infile:
       data sequence                                           [stdin]
  stdout:
       data sequence                                           [stdout]
  notice:

  SPTK: version $sptkver
  CVS Info: $cvsid
  
EOF
exit 1
