## Process this file with automake to produce Makefile.in
##
## $Id$
lpcoefdir = $(datarootdir)/SPTK

bin_PROGRAMS = ds
ds_SOURCES = ds.c 
ds_LDADD = $(top_builddir)/lib/libSPTK.a
ds_CPPFLAGS = -DLIB=\"$(lpcoefdir)\" -I$(top_srcdir)/include

dist_lpcoef_DATA = lpfcoef.2to1 \
	lpfcoef.3to2 \
	lpfcoef.4to3 \
	lpfcoef.5to2dn \
	lpfcoef.5to2up \
	titech/lpfcoef.7to4
