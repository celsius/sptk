#! /bin/csh -f
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
#   Draw a Running Log Spectrum Graph                                   #
#                                                                       #
#                                               1988.5  K.Tokuda        #
#                                               1996.6  K.Koishida      #
#                                               1998.7  M.Tamura        #
#                                                                       #
#########################################################################

set path    = ( /usr/local/SPTK/bin $path )
set sptkver = 'SPTK_VERSION'
set cvsid   = '$Id$'
set format  = 'float'

set cmnd = $0
set cmnd = $cmnd:t

set h    = 1.5
set w    = 0.25
set z    = 1
@ xo     = 95
@ yo     = 30
@ x      = 1
@ ymin   = -100
@ yscale = 100

set yy   = ("" "")
set p    = 2
set ln   = 1
set cs   = 2
@ cy     = -8
@ cyy    = -14
@ cyyy   = -20

@ g      = 0
@ s      = 0
@ l      = 256

set trans = 0
set help  = 0
set file

set xname = ( "Normalized frequency" \
              "Normalized frequency (rad)" \
              "" \
              "Frequency (kHz)" \
              "Frequency (kHz)" \
              "Frequency (kHz)" \
              "" \
              "Frequency (kHz)" \
              "" \
              "Frequency (kHz)" )
set xs = ( "0 0.25 0.5" \
           0\ \'1.57\ \"\\pi/2\"\ \'3.14\ \"\\pi\" \
           "" \
           "0 1 2 3 4" \
           "0 1 2 3 4 5" \
           "0 2 4 6" \
           "" \
           "0 2 4 6 8" \
           "" \
           "0 2 4 6 8 10" )
set xx = ("0 0.5" "0 3.14" "" "0 4" "0 5" "0 6" "" "0 8" "" "0 10")
set xl = (0.5 3.14 "" 4 5 6 "" 8 "" 10)

set on_x = (25 60 95 130 165)

@ i = 0
while ($i < $#argv)
        @ i++
        switch ($argv[$i])
        case -t:
                set trans = 1
                breaksw
        case -f:
                @ i++
                set f = $argv[$i]
                breaksw
        case -H:
                @ i++
                set h = $argv[$i]
                breaksw
        case -W:
                @ i++
                set w = $argv[$i]
                breaksw
        case -o:
                @ i++
                @ xo = $argv[$i]
                @ i++
                @ yo = $argv[$i]
                breaksw
        case -O:
                @ i++
                @ on = $argv[$i]
                breaksw
        case -x:
                @ i++
                @ x = $argv[$i]
                breaksw
        case -y:
                @ i++
                @ ymin = $argv[$i]
                breaksw
        case -yo:
                @ i++
                @ yo = $argv[$i]
                breaksw
        case -yy:
                @ i++
                @ yscale = $argv[$i]
                breaksw
        case -p:
                @ i++
                set p = $argv[$i]
                breaksw
        case -ln:
                @ i++
                set ln = $argv[$i]
                breaksw
        case -c:
                @ i++
                set c = "$argv[$i]"
                breaksw
        case -c2:
                @ i++
                set cc = "$argv[$i]"
                breaksw
        case -c3:
                @ i++
                set ccc = "$argv[$i]"
                breaksw
        case -cy:
                @ i++
                set cy = "$argv[$i]"
                breaksw
        case -cy2:
                @ i++
                set cyy = "$argv[$i]"
                breaksw
        case -cy3:
                @ i++
                set cyyy = "$argv[$i]"
                breaksw
        case -cs:
                @ i++
                set cs = "$argv[$i]"
                breaksw
        case -g:
                @ i++
                @ g = $argv[$i]
                breaksw
        case -s:
                @ i++
                set s = $argv[$i]
                breaksw
        case -e:
                @ i++
                set e = $argv[$i]
                breaksw
        case -l:
                @ i++
                @ l = $argv[$i]
                breaksw
        case -n:
                @ i++
                @ n = $argv[$i]
                breaksw
        case -z:
                @ i++
                set z = $argv[$i]
                breaksw
        case -help:
                set help = 1
        case -h:
                set exit_status = 0
                goto usage
                breaksw
        default:
                set file = $argv[$i]
                if ( ! -f $file ) then
                        echo2 "${cmnd}: Can't open file "'"'"$file"'"'" \!"
                        set exit_status = 1
                        goto usage
                endif
                breaksw
        endsw
end
goto main

usage:
        echo2 ''
        echo2 " $cmnd - draw a running log spectrum graph"
        echo2 ''
        echo2 '  usage:'
        echo2 '       grlogsp.sh [ options ] [ infile ] > stdout'
        echo2 '  options:'
        echo2 '       -t             : transpose x and y axes       [FALSE]'
        echo2 '       -O   O         : origin of graph              [1]    '
if ( $help ) then
        echo2 '                         1 ( 25, yo) <mm>   +-----------+'
        echo2 '                         2 ( 60, yo) <mm>   |           |'
        echo2 '                         3 ( 95, yo) <mm>   |           |'
        echo2 '                         4 (130, yo) <mm>   | 1 2 3 4 5 |'
        echo2 '                         5 (165, yo) <mm>   +-----------+'
        echo2 '                         (yo+100,x) <mm> if -t is specified'
endif
        echo2 '       -x   x         : x scale                      [1]'
if ( $help ) then
        echo2 '                         1  (Normalized frequency <0, 0.5>)'
        echo2 '                         2  (Normalized frequency <0, pi/2, pi>)'
        echo2 '                         4  (Frequency <0, 1, 2, 3, 4     kHz>)'
        echo2 '                         5  (Frequency <0, 1, 2, 3, 4, 5  kHz>)'
        echo2 '                         8  (Frequency <0, 2, 4, 6, 8     kHz>)'
        echo2 '                         10 (Frequency <0, 2, 4, 6, 8, 10 kHz>)'
endif
        echo2 '       -y   ymin      : y minimum                    [-100]'
        echo2 '       -yy  yy        : y scale           <dB/10mm>  [100]'
        echo2 '       -yo  yo        : y offset                     [30]'
        echo2 '       -p   p         : pen no.                      [2]'
        echo2 '       -ln  ln        : line number                  [1]'
        echo2 '       -s   s         : start frame number           [0]'
        echo2 '       -e   e         : end frame number             [EOF]'
        echo2 '       -n   n         : number of frame              [EOF]'
        echo2 '       -l   l         : frame length                 [256]'
        echo2 '       -c   "c"       : comment for the graph        [N/A]'
        echo2 '       -c2  "c2"      : comment for the graph        [N/A]'
        echo2 '       -c3  "c3"      : comment for the graph        [N/A]'
        echo2 '       -h             : print this message'
        echo2 '       -help          : print help in detail'
if ( $help ) then
        echo2 '       (level 2)'
        echo2 '       -W   W         : width of the graph  <100mm>  [0.25]'
        echo2 '       -H   H         : height of the graph <100mm>  [1.5]'
        echo2 '       -z   z         : moving width           <mm>  [1]'
        echo2 '       -o   xo yo     : origin of the graph          [30, 65]'
        echo2 '                       (if -o option exists, -O is not effective)'
        echo2 '       -g   g         : type of the graph            [0]'
        echo2 '                         0 (no frame)'
        echo2 '                         1 (half frame)'
        echo2 '                         2 (with frame)'
        echo2 '       -cy  cy        : first comment position       [-8]'
        echo2 '       -cy2 cy2       : second comment position      [-14]'
        echo2 '       -cy3 cy3       : third comment position       [-20]'
        echo2 '       -cs  cs        : comment size                 [1]'
        echo2 '       -f   f         : additional data file for fig [NULL]'
endif
        echo2 '  infile:'
        echo2 "       log spectrum ($format)                          [stdin]"
        echo2 '  stdout:'
        echo2 '       XY-plotter command'
        echo2 "  SPTK: version $sptkver"
        echo2 "  CVS Info: $cvsid"
        echo2 ''
exit 0

main:
if ($?on) then
        @ xo = $on_x[$on]
endif

@ yy[1] = $ymin
@ yy[2] = $yy[1] + $yscale * 10

set ww = $w
if( $trans ) then
        set T = "-t"
        @ xxo = $xo
        @ xo = $yo
        @ yo = $xxo
        @ yo = $yo + 100
        set w = $h
        set h = -$ww
else
        set T = ""
endif


if ($?ccc) then
        if ($trans ) then
                @ xoc = $xo + $cyyy - 5
                @ yoc = $yo
        else
                @ xoc = $xo
                @ yoc = $yo + $cyyy
        endif
        fig $T -W $w -H $h -o $xoc $yoc -g 0 << EOF
                csize $cs
                xname "$ccc"
EOF
endif
if ($?cc) then
        if ($trans ) then
                @ xoc = $xo + $cyy - 5
                @ yoc = $yo
        else
                @ xoc = $xo
                @ yoc = $yo + $cyy
        endif
        fig $T -W $w -H $h -o $xoc $yoc -g 0 << EOF
                csize $cs
                xname "$cc"
EOF
endif
if ($?c) then
        if ($trans ) then
                @ xoc = $xo + $cy - 5
                @ yoc = $yo
        else
                @ xoc = $xo
                @ yoc = $yo + $cy
        endif
        fig $T -W $w -H $h -o $xoc $yoc -g 0 << EOF
                csize $cs
                xname "$c"
EOF
endif

fig $T -W $w -H $h -o $xo $yo -g $g << EOF
        x        $xx[$x] 
        xscale        $xs[$x] 
        xname "$xname[$x]"
EOF

if ($?f) then
        fig $f -W $w -H $h -o $xo $yo -g 0
endif

@ lhi = $l / 2 + 1
@ s = $s * $lhi

if ($?n) then
        @ e = $s + $n * $lhi - 1
        set ee = "-e $e"
else if ($?e) then
        @ e = $e * $lhi - 1
        if( $e < $s ) then
                echo2 "${cmnd}: invalid start or end frame number"
                exit 1
        endif
        set ee = "-e $e"
else
        set ee = ""
endif

if ($trans) set T = "-t -1"

if ( $format == 'float' ) then
        bcut -s $s $ee +f $file|\
        fdrw $T -W $ww -o $xo $yo -g 0 -y $yy -n $lhi -p $p -m $ln -z $z
else
        bcut -s $s $ee +d $file|\
        fdrw $T -W $ww -o $xo $yo -g 0 -y $yy -n $lhi -p $p -m $ln -z $z
endif
