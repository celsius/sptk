#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ(2.59)
AC_INIT(SPTK, 3.1, dsp-cmnd@sp.nitech.ac.jp)
AC_COPYRIGHT(Copyright 1984-2007 Nagoya Institute of Technology and Tokyo Institute of Technology)
AC_SUBST(PACKAGE_NAME)
AC_SUBST(PACKAGE_VERSION)


# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_RANLIB
AN_MAKEVAR([AR], [AC_PROG_AR])
AN_PROGRAM([ar], [AC_PROG_AR])
AC_DEFUN([AC_PROG_AR], [AC_CHECK_TOOL(AR, ar, :)])
AC_PROG_AR


# Checks for libraries.
AC_CHECK_LIB([X11], [main])
AC_CHECK_LIB([m], [main])


# Checks for X Window Systems
AC_PATH_X
AC_SUBST(x_includes)
AC_SUBST(x_libraries)


# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h limits.h stdlib.h string.h strings.h sys/ioctl.h])


# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T


# Checks for library functions.
AC_PROG_GCC_TRADITIONAL
AC_FUNC_MALLOC
AC_FUNC_STRTOD
AC_CHECK_FUNCS([bzero memset pow sqrt strrchr])


# Checks for OS
AC_CANONICAL_TARGET
case "$target_os" in
   *linux*)
	DA_OSTYPE=LINUX
   ;;
   *solaris*)
   	DA_OSTYPE=SOLARIS
   ;;
   *sun*)
   	DA_OSTYPE=SUNOS
   ;;
   *freebsd*)
   	DA_OSTYPE=FreeBSD
   ;;
esac
AC_SUBST(DA_OSTYPE)


# Checks for double 
AC_ARG_VAR([DOUBLE],[use double precision instead of single precision for floating point operations])
if test -z $DOUBLE; then
	DOUBLE=0
fi
if test "$DOUBLE" -ne 0; then
	DDOUBLE=-DDOUBLE
fi
AC_SUBST(DDOUBLE)

AC_CONFIG_FILES([Makefile
                 bin/Makefile
                 bin/acep/Makefile
                 bin/acorr/Makefile
                 bin/agcep/Makefile
                 bin/amcep/Makefile
                 bin/average/Makefile
                 bin/b2mc/Makefile
                 bin/bcp/Makefile
                 bin/bcut/Makefile
                 bin/c2acr/Makefile
                 bin/c2ir/Makefile
                 bin/c2sp/Makefile
                 bin/cat2/Makefile
                 bin/cdist/Makefile
                 bin/clip/Makefile
                 bin/da/Makefile
                 bin/decimate/Makefile
                 bin/delay/Makefile
                 bin/df2/Makefile
                 bin/dfs/Makefile
                 bin/dmp/Makefile
                 bin/ds/Makefile
                 bin/echo2/Makefile
                 bin/excite/Makefile
                 bin/extract/Makefile
                 bin/fd/Makefile
                 bin/fft/Makefile
                 bin/fft2/Makefile
                 bin/fftcep/Makefile
                 bin/fftr/Makefile
                 bin/fftr2/Makefile
                 bin/fig+fdrw/Makefile
                 bin/frame/Makefile
                 bin/freqt/Makefile
                 bin/gc2gc/Makefile
                 bin/gcep/Makefile
                 bin/glsadf/Makefile
                 bin/gnorm/Makefile
                 bin/grpdelay/Makefile
                 bin/histogram/Makefile
                 bin/ifft/Makefile
                 bin/ifft2/Makefile
                 bin/iglsadf/Makefile
                 bin/ignorm/Makefile
                 bin/imglsadf/Makefile
                 bin/impulse/Makefile
                 bin/imsvq/Makefile
                 bin/interpolate/Makefile
                 bin/ivq/Makefile
                 bin/lbg/Makefile
                 bin/levdur/Makefile
                 bin/linear_intpl/Makefile
                 bin/lmadf/Makefile
                 bin/lpc/Makefile
                 bin/lpc2c/Makefile
                 bin/lpc2lsp/Makefile
                 bin/lpc2par/Makefile
                 bin/lsp2lpc/Makefile
                 bin/lspcheck/Makefile
                 bin/lspdf/Makefile
                 bin/ltcdf/Makefile
                 bin/mc2b/Makefile
                 bin/mcep/Makefile
                 bin/merge/Makefile
                 bin/mgc2mgc/Makefile
                 bin/mgc2sp/Makefile
                 bin/mgcep/Makefile
                 bin/mglsadf/Makefile
                 bin/minmax/Makefile
                 bin/mlpg/Makefile
                 bin/mlsadf/Makefile
                 bin/msvq/Makefile
                 bin/nan/Makefile
                 bin/norm0/Makefile
                 bin/nrand/Makefile
                 bin/par2lpc/Makefile
                 bin/phase/Makefile
                 bin/pitch/Makefile
                 bin/poledf/Makefile
                 bin/psgr/Makefile
                 bin/ramp/Makefile
                 bin/reverse/Makefile
                 bin/rmse/Makefile
                 bin/root_pol/Makefile
                 bin/sin/Makefile
                 bin/smcep/Makefile
                 bin/snr/Makefile
                 bin/sopr/Makefile
                 bin/sp2mgc/Makefile
                 bin/spec/Makefile
                 bin/step/Makefile
                 bin/swab/Makefile
                 bin/train/Makefile
                 bin/uels/Makefile
                 bin/ulaw/Makefile
                 bin/us/Makefile
                 bin/vopr/Makefile
                 bin/vq/Makefile
                 bin/vstat/Makefile
                 bin/vsum/Makefile
                 bin/window/Makefile
                 bin/x2x/Makefile
                 bin/xgr/Makefile
                 bin/zcross/Makefile
                 bin/zerodf/Makefile
                 include/Makefile
                 lib/Makefile
                 lib/Makefile.lib
                 script/Makefile])
AC_OUTPUT

