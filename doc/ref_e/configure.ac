# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(SPTK-Reference-Manual, 3.2, http://sourceforge.net/projects/sp-tk/)
AC_COPYRIGHT(Copyright 1984-2007 Tokyo Institute of Technology)
AC_COPYRIGHT(Copyright 1996-2008 Nagoya Institute of Technology)

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
