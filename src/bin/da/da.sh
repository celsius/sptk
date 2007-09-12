#!/bin/csh -f
# ----------------------------------------------------------------
#       Speech Signal Processing Toolkit (SPTK): version 3.0
#                        SPTK Working Group
# 
#                  Department of Computer Science
#                  Nagoya Institute of Technology
#                               and
#   Interdisciplinary Graduate School of Science and Engineering
#                  Tokyo Institute of Technology
#                     Copyright (c) 1984-2000
#                       All Rights Reserved.
# 
# Permission is hereby granted, free of charge, to use and
# distribute this software and its documentation without
# restriction, including without limitation the rights to use,
# copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of this work, and to permit persons to whom this
# work is furnished to do so, subject to the following conditions:
# 
#   1. The code must retain the above copyright notice, this list
#      of conditions and the following disclaimer.
# 
#   2. Any modifications must be clearly marked as such.
#                                                                        
# NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSITITUTE OF TECHNOLOGY,
# SPTK WORKING GROUP, AND THE CONTRIBUTORS TO THIS WORK DISCLAIM
# ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT
# SHALL NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSITITUTE OF
# TECHNOLOGY, SPTK WORKING GROUP, NOR THE CONTRIBUTORS BE LIABLE
# FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
# DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
# WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
# ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
# PERFORMANCE OF THIS SOFTWARE.
# ----------------------------------------------------------------


#########################################################################
#                                                                       #
#   play 16-bit linear PCM                                              #
#                                                                       #
#                                               2000.4  S.Sako          #
#                                                                       #
#########################################################################

set path        = ( /usr/local/SPTK/bin $path )
set libpath     = /usr/local/SPTK/lib
set cmnd        = `basename $0`
set sptkver     = 'SPTK_VERSION'
set cvsid       = '$Id: da.sh,v 1.8 2007/09/12 06:20:43 heigazen Exp $'

set file        = ""
set stdinput    = 1

set arch        = `uname -m`

set freq        = 10
set ampl        = 50
set gain        = 0
set ftype        = s
set port        = s
set headersize  = 0
set verbosemode = 0

set byteswap    = -1
set daoption    = ""

