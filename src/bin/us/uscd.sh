#!/bin/csh -f

#########################################################################
#                                                                       #
#       Sampling rate conversion from 8|10|12|16kHz to                  #
#                                                 11.025|22.05|44.1kHz  #
#                                                                       #
#                                           1998    T.Kobayashi         #
#                                           2000.7  S.Sako              #
#                                                                       #
#########################################################################

set path        = ( /usr/local/SPTK/bin $path )
set libpath     = /usr/local/SPTK/lib

set cmnd        = `basename $0`

set type        = +sf
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
                set type = ''
                breaksw
        case +s:
                set type = '+sf'
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

switch ($iext)
        case 8:
                @ osr = 4
                breaksw
        case 10:
                @ osr = 5
                breaksw
        case 12:
                @ osr = 6
                breaksw
        case 16
                @ osr = 8
                breaksw
        default
                goto usage
endsw

switch ($oext)
        case 44:
                breaksw
        case 44.1:
#               set oext = 44
                breaksw
        case 22:
                @ osr *= 2
                breaksw
        case 22.05:
#               set oext = 22
                @ osr *= 2
                breaksw
        case 11:
                if ($iext == 10 || $iext == 8) then
                        @ osr *= 4
                else
                        goto usage
                endif
                breaksw
        case 11.025:
#               set oext = 11
                if ($iext == 10 || $iext == 8) then
                        @ osr *= 4
                else
                        goto usage
                endif
                breaksw
        default:
                goto usage
endsw

if( $stdinput == 1) then
        if ($?outfile) then
                if( -e $outfile) then
                        echo2 "${cmnd}: $outfile - File exits."
                        exit 1
                endif
                x2x $type |\
                us -c ${libpath}/lpfcoef.2to3f -u 3 -d 2 |\
                us -c ${libpath}/lpfcoef.2to3s -u 3 -d 2 |\
                us -c ${libpath}/lpfcoef.5to7 -u 7 -d 5 |\
                us -c ${libpath}/lpfcoef.5to7 -u 7 -d $osr |\
                x2x +fs > $outfile
        else
                x2x $type |\
                us -c ${libpath}/lpfcoef.2to3f -u 3 -d 2 |\
                us -c ${libpath}/lpfcoef.2to3s -u 3 -d 2 |\
                us -c ${libpath}/lpfcoef.5to7 -u 7 -d 5 |\
                us -c ${libpath}/lpfcoef.5to7 -u 7 -d $osr |\
                x2x +fs
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
                x2x $type $infile |\
                us -c ${libpath}/lpfcoef.2to3f -u 3 -d 2 |\
                us -c ${libpath}/lpfcoef.2to3s -u 3 -d 2 |\
                us -c ${libpath}/lpfcoef.5to7 -u 7 -d 5 |\
                us -c ${libpath}/lpfcoef.5to7 -u 7 -d $osr |\
                x2x +fs > $outfile
        else
                x2x $type $infile |\
                us -c ${libpath}/lpfcoef.2to3f -u 3 -d 2 |\
                us -c ${libpath}/lpfcoef.2to3s -u 3 -d 2 |\
                us -c ${libpath}/lpfcoef.5to7 -u 7 -d 5 |\
                us -c ${libpath}/lpfcoef.5to7 -u 7 -d $osr |\
                x2x +fs
        endif
end
exit 0

usage:

cat2 <<EOF

 uscd - sampling rate conversion from 8|10|12|16kHz to 11.025|22.05|44.1kHz

  usage:
       $cmnd [ options ] [ infile ] [ outfile ]
       $cmnd [ options ] [ infile1 ] [ infile2 ] ... [ outdir ]

  options:
       -s s     : input sampling frequency (8,10,12,16kHz)   [10]
       -S s     : output sampling frequency 
                                      (11.025,22.05,44.1kHz) [11.025]
       +x       : data format                                [s]
                     s(short)   f(float)
       -h       : print this message
  infile:
       data sequence                                         [stdin]
  stdout:
       data sequence                                         [stdout]
  notice:

EOF
exit 1
