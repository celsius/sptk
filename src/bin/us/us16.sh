#!/bin/csh -f

#########################################################################
#                                                                       #
#       Sampling rate conversion from 10|12kHz to 16kHz                 #
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
set oext        = 16

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
        case 10:
                set usops = "-c ${libpath}/lpfcoef.5to8 -u 8 -d 5"
                breaksw
        case 12:
                set usops = "-c ${libpath}/lpfcoef.3to4 -u 4 -d 3"
                breaksw
        default
                goto usage
endsw

if( $stdinput == 1) then
        if ($?outfile) then
                if( -e $outfile) then
                        echo2 "${cmnd}: $outfile - File exits."
                        exit 1
                endif
                x2x $type |\
                us $usops |\
                x2x +fs > $outfile
        else
                x2x $type |\
                us $usops |\
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
                us $usops |\
                x2x +fs > $outfile
        else
                x2x $type $infile |\
                us $usops |\
                x2x +fs
        endif
end
exit 0

usage:

cat2 <<EOF

 uscd - sampling rate conversion from 10|12kHz to 16kHz

  usage:
       $cmnd [ options ] [ infile ] [ outfile ]
       $cmnd [ options ] [ infile1 ] [ infile2 ] ... [ outdir ]

  options:
       -s s     : input sampling frequency 10|12kHz          [10]
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

