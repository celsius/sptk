#!/bin/tcsh -f

#onintr CLEAR

set TODAY	= `date +%y%m%d`
set BASEDIR	= /usr/local/SPTK
set MKDISTDIR	= $BASEDIR/makedist
set SRCDIR	= $BASEDIR/src
set ARCHIVEDIR	= $BASEDIR/release
set LIST	= $MKDISTDIR/list.release

set VERSION	= `cat $MKDISTDIR/VERSION`
set MAJ_VER	= $VERSION:r
set MIN_VER	= $VERSION:e
if ( x"$MIN_VER" == x ) then
	set MIN_VER = 0
else
	@ MIN_VER++
endif
set VERSION = $MAJ_VER.$MIN_VER

set ARCHIVE	= SPTK-$VERSION
set DOCUMENT	= SPTKref-$VERSION

cd $ARCHIVEDIR

if ( -f $ARCHIVE.tar.gz ) mv $ARCHIVE.tar.gz{,.old}
if ( -f $DOCUMENT.tar.gz ) mv $DOCUMENT.tar.gz{,.old}

rm -rf $ARCHIVE $DOCUMENT

make -f $MKDISTDIR/Makefile TARGETDIR=$ARCHIVE `cat $LIST` \
  && tar cfz $ARCHIVE.tar.gz $ARCHIVE \
  && rm -rf $ARCHIVE

$MKDISTDIR/cp-tex.pl -v -l $LIST -d $DOCUMENT \
  && tar cfz $DOCUMENT.tar.gz $DOCUMENT \
  && rm -rf $DOCUMENT

goto END

CLEAR:
rm -rf $ARCHIVE $DOCUMENT

END:
echo $VERSION >! $MKDISTDIR/VERSION

