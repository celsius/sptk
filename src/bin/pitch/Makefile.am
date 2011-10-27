## Process this file with automake to produce Makefile.in
##
## $Id: Makefile.am,v 1.2 2011/10/27 13:19:02 mataki Exp $
INCLUDES = -I$(top_srcdir)/include
bin_PROGRAMS = pitch
pitch_SOURCES = pitch.c $(srcdir)/snack/jkGetF0.c $(srcdir)/snack/sigproc.c
pitch_LDADD = $(top_srcdir)/lib/libSPTK.a
