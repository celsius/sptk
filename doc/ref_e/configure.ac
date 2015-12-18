# ----------------------------------------------------------------- #
#             The Speech Signal Processing Toolkit (SPTK)           #
#             developed by SPTK Working Group                       #
#             http://sp-tk.sourceforge.net/                         #
# ----------------------------------------------------------------- #
#                                                                   #
#  Copyright (c) 1984-2007  Tokyo Institute of Technology           #
#                           Interdisciplinary Graduate School of    #
#                           Science and Engineering                 #
#                                                                   #
#                1996-2015  Nagoya Institute of Technology          #
#                           Department of Computer Science          #
#                                                                   #
# All rights reserved.                                              #
#                                                                   #
# Redistribution and use in source and binary forms, with or        #
# without modification, are permitted provided that the following   #
# conditions are met:                                               #
#                                                                   #
# - Redistributions of source code must retain the above copyright  #
#   notice, this list of conditions and the following disclaimer.   #
# - Redistributions in binary form must reproduce the above         #
#   copyright notice, this list of conditions and the following     #
#   disclaimer in the documentation and/or other materials provided #
#   with the distribution.                                          #
# - Neither the name of the SPTK working group nor the names of its #
#   contributors may be used to endorse or promote products derived #
#   from this software without specific prior written permission.   #
#                                                                   #
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            #
# CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       #
# INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          #
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          #
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS #
# BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          #
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   #
# TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     #
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON #
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   #
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY    #
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           #
# POSSIBILITY OF SUCH DAMAGE.                                       #
# ----------------------------------------------------------------- #

# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(SPTK-Reference-Manual, 3.8, http://sourceforge.net/projects/sp-tk/)
AC_COPYRIGHT(Copyright 1984-2007 Tokyo Institute of Technology)
AC_COPYRIGHT(Copyright 1996-2014 Nagoya Institute of Technology)

# Checks for programs.
AC_PATH_PROGS([LATEX], [latex], [:], [/usr/bin:$PATH])
if test ! -f "$LATEX"; then
        AC_MSG_ERROR(Can't find latex)
fi
AC_PATH_PROGS([DVIPS], [dvips], [:], [/usr/bin:$PATH])
if test ! -f "$DVIPS"; then
        AC_MSG_ERROR(Can't find dvips)
fi
AC_PATH_PROGS([MAKEINDEX], [makeindex], [:], [/usr/bin:$PATH])
if test ! -f "$MAKEINDEX"; then
	AC_MSG_ERROR(Can't find makeindex)
fi
AC_PATH_PROGS([PS2PDF], [ps2pdf], [:], [/usr/bin:$PATH])
if test ! -f "$PS2PDF"; then
        AC_MSG_ERROR(Can't find ps2pdf)
fi
AC_PATH_PROGS([DVIPDF], [dvipdfmx dvipdfm dvipdf], [:], [/usr/bin:$PATH])
if test ! -f "$DVIPDF"; then
	AC_MSG_ERROR(Can't find dvipdf)
fi

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
