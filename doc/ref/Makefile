# ----------------------------------------------------------------
#       Speech Signal Processing Toolkit (SPTK): version 3.0
# 		       SPTK Working Group
# 
# 		 Department of Computer Science
# 		 Nagoya Institute of Technology
# 			      and
#   Interdisciplinary Graduate School of Science and Engineering
# 		 Tokyo Institute of Technology
# 		    Copyright (c) 1984-2000
# 		      All Rights Reserved.
# 
# Permission is hereby granted, free of charge, to use and
# distribute this software and its documentation without
# restriction, including without limitation the rights to use,
# copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of this work, and to permit persons to whom this
# work is furnished to do so, subject to the following conditions:
# 
#   1. The code must retain the above copyright notice, this list
#      of conditions and the following disclaimer.
# 
#   2. Any modifications must be clearly marked as such.
#                                                                        
# NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSITITUTE OF TECHNOLOGY,
# SPTK WORKING GROUP, AND THE CONTRIBUTORS TO THIS WORK DISCLAIM
# ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT
# SHALL NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSITITUTE OF
# TECHNOLOGY, SPTK WORKING GROUP, NOR THE CONTRIBUTORS BE LIABLE
# FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
# DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
# WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
# ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
# PERFORMANCE OF THIS SOFTWARE.
# ----------------------------------------------------------------

#
# Makefile for TeX Compiling.
#

############################################################
#### Source Files ##########################################
############################################################

SRC  = main.tex
INC  = cmndref.sty \
	acep.tex \
	acorr.tex \
	agcep.tex \
	amcep.tex \
	average.tex \
	b2mc.tex \
	bcp.tex \
	bcut.tex \
	bell.tex \
	c2acr.tex \
	c2ir.tex \
	c2sp.tex \
	ccp.tex \
	cdist.tex \
	clip.tex \
	da.tex \
	decimate.tex \
	delay.tex \
	df2.tex \
	dfs.tex \
	dmp.tex \
	ds.tex \
	echo2.tex \
	excite.tex \
	extract.tex \
	fd.tex \
	fdrw.tex \
	fft.tex \
	fftcep.tex \
	fft2.tex \
	fftr.tex \
	fftr2.tex \
	fig.tex \
	frame.tex \
	freqt.tex \
	gc2gc.tex \
	gcep.tex \
	glogsp.tex \
	glsadf.tex \
	gnorm.tex \
	grlogsp.tex \
	grpdelay.tex \
	gwave.tex \
	histogram.tex \
	ifft.tex \
	ifft2.tex \
	iglsadf.tex \
	ignorm.tex \
	imglsadf.tex \
	impulse.tex \
	imsvq.tex \
	interpolate.tex \
	ivq.tex \
	lbg.tex \
	levdur.tex \
	linear_intpl.tex \
	lmadf.tex \
	lpc.tex \
	lpc2c.tex \
	lpc2lsp.tex \
	lpc2par.tex \
	lsp2lpc.tex \
	lspcheck.tex \
	lspdf.tex \
	ltcdf.tex \
	mc2b.tex \
	mcep.tex \
	merge.tex \
	mgc2mgc.tex \
	mgc2sp.tex \
	mgcep.tex \
	mgcep2.tex \
	mglsadf.tex \
	minmax.tex \
	mlpg.tex \
	mlsadf.tex \
	msvq.tex \
	nan.tex \
	norm0.tex \
	nrand.tex \
	par2lpc.tex \
	phase.tex \
	pitch.tex \
	poledf.tex \
	psgr.tex \
	ramp.tex \
	reverse.tex \
	rmse.tex \
	rndpg.tex \
	root_pol.tex \
	sin.tex \
	snr.tex \
	sopr.tex \
	spec.tex \
	srcnv.tex \
	step.tex \
	swab.tex \
	train.tex \
	uels.tex \
	ulaw.tex \
	us.tex \
	vopr.tex \
	vq.tex \
	vstat.tex \
	vsum.tex \
	window.tex \
	x2x.tex \
	xgr.tex \
	zcross.tex \
	zerodf.tex \
	ref.tex
EPSF = fig/bcp.eps \
	fig/df2.eps \
	fig/fft2.eps \
	fig/fft2-trans.eps \
	fig/fft2-comp.eps \
	fig/fft2-quad.eps \
	fig/fftr2.eps \
	fig/g0.eps \
	fig/g1.eps \
	fig/g2.eps \
	fig/scale.eps \
	fig/fig-print1.eps \
	fig/fig-print2.eps \
	fig/grid.eps \
	fig/circle.eps \
	fig/box.eps \
	fig/clip.eps \
	fig/mark.eps \
	fig/pen-line.eps \
	fig/paint.eps \
	fig/glogsp-on.eps \
	fig/glogsp-sample.eps \
	fig/grlogsp-on.eps \
	fig/ifft.eps \
	fig/ifft2.eps \
	fig/merge.eps


############################################################
#### Intermediate Files ####################################
############################################################

AUX	= $(SRC:.tex=.aux)
IDX	= $(SRC:.tex=.idx)
IND	= $(SRC:.tex=.ind)


############################################################
#### Target File ###########################################
############################################################

DVI	= $(SRC:.tex=.dvi)
PS	= $(SRC:.tex=.ps)
PDF	= $(SRC:.tex=.pdf)
TARGET	= $(DVI)


############################################################
#### Command Definitions ###################################
############################################################

LATEX	= platex
MAKEIDX	= mendex -f
DVIPS	= dvips -f
PSPDF	= ps2pdf
DVIPDF  = dvipdfmx

RM	= rm -rf
TOUCH	= touch


############################################################
#### Suffix Rules ##########################################
############################################################

.SUFFIXES: .tex .aux .idx .ind .dvi .ps .pdf

.tex.aux:
	$(LATEX) $<
	$(RM) $*.idx $*.dvi

.tex.idx:
	$(LATEX) $<
	$(RM) $*.dvi

.idx.ind:
	mv $*.idx $*.idx.jis
	nkf -e $*.idx.jis > $*.idx
	$(MAKEIDX) $<
	mv $*.ind $*.ind.euc
	nkf -j $*.ind.euc > $*.ind

.tex.dvi:
	$(LATEX) $<
	$(LATEX) $<
	$(TOUCH) $@ $*.ind

.dvi.ps:
	$(DVIPS) $< > $@

.ps.pdf:
	$(PSPDF) $< $@

.dvi.pdf:
	$(DVIPDF) -o $@ $<

############################################################
#### Make Target File ######################################
############################################################

all: $(TARGET)

$(PS): $(DVI) $(EPSF)

$(DVI): $(AUX) $(IND) $(EPSF)

$(IND): $(AUX)

$(AUX): $(INC)

$(PDF): $(DVI) $(EPSF)

############################################################
#### Directory Cleaning ####################################
############################################################

clean:
	$(RM) *.dvi *.ps *.pdf *.aux *.toc *.lof *.lot *.log *.idx* *.ind* *.ilg *.out
	$(RM) *~ \#* $(TARGET)

logclean:
	$(RM) *.log *.ilg

