#!/bin/tcsh -f

#onintr CLEAR

set TODAY	= `date +%y%m%d`
set BASEDIR	= ..
set MKDISTDIR	= .
set SNAPDIR	= $MKDISTDIR/snapshot
set LIST	= $MKDISTDIR/list.all
set ARCHIVE	= SPTK-snap-$TODAY
set DOCUMENT	= SPTK-doc-snap-$TODAY
set EDOCUMENT	= SPTK-doc_e-snap-$TODAY

if ( ! -d $SNAPDIR ) then
	rm -rf $SNAPDIR
	mkdir -p $SNAPDIR
endif

if ( -f $ARCHIVE.tar.gz ) mv $ARCHIVE.tar.gz{,.old}
if ( -f $DOCUMENT.tar.gz ) mv $DOCUMENT.tar.gz{,.old}
if ( -f $EDOCUMENT.tar.gz ) mv $EDOCUMENT.tar.gz{,.old}

rm -rf $ARCHIVE $DOCUMENT $EDOCUMENT

make -f $MKDISTDIR/Makefile SOURCEDIR=$BASEDIR TARGETDIR=$ARCHIVE `cat $LIST` \
  && tar cfz $ARCHIVE.tar.gz $ARCHIVE \
  && rm -rf $ARCHIVE
if ( -f $ARCHIVE.tar.gz ) mv $ARCHIVE.tar.gz $SNAPDIR

$MKDISTDIR/cp-tex.pl -v -l $LIST -d $DOCUMENT \
  && tar cfz $DOCUMENT.tar.gz $DOCUMENT \
  && rm -rf $DOCUMENT
if ( -f $DOCUMENT.tar.gz ) mv $DOCUMENT.tar.gz $SNAPDIR

$MKDISTDIR/cp-tex-e.pl -v -l $LIST -d $EDOCUMENT \
  && tar cfz $EDOCUMENT.tar.gz $EDOCUMENT \
  && rm -rf $EDOCUMENT
if ( -f $EDOCUMENT.tar.gz ) mv $EDOCUMENT.tar.gz $SNAPDIR

goto END

CLEAR:
rm -rf $ARCHIVE $DOCUMENT $EDOCUMENT

END:

