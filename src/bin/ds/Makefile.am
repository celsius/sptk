## Process this file with automake to produce Makefile.in
##
## $Id$
INCLUDES = -I$(top_srcdir)/include
bin_PROGRAMS = ds
ds_SOURCES = ds.c 
ds_LDADD = $(top_srcdir)/lib/libSPTK.a

lpcoefdir = $(DESTDIR)$(libdir)
dist_lpcoef_DATA = lpfcoef.2to1 \
	lpfcoef.3to2 \
	lpfcoef.4to3 \
	lpfcoef.5to2dn \
	lpfcoef.5to2up 