@ i = 0
while ($i < $#argv)
        @ i++
        switch ($argv[$i])
        case -s:
                @ i++
                set freq = $argv[$i]
                breaksw
        case -o:
                @ i++
                set port = $argv[$i]
                set daoption = ( $daoption -o $port )
                   breaksw
        case -a:
                @ i++
                set ampl = $argv[$i]
                set daoption = ( $daoption -a $ampl )
                breaksw
        case -g:
                @ i++
                set gain = $argv[$i]
                set daoption = ( $daoption -a $gain )
                breaksw
        case -H:
                @ i++
                set headersize = $argv[$i]
                set daoption = ( $daoption -H $headersize )
                breaksw
        case -v:
                set verbosemode = 1
                breaksw
        case -w:
                set byteswap = 1
                breaksw
        case +s:
                set ftype = s
                breaksw
        case +f:
                set ftype = f
                breaksw
        case -h:
                set exit_status = 0
                goto usage
        default
                set file = ( $file $argv[$i] )
                set stdinput = -1
                breaksw
        endsw
end
   
if ( $ftype == "f" ) then
        if ( $byteswap > 0 ) then
                alias fileconvert 'swab +f \!^'
        else
                alias fileconvert 'cat \!^'
        endif
else
        if( $byteswap > 0 ) then
                alias fileconvert 'swab +s \!^ | x2x +sf'
        else
                alias fileconvert 'x2x +sf \!^'
        endif
endif

switch ($arch)
        case sun4*:
                goto cnvt48
                breaksw
        case i?86:
        case x86_64:
                goto cnvt44
                breaksw
        endsw
exit 0


usage:

cat2 <<EOF

 da - play 16-bit linear PCM data

  usage:
       da [ options ] [ infile1 ] [ infile2 ] ...
EOF

switch($arch)
        case sun4*:
                cat2 <<EOF
  options:
       -s s  : sampling frequency ( 8, 10, 11.025, 12, 16, 20, 22,
                                       22.05, 32, 44.1, 48 kHz)   [10]
       -g g  : gain (..,-2,-1,0,1,2,...)                          [0]
       -a a  : amplitude gain (0..100)                            [N/A]
       -o o  : output port                                        [s]
                  s(speaker)   h(headphone)
       -H H  : header size in byte                                [0]
       -v    : display filename                                   [FALSE]
       -w    : execute byte swap                                  [FALSE]
       +x    : data format                                        [s]
                  s(short)   f(float)
       -h    : print this message
  infile:
       data sequence                                              [stdin]
  notice:

  SPTK: version $sptkver
  CVS Info: $cvsid
  
EOF
                exit 1

        case i?86:
        case x86_64:
                cat2 <<EOF
  options:
       -s s  : sampling frequency ( 8, 10, 11.025, 12, 16, 20,
                                              22.025, 44.1 kHz)  [10]
       -g g  : gain (..,-2,-1,0,1,2,...)                         [0]
       -a a  : amplitude gain (0..100)                           [N/A]
       -H H  : header size in byte                               [0]
       -v    : display filename                                  [FALSE]
       -w    : execute byte swap                                 [FALSE]
       +x    : data format                                       [s]
                  s(short)   f(float)
       -h    : print this message
  infile:
       data sequence                                             [stdin]
  notice:

  SPTK: version $sptkver
  CVS Info: $cvsid
  
EOF
                exit 1
endsw

endif


cnvt48:

if ( $stdinput == 1 ) then
        set infile = ""
        goto stdin48
endif

foreach infile ( ${file} )
        if ( ! -f $infile ) then
                echo2 "${cmnd}: Can't open file "'"'"$infile"'"'" \!"
                break
        endif
        if ( $verbosemode ) then
                echo2 "${cmnd}: ${infile}"
        endif

        stdin48:
        switch ($freq)
                case 8:
                        fileconvert $infile |\
                        dawrite $daoption[*] +f -s 8
                        breaksw
                case 10:
                        fileconvert $infile |\
                        us -c ${libpath}/lpfcoef.5to8 -d 5 -u 8 |\
                        dawrite $daoption[*] +f -s 16
                        breaksw
                case 11:
                case 11.025:
                        fileconvert $infile |\
                        dawrite $daoption[*] +f -s 11.025
                        breaksw
                case 12:
                        fileconvert $infile |\
                        us -c ${libpath}/lpfcoef.3to4 -d 3 -u 4 |\
                        dawrite $daoption[*] +f -s 16
                        breaksw
                case 16:
                        fileconvert $infile |\
                        dawrite $daoption[*] +f -s 16
                        breaksw
                case 20:
                        fileconvert $infile |\
                        ds -s 54 |\
                        dawrite $daoption[*] +f -s 16
                        breaksw
                case 22:
                case 22.05:
                        fileconvert $infile |\
                        dawrite $daoption[*] +f -s 22.05
                        breaksw
                case 32:
                        fileconvert $infile |\
                        dawrite $daoption[*] +f -s 32
                        breaksw
                case 44:
                case 44.1:
                        fileconvert $infile |\
                        dawrite $daoption[*] +f -s 44.1
                        breaksw
                case 48:
                        fileconvert $infile |\
                        dawrite $daoption[*] +f -s 48
                        breaksw
                default
                        echo2 "${cmnd} : unavailable sampling frequency"
                        goto usage
        endsw
        if( $stdinput == 1 ) then
                exit 0
        endif            
end
exit 0

cnvt44:

switch ($freq)
        case 8:
                @ osr = 4
                breaksw
        case 10:
                @ osr = 5
                breaksw
        case 12:
                @ osr = 6
                breaksw
        case 16:
                @ osr = 8
                breaksw
        case 20:
                @ osr = 10
                breaksw
        case 11:
        case 11.025:
        case 22:
        case 22.05:
        case 44:
        case 44.1:
                @ osr = -1
                breaksw
        default
                goto usage
endsw

@ osr *= 2

if( $stdinput == 1 ) then
        set infile = ""
        goto stdin44
endif

foreach infile ($file)
        if ( ! -f $infile ) then
                echo2 "${cmnd}: Can't open file "'"'"$file"'"'" \!"
                break
        endif
        if ( $verbosemode ) then
                echo2 "${cmnd}: ${infile}"
        endif

        stdin44:
        if( $osr > 0 ) then
                fileconvert $infile |\
                us -c ${libpath}/lpfcoef.2to3f -u3 -d2 |\
                us -c ${libpath}/lpfcoef.2to3s -u3 -d2 |\
                us -c ${libpath}/lpfcoef.5to7 -u7 -d5 |\
                us -c ${libpath}/lpfcoef.5to7 -u7 -d$osr |\
                dawrite $daoption[*] +f -s 22.05
        else
                fileconvert $infile |\
                dawrite $daoption[*] +f -s $freq
        endif        
        if( $stdinput == 1 ) then
                exit 0
        endif            
end
exit 0

