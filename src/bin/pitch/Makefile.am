## Process this file with automake to produce Makefile.in
##
## $Id$
INCLUDES = -I$(top_srcdir)/include
bin_PROGRAMS = pitch
pitch_SOURCES = pitch.c $(srcdir)/snack/jkGetF0.c $(srcdir)/snack/sigproc.c
pitch_LDADD = $(top_srcdir)/lib/libSPTK.a
