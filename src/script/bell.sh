#!/bin/csh -f

#########################################################################
#									#
#   Bell								#
#									#
#						1996.6  K.Koishida	#
#									#
#########################################################################

set path	= ( /usr/local/SPTK/bin $path )

set cmnd	= $0
set cmnd	= $cmnd:t

set no 		= 1

@ i = 0
while ($i < $#argv)
	@ i++
	switch ($argv[$i])
	case -h:
		set exit_status = 0
		goto usage
		breaksw
	default:
		@ no = $argv[$i]
		breaksw
	endsw
end
goto main

usage:
	echo2 ''
	echo2 " $cmnd - bell"
	echo2 ''
	echo2 '  usage:'
	echo2 "       $cmnd [ option ] [ num ] > stdout"
	echo2 '  option:'
	echo2 '       -h  : print this message'
	echo2 '  notice:'
	echo2 '       num : number of bell      [1]'
	echo2 ''
exit $exit_status

main:
if ( $no < 1 ) set no = 1

set i = 1
while ( $i < $no )
	echo2 -n ''
	@ i++
	sleep 1
end
echo2 -n ''

