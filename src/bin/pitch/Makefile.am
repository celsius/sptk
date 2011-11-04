## Process this file with automake to produce Makefile.in
##
## $Id: Makefile.am,v 1.3 2011/11/04 05:05:33 sawada11 Exp $
INCLUDES = -I$(top_srcdir)/include
bin_PROGRAMS = pitch
pitch_SOURCES = pitch.c $(srcdir)/snack/jkGetF0.c $(srcdir)/snack/sigproc.c $(srcdir)/swipe/swipe.c $(srcdir)/swipe/vector.c
pitch_LDADD = $(top_srcdir)/lib/libSPTK.a
