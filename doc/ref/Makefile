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
	icep.tex \
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
	root_pol.tex \
	sin.tex \
	snr.tex \
	sopr.tex \
	spec.tex \
	step.tex \
	swab.tex \
	train.tex \
	uels.tex \
	ulaw.tex \
	vopr.tex \
	vq.tex \
	vstat.tex \
	vsum.tex \
	window.tex \
	x2x.tex \
	xgr.tex \
	zcross.tex \
	zerodf.tex
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

LATEX	= jlatex
DVIPS	= dvips -f
PSPDF	= ps2pdf

RM	= rm -rf
TOUCH	= touch


############################################################
#### Suffix Rules ##########################################
############################################################

.SUFFIXES: .tex .aux .dvi .ps .pdf .bib .bbl

.tex.aux:
	$(LATEX) $(SRC)

.tex.dvi:
	$(LATEX) $(SRC)
	$(TOUCH) $(DVI)

.dvi.ps:
	$(DVIPS) $(DVI) > $(PS)

.ps.pdf:
	$(PSPDF) $(PS) $(PDF)


############################################################
#### Make Target File ######################################
############################################################

all: $(TARGET)

$(PS): $(DVI) $(EPSF)

$(DVI): $(AUX) $(EPSF)

$(AUX): $(INC)


############################################################
#### Directory Cleaning ####################################
############################################################

clean:
	$(RM) *.dvi *.ps *.pdf *.aux *.toc *.lof *.lot *.log
	$(RM) *~ \#* $(TARGET)

logclean:
	$(RM) *.log *.blg

