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

set cmnd	= $0
set file	= -1

set arch	= `uname -m`

set freq	= 10
set ampl	= 50
set gain	= 0
set ftype	= f
set port	= h
set headersize  = 0
set verbosemode	= 0

set byteswap    = -1

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
	   	breaksw
	case -a:
		@ i++
		set ampl = $argv[$i]
		breaksw
	case -g:
		@ i++
		set gain = $argv[$i]
		breaksw
	case -H:
		@ i++
		set headersize = $argv[$i]
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
		set file = $i
		break
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
	default
		breaksw
endsw      
exit 0


usage:
cat <<EOF

 daplay - play 16-bit linear PCM data

  usage:
       daplay [options ] infile1 infile2 ...
  options:
       -s s  : sampling frequency (8,10,12,16,20 kHz)  [10]
       -g g  : gain (..,-2,-1,0,1,2,...)               [0]
       -a a  : amplitude gain (0..100)                 [N/A]
       -o o  : output port                             [0]
                  s(speaker)   h(headphone)
       -H H  : header size in byte                     [0]
       -v    : display filename                        [FALSE]
       -w    : execute byte swap                       [FALSE]
       +x    : data format                             [s]
                  s(short)   f(float)
       -h    : print this message
  infile:
       data sequence                                   [stdin]
  notice:

EOF
exit 1
endif


cnvt48:
if( $file < 0) then
	set infile = ""
	goto stdin48
endif
set daops	= "-g $gain -a $ampl -o $port +f "
foreach infile ($argv[${file}-$#argv])
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
				da $daops -s 8
			breaksw
		case 10:
			fileconvert $infile |\
				srcnv -s 58 -d 5 -u 8 |\
				da $daops -s 16
			breaksw
		case 12:
			fileconvert $infile |\
				srcnv -s 34 -d 3 -u 4 |\
				da $daops -s 16
			breaksw
		case 16:
			fileconvert $infile $byteswap|\
				da $daops -s 16
			breaksw
		case 20:
			fileconvert $infile |\
				ds -s 54 |\
				da $daops -s 16
			breaksw
		default
			goto usage
	endsw
	if( $file < 0) then
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
	case 16
		@ osr = 8
		breaksw
	case 20:
		@ osr = 10
		breaksw
	default
		goto usage
endsw

@ osr *= 2

if( $file < 0) then
	set infile = ""
	goto stdin44
endif

foreach infile ($argv[${file}-$#argv])
	if ( ! -f $infile ) then
		echo2 "${cmnd}: Can't open file "'"'"$file"'"'" \!"
		break
	endif
	if ( $verbosemode ) then
		echo "${cmd}: ${infile}"
	endif

	stdin44:
	fileconvert $infile |\
		srcnv -s 23F -u3 -d2 |\
		srcnv -s 23S -u3 -d2 |\
		srcnv -s 57 -u7 -d5 |\
		srcnv -s 57 -u7 -d$osr |\
		da -H $headersize -g $gain -a $ampl +f -s 22
	if( $file < 0) then
		exit 0
	endif
end
exit 0
