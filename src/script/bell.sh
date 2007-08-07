#!/bin/csh -f
# ----------------------------------------------------------------
#       Speech Signal Processing Toolkit (SPTK): version 3.0
# 		       SPTK Working Group
# 
# 		 Department of Computer Science
# 		 Nagoya Institute of Technology
# 			      and
#   Interdisciplinary Graduate School of Science and Engineering
# 		 Tokyo Institute of Technology
# 		    Copyright (c) 1984-2000
# 		      All Rights Reserved.
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
#									#
#   $Id: bell.sh,v 1.3 2007/08/07 06:09:54 heigazen Exp $               #
#                                                                       #
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

