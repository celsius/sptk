#!/bin/csh -f

#########################################################################
#                                                                       #
#   play 16-bit linear PCM                                              #
#                                                                       #
#                                                                       #
#                                               2000.4  S.Sako          #
#                                                                       #
#########################################################################

set path        = ( /usr/local/SPTK/bin $path )

set cmnd	= `basename $0`

set file	= ""
set stdinput    = 1

set arch	= `uname -m`

set freq	= 10
set ampl	= 50
set gain	= 0
set ftype	= s
set port	= s
set headersize  = 0
set verbosemode	= 0

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
		goto cnvt44
		breaksw
	endsw
exit 0


usage:

cat <<EOF

 daplay - play 16-bit linear PCM data

  usage:
       daplay [ options ] [ infile1 ] [ infile2 ] ...
EOF

switch($arch)
	case sun4*:
cat <<EOF
  options:
       -s s  : sampling frequency (8,10,11,12,16,20,22,32,44,48 kHz) [10]
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

EOF
		exit 1

 	case i?86:
cat <<EOF
  options:
       -s s  : sampling frequency (8,10,11,12,16,20,22,44 kHz)   [10]
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

EOF
		exit 1
endsw

endif


cnvt48:

if ( $stdinput == 1 ) then
	set infile	= ""
	goto stdin48
endif

foreach infile ( ${file} )
	if ( ! -f $infile ) then
		echo2 "${cmnd}: Can't open file "'"'"$infile"'"'" \!"
		break
	endif
	if ( $verbosemode ) then
		echo "${cmnd}: ${infile}"
	endif

	stdin48:
	switch ($freq)
		case 8:
			fileconvert $infile |\
				da $daoption[*] +f -s 8
			breaksw
		case 10:
			fileconvert $infile |\
				us -s 58 -d 5 -u 8 |\
				da $daoption[*] +f -s 16
			breaksw
		case 11:
			fileconvert $infile |\
				da $daoption[*] +f -s 11
			breaksw
		case 12:
			fileconvert $infile |\
				us -s 34 -d 3 -u 4 |\
				da $daoption[*] +f -s 16
			breaksw
		case 16:
			fileconvert $infile |\
				da $daoption[*] +f -s 16
			breaksw
		case 20:
			fileconvert $infile |\
				ds -s 54 |\
				da $daoption[*] +f -s 16
			breaksw
		case 22:
			fileconvert $infile |\
				da $daoption[*] +f -s 22
			breaksw
		case 32:
			fileconvert $infile |\
				da $daoption[*] +f -s 32
			breaksw
		case 44:
			fileconvert $infile |\
				da $daoption[*] +f -s 44
			breaksw
		case 48:
			fileconvert $infile |\
				da $daoption[*] +f -s 48
			breaksw
		default
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
	case 11:
		@ osr = -1
		breaksw
	case 12:
		@ osr = 6
		breaksw
	case 16
		@ osr = 8
		breaksw
	case 20:
		@ osr = 10
		breaksw
	case 22:
		@ osr = -1
		breaksw
	case 44:
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
		echo "${cmnd}: ${infile}"
	endif

	stdin44:
	if( $osr > 0 ) then
		fileconvert $infile |\
			us -s 23F -u3 -d2 |\
			us -s 23S -u3 -d2 |\
			us -s 57 -u7 -d5 |\
			us -s 57 -u7 -d$osr |\
			da $daoption[*] +f -s 22
	else
		fileconvert $infile |\
			da $daoption[*] +f -s $freq
	endif	
	if( $stdinput == 1 ) then
		exit 0
	endif    	
end
exit 0

