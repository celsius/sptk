
SOURCEDIR = /usr/local/SPTK
TARGETDIR = SPTK

CP = cp -p

documents:
	mkdir -p $(TARGETDIR)
	$(CP) $(SOURCEDIR)/README $(TARGETDIR)/README
	$(CP) $(SOURCEDIR)/README.jp $(TARGETDIR)/README.jp

makefiles: documents
	mkdir -p $(TARGETDIR)/src/bin
	$(CP) $(SOURCEDIR)/src/Makefile $(TARGETDIR)/src/Makefile
	$(CP) $(SOURCEDIR)/src/bin/Makefile $(TARGETDIR)/src/bin/Makefile

acep: makefiles lib include lmadf
	mkdir -p $(TARGETDIR)/src/bin/acep
	$(CP) $(SOURCEDIR)/src/bin/acep/Makefile $(TARGETDIR)/src/bin/acep/Makefile
	$(CP) $(SOURCEDIR)/src/bin/acep/_acep.c $(TARGETDIR)/src/bin/acep/_acep.c
	$(CP) $(SOURCEDIR)/src/bin/acep/acep.c $(TARGETDIR)/src/bin/acep/acep.c

acorr: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/acorr
	$(CP) $(SOURCEDIR)/src/bin/acorr/Makefile $(TARGETDIR)/src/bin/acorr/Makefile
	$(CP) $(SOURCEDIR)/src/bin/acorr/_acorr.c $(TARGETDIR)/src/bin/acorr/_acorr.c
	$(CP) $(SOURCEDIR)/src/bin/acorr/acorr.c $(TARGETDIR)/src/bin/acorr/acorr.c

agcep: makefiles lib include iglsadf ignorm
	mkdir -p $(TARGETDIR)/src/bin/agcep
	$(CP) $(SOURCEDIR)/src/bin/agcep/Makefile $(TARGETDIR)/src/bin/agcep/Makefile
	$(CP) $(SOURCEDIR)/src/bin/agcep/_agcep.c $(TARGETDIR)/src/bin/agcep/_agcep.c
	$(CP) $(SOURCEDIR)/src/bin/agcep/agcep.c $(TARGETDIR)/src/bin/agcep/agcep.c

amcep: makefiles lib include mlsadf b2mc
	mkdir -p $(TARGETDIR)/src/bin/amcep
	$(CP) $(SOURCEDIR)/src/bin/amcep/Makefile $(TARGETDIR)/src/bin/amcep/Makefile
	$(CP) $(SOURCEDIR)/src/bin/amcep/_amcep.c $(TARGETDIR)/src/bin/amcep/_amcep.c
	$(CP) $(SOURCEDIR)/src/bin/amcep/amcep.c $(TARGETDIR)/src/bin/amcep/amcep.c

average: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/average
	$(CP) $(SOURCEDIR)/src/bin/average/Makefile $(TARGETDIR)/src/bin/average/Makefile
	$(CP) $(SOURCEDIR)/src/bin/average/_average.c $(TARGETDIR)/src/bin/average/_average.c
	$(CP) $(SOURCEDIR)/src/bin/average/average.c $(TARGETDIR)/src/bin/average/average.c

b2mc: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/b2mc
	$(CP) $(SOURCEDIR)/src/bin/b2mc/Makefile $(TARGETDIR)/src/bin/b2mc/Makefile
	$(CP) $(SOURCEDIR)/src/bin/b2mc/_b2mc.c $(TARGETDIR)/src/bin/b2mc/_b2mc.c
	$(CP) $(SOURCEDIR)/src/bin/b2mc/b2mc.c $(TARGETDIR)/src/bin/b2mc/b2mc.c

bcp: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/bcp
	$(CP) $(SOURCEDIR)/src/bin/bcp/Makefile $(TARGETDIR)/src/bin/bcp/Makefile
	$(CP) $(SOURCEDIR)/src/bin/bcp/bcp.c $(TARGETDIR)/src/bin/bcp/bcp.c

bcut: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/bcut
	$(CP) $(SOURCEDIR)/src/bin/bcut/Makefile $(TARGETDIR)/src/bin/bcut/Makefile
	$(CP) $(SOURCEDIR)/src/bin/bcut/bcut.c $(TARGETDIR)/src/bin/bcut/bcut.c

c2acr: makefiles lib include fftr
	mkdir -p $(TARGETDIR)/src/bin/c2acr
	$(CP) $(SOURCEDIR)/src/bin/c2acr/Makefile $(TARGETDIR)/src/bin/c2acr/Makefile
	$(CP) $(SOURCEDIR)/src/bin/c2acr/_c2acr.c $(TARGETDIR)/src/bin/c2acr/_c2acr.c
	$(CP) $(SOURCEDIR)/src/bin/c2acr/c2acr.c $(TARGETDIR)/src/bin/c2acr/c2acr.c

c2ir: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/c2ir
	$(CP) $(SOURCEDIR)/src/bin/c2ir/Makefile $(TARGETDIR)/src/bin/c2ir/Makefile
	$(CP) $(SOURCEDIR)/src/bin/c2ir/_c2ir.c $(TARGETDIR)/src/bin/c2ir/_c2ir.c
	$(CP) $(SOURCEDIR)/src/bin/c2ir/c2ir.c $(TARGETDIR)/src/bin/c2ir/c2ir.c

c2sp: makefiles lib include fftr
	mkdir -p $(TARGETDIR)/src/bin/c2sp
	$(CP) $(SOURCEDIR)/src/bin/c2sp/Makefile $(TARGETDIR)/src/bin/c2sp/Makefile
	$(CP) $(SOURCEDIR)/src/bin/c2sp/_c2sp.c $(TARGETDIR)/src/bin/c2sp/_c2sp.c
	$(CP) $(SOURCEDIR)/src/bin/c2sp/c2sp.c $(TARGETDIR)/src/bin/c2sp/c2sp.c

ccp: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/ccp
	$(CP) $(SOURCEDIR)/src/bin/ccp/Makefile $(TARGETDIR)/src/bin/ccp/Makefile
	$(CP) $(SOURCEDIR)/src/bin/ccp/ccp.c $(TARGETDIR)/src/bin/ccp/ccp.c

cdist: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/cdist
	$(CP) $(SOURCEDIR)/src/bin/cdist/Makefile $(TARGETDIR)/src/bin/cdist/Makefile
	$(CP) $(SOURCEDIR)/src/bin/cdist/cdist.c $(TARGETDIR)/src/bin/cdist/cdist.c

clip: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/clip
	$(CP) $(SOURCEDIR)/src/bin/clip/Makefile $(TARGETDIR)/src/bin/clip/Makefile
	$(CP) $(SOURCEDIR)/src/bin/clip/_clip.c $(TARGETDIR)/src/bin/clip/_clip.c
	$(CP) $(SOURCEDIR)/src/bin/clip/clip.c $(TARGETDIR)/src/bin/clip/clip.c

da: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/da
	$(CP) $(SOURCEDIR)/src/bin/da/Makefile $(TARGETDIR)/src/bin/da/Makefile
	$(CP) $(SOURCEDIR)/src/bin/da/da.c $(TARGETDIR)/src/bin/da/da.c
	$(CP) $(SOURCEDIR)/src/bin/da/da.h $(TARGETDIR)/src/bin/da/da.h
	$(CP) $(SOURCEDIR)/src/bin/da/da_10.fir $(TARGETDIR)/src/bin/da/da_10.fir
	$(CP) $(SOURCEDIR)/src/bin/da/da_12.fir $(TARGETDIR)/src/bin/da/da_12.fir

decimate: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/decimate
	$(CP) $(SOURCEDIR)/src/bin/decimate/Makefile $(TARGETDIR)/src/bin/decimate/Makefile
	$(CP) $(SOURCEDIR)/src/bin/decimate/decimate.c $(TARGETDIR)/src/bin/decimate/decimate.c

delay: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/delay
	$(CP) $(SOURCEDIR)/src/bin/delay/Makefile $(TARGETDIR)/src/bin/delay/Makefile
	$(CP) $(SOURCEDIR)/src/bin/delay/delay.c $(TARGETDIR)/src/bin/delay/delay.c

df2: makefiles lib include dfs
	mkdir -p $(TARGETDIR)/src/bin/df2
	$(CP) $(SOURCEDIR)/src/bin/df2/Makefile $(TARGETDIR)/src/bin/df2/Makefile
	$(CP) $(SOURCEDIR)/src/bin/df2/_df2.c $(TARGETDIR)/src/bin/df2/_df2.c
	$(CP) $(SOURCEDIR)/src/bin/df2/df2.c $(TARGETDIR)/src/bin/df2/df2.c

dfs: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/dfs
	$(CP) $(SOURCEDIR)/src/bin/dfs/Makefile $(TARGETDIR)/src/bin/dfs/Makefile
	$(CP) $(SOURCEDIR)/src/bin/dfs/_dfs.c $(TARGETDIR)/src/bin/dfs/_dfs.c
	$(CP) $(SOURCEDIR)/src/bin/dfs/dfs.c $(TARGETDIR)/src/bin/dfs/dfs.c

dmp: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/dmp
	$(CP) $(SOURCEDIR)/src/bin/dmp/Makefile $(TARGETDIR)/src/bin/dmp/Makefile
	$(CP) $(SOURCEDIR)/src/bin/dmp/dmp.c $(TARGETDIR)/src/bin/dmp/dmp.c

ds: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/ds
	$(CP) $(SOURCEDIR)/src/bin/ds/Makefile $(TARGETDIR)/src/bin/ds/Makefile
	$(CP) $(SOURCEDIR)/src/bin/ds/ds.c $(TARGETDIR)/src/bin/ds/ds.c
	$(CP) $(SOURCEDIR)/src/bin/ds/lpfcoef.2to1 $(TARGETDIR)/src/bin/ds/lpfcoef.2to1
	$(CP) $(SOURCEDIR)/src/bin/ds/lpfcoef.4to3 $(TARGETDIR)/src/bin/ds/lpfcoef.4to3
	$(CP) $(SOURCEDIR)/src/bin/ds/lpfcoef.5to2up $(TARGETDIR)/src/bin/ds/lpfcoef.5to2up
	$(CP) $(SOURCEDIR)/src/bin/ds/lpfcoef.5to2dn $(TARGETDIR)/src/bin/ds/lpfcoef.5to2dn

echo2: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/echo2
	$(CP) $(SOURCEDIR)/src/bin/echo2/Makefile $(TARGETDIR)/src/bin/echo2/Makefile
	$(CP) $(SOURCEDIR)/src/bin/echo2/echo2.c $(TARGETDIR)/src/bin/echo2/echo2.c

excite: makefiles lib include nrand
	mkdir -p $(TARGETDIR)/src/bin/excite
	$(CP) $(SOURCEDIR)/src/bin/excite/Makefile $(TARGETDIR)/src/bin/excite/Makefile
	$(CP) $(SOURCEDIR)/src/bin/excite/excite.c $(TARGETDIR)/src/bin/excite/excite.c

extract: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/extract
	$(CP) $(SOURCEDIR)/src/bin/extract/Makefile $(TARGETDIR)/src/bin/extract/Makefile
	$(CP) $(SOURCEDIR)/src/bin/extract/extract.c $(TARGETDIR)/src/bin/extract/extract.c

fd: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/fd
	$(CP) $(SOURCEDIR)/src/bin/fd/Makefile $(TARGETDIR)/src/bin/fd/Makefile
	$(CP) $(SOURCEDIR)/src/bin/fd/fd.c $(TARGETDIR)/src/bin/fd/fd.c

fft: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/fft
	$(CP) $(SOURCEDIR)/src/bin/fft/Makefile $(TARGETDIR)/src/bin/fft/Makefile
	$(CP) $(SOURCEDIR)/src/bin/fft/_fft.c $(TARGETDIR)/src/bin/fft/_fft.c
	$(CP) $(SOURCEDIR)/src/bin/fft/fft.c $(TARGETDIR)/src/bin/fft/fft.c

fft2: makefiles lib include fft
	mkdir -p $(TARGETDIR)/src/bin/fft2
	$(CP) $(SOURCEDIR)/src/bin/fft2/Makefile $(TARGETDIR)/src/bin/fft2/Makefile
	$(CP) $(SOURCEDIR)/src/bin/fft2/_fft2.c $(TARGETDIR)/src/bin/fft2/_fft2.c
	$(CP) $(SOURCEDIR)/src/bin/fft2/fft2.c $(TARGETDIR)/src/bin/fft2/fft2.c

fftr: makefiles lib include fft
	mkdir -p $(TARGETDIR)/src/bin/fftr
	$(CP) $(SOURCEDIR)/src/bin/fftr/Makefile $(TARGETDIR)/src/bin/fftr/Makefile
	$(CP) $(SOURCEDIR)/src/bin/fftr/_fftr.c $(TARGETDIR)/src/bin/fftr/_fftr.c
	$(CP) $(SOURCEDIR)/src/bin/fftr/fftr.c $(TARGETDIR)/src/bin/fftr/fftr.c

fftr2: makefiles lib include fftr fft
	mkdir -p $(TARGETDIR)/src/bin/fftr2
	$(CP) $(SOURCEDIR)/src/bin/fftr2/Makefile $(TARGETDIR)/src/bin/fftr2/Makefile
	$(CP) $(SOURCEDIR)/src/bin/fftr2/_fftr2.c $(TARGETDIR)/src/bin/fftr2/_fftr2.c
	$(CP) $(SOURCEDIR)/src/bin/fftr2/fftr2.c $(TARGETDIR)/src/bin/fftr2/fftr2.c

fdrw: fig

fig: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/fig+fdrw
	$(CP) $(SOURCEDIR)/src/bin/fig+fdrw/Makefile $(TARGETDIR)/src/bin/fig+fdrw/Makefile
	$(CP) $(SOURCEDIR)/src/bin/fig+fdrw/fdrw.c $(TARGETDIR)/src/bin/fig+fdrw/fdrw.c
	$(CP) $(SOURCEDIR)/src/bin/fig+fdrw/fig.c $(TARGETDIR)/src/bin/fig+fdrw/fig.c
	$(CP) $(SOURCEDIR)/src/bin/fig+fdrw/fig.h $(TARGETDIR)/src/bin/fig+fdrw/fig.h
	$(CP) $(SOURCEDIR)/src/bin/fig+fdrw/fig0.c $(TARGETDIR)/src/bin/fig+fdrw/fig0.c
	$(CP) $(SOURCEDIR)/src/bin/fig+fdrw/fig1.c $(TARGETDIR)/src/bin/fig+fdrw/fig1.c
	$(CP) $(SOURCEDIR)/src/bin/fig+fdrw/plot.c $(TARGETDIR)/src/bin/fig+fdrw/plot.c
	$(CP) $(SOURCEDIR)/src/bin/fig+fdrw/plot.h $(TARGETDIR)/src/bin/fig+fdrw/plot.h
	$(CP) $(SOURCEDIR)/src/bin/fig+fdrw/plsub.c $(TARGETDIR)/src/bin/fig+fdrw/plsub.c

frame: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/frame
	$(CP) $(SOURCEDIR)/src/bin/frame/Makefile $(TARGETDIR)/src/bin/frame/Makefile
	$(CP) $(SOURCEDIR)/src/bin/frame/frame.c $(TARGETDIR)/src/bin/frame/frame.c

freqt: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/freqt
	$(CP) $(SOURCEDIR)/src/bin/freqt/Makefile $(TARGETDIR)/src/bin/freqt/Makefile
	$(CP) $(SOURCEDIR)/src/bin/freqt/_freqt.c $(TARGETDIR)/src/bin/freqt/_freqt.c
	$(CP) $(SOURCEDIR)/src/bin/freqt/freqt.c $(TARGETDIR)/src/bin/freqt/freqt.c

gc2gc: makefiles lib include gnorm ignorm
	mkdir -p $(TARGETDIR)/src/bin/gc2gc
	$(CP) $(SOURCEDIR)/src/bin/gc2gc/Makefile $(TARGETDIR)/src/bin/gc2gc/Makefile
	$(CP) $(SOURCEDIR)/src/bin/gc2gc/_gc2gc.c $(TARGETDIR)/src/bin/gc2gc/_gc2gc.c
	$(CP) $(SOURCEDIR)/src/bin/gc2gc/gc2gc.c $(TARGETDIR)/src/bin/gc2gc/gc2gc.c

gcep: makefiles lib include ignorm fftr gc2gc ifft
	mkdir -p $(TARGETDIR)/src/bin/gcep
	$(CP) $(SOURCEDIR)/src/bin/gcep/Makefile $(TARGETDIR)/src/bin/gcep/Makefile
	$(CP) $(SOURCEDIR)/src/bin/gcep/_gcep.c $(TARGETDIR)/src/bin/gcep/_gcep.c
	$(CP) $(SOURCEDIR)/src/bin/gcep/gcep.c $(TARGETDIR)/src/bin/gcep/gcep.c

glsadf: makefiles lib include gnorm poledf
	mkdir -p $(TARGETDIR)/src/bin/glsadf
	$(CP) $(SOURCEDIR)/src/bin/glsadf/Makefile $(TARGETDIR)/src/bin/glsadf/Makefile
	$(CP) $(SOURCEDIR)/src/bin/glsadf/_glsadf.c $(TARGETDIR)/src/bin/glsadf/_glsadf.c
	$(CP) $(SOURCEDIR)/src/bin/glsadf/glsadf.c $(TARGETDIR)/src/bin/glsadf/glsadf.c

gnorm: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/gnorm
	$(CP) $(SOURCEDIR)/src/bin/gnorm/Makefile $(TARGETDIR)/src/bin/gnorm/Makefile
	$(CP) $(SOURCEDIR)/src/bin/gnorm/_gnorm.c $(TARGETDIR)/src/bin/gnorm/_gnorm.c
	$(CP) $(SOURCEDIR)/src/bin/gnorm/gnorm.c $(TARGETDIR)/src/bin/gnorm/gnorm.c

grpdelay: makefiles lib include fftr
	mkdir -p $(TARGETDIR)/src/bin/grpdelay
	$(CP) $(SOURCEDIR)/src/bin/grpdelay/Makefile $(TARGETDIR)/src/bin/grpdelay/Makefile
	$(CP) $(SOURCEDIR)/src/bin/grpdelay/_grpdelay.c $(TARGETDIR)/src/bin/grpdelay/_grpdelay.c
	$(CP) $(SOURCEDIR)/src/bin/grpdelay/grpdelay.c $(TARGETDIR)/src/bin/grpdelay/grpdelay.c

histogram: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/histogram
	$(CP) $(SOURCEDIR)/src/bin/histogram/Makefile $(TARGETDIR)/src/bin/histogram/Makefile
	$(CP) $(SOURCEDIR)/src/bin/histogram/_histogram.c $(TARGETDIR)/src/bin/histogram/_histogram.c
	$(CP) $(SOURCEDIR)/src/bin/histogram/histogram.c $(TARGETDIR)/src/bin/histogram/histogram.c

icep: makefiles lib include fftr
	mkdir -p $(TARGETDIR)/src/bin/icep
	$(CP) $(SOURCEDIR)/src/bin/icep/Makefile $(TARGETDIR)/src/bin/icep/Makefile
	$(CP) $(SOURCEDIR)/src/bin/icep/_icep.c $(TARGETDIR)/src/bin/icep/_icep.c
	$(CP) $(SOURCEDIR)/src/bin/icep/icep.c $(TARGETDIR)/src/bin/icep/icep.c

ifft: makefiles lib include fft
	mkdir -p $(TARGETDIR)/src/bin/ifft
	$(CP) $(SOURCEDIR)/src/bin/ifft/Makefile $(TARGETDIR)/src/bin/ifft/Makefile
	$(CP) $(SOURCEDIR)/src/bin/ifft/_ifft.c $(TARGETDIR)/src/bin/ifft/_ifft.c
	$(CP) $(SOURCEDIR)/src/bin/ifft/ifft.c $(TARGETDIR)/src/bin/ifft/ifft.c

ifft2: makefiles lib include ifft
	mkdir -p $(TARGETDIR)/src/bin/ifft2
	$(CP) $(SOURCEDIR)/src/bin/ifft2/Makefile $(TARGETDIR)/src/bin/ifft2/Makefile
	$(CP) $(SOURCEDIR)/src/bin/ifft2/_ifft2.c $(TARGETDIR)/src/bin/ifft2/_ifft2.c
	$(CP) $(SOURCEDIR)/src/bin/ifft2/ifft2.c $(TARGETDIR)/src/bin/ifft2/ifft2.c

iglsadf: makefiles lib include gnorm zerodf
	mkdir -p $(TARGETDIR)/src/bin/iglsadf
	$(CP) $(SOURCEDIR)/src/bin/iglsadf/Makefile $(TARGETDIR)/src/bin/iglsadf/Makefile
	$(CP) $(SOURCEDIR)/src/bin/iglsadf/_iglsadf.c $(TARGETDIR)/src/bin/iglsadf/_iglsadf.c
	$(CP) $(SOURCEDIR)/src/bin/iglsadf/iglsadf.c $(TARGETDIR)/src/bin/iglsadf/iglsadf.c

ignorm: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/ignorm
	$(CP) $(SOURCEDIR)/src/bin/ignorm/Makefile $(TARGETDIR)/src/bin/ignorm/Makefile
	$(CP) $(SOURCEDIR)/src/bin/ignorm/_ignorm.c $(TARGETDIR)/src/bin/ignorm/_ignorm.c
	$(CP) $(SOURCEDIR)/src/bin/ignorm/ignorm.c $(TARGETDIR)/src/bin/ignorm/ignorm.c

imglsadf: makefiles lib include mc2b gnorm
	mkdir -p $(TARGETDIR)/src/bin/imglsadf
	$(CP) $(SOURCEDIR)/src/bin/imglsadf/Makefile $(TARGETDIR)/src/bin/imglsadf/Makefile
	$(CP) $(SOURCEDIR)/src/bin/imglsadf/_imglsadf.c $(TARGETDIR)/src/bin/imglsadf/_imglsadf.c
	$(CP) $(SOURCEDIR)/src/bin/imglsadf/imglsadf.c $(TARGETDIR)/src/bin/imglsadf/imglsadf.c

impulse: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/impulse
	$(CP) $(SOURCEDIR)/src/bin/impulse/Makefile $(TARGETDIR)/src/bin/impulse/Makefile
	$(CP) $(SOURCEDIR)/src/bin/impulse/impulse.c $(TARGETDIR)/src/bin/impulse/impulse.c

imsvq: makefiles lib include ivq
	mkdir -p $(TARGETDIR)/src/bin/imsvq
	$(CP) $(SOURCEDIR)/src/bin/imsvq/Makefile $(TARGETDIR)/src/bin/imsvq/Makefile
	$(CP) $(SOURCEDIR)/src/bin/imsvq/_imsvq.c $(TARGETDIR)/src/bin/imsvq/_imsvq.c
	$(CP) $(SOURCEDIR)/src/bin/imsvq/imsvq.c $(TARGETDIR)/src/bin/imsvq/imsvq.c

interpolate: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/interpolate
	$(CP) $(SOURCEDIR)/src/bin/interpolate/Makefile $(TARGETDIR)/src/bin/interpolate/Makefile
	$(CP) $(SOURCEDIR)/src/bin/interpolate/interpolate.c $(TARGETDIR)/src/bin/interpolate/interpolate.c

ivq: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/ivq
	$(CP) $(SOURCEDIR)/src/bin/ivq/Makefile $(TARGETDIR)/src/bin/ivq/Makefile
	$(CP) $(SOURCEDIR)/src/bin/ivq/_ivq.c $(TARGETDIR)/src/bin/ivq/_ivq.c
	$(CP) $(SOURCEDIR)/src/bin/ivq/ivq.c $(TARGETDIR)/src/bin/ivq/ivq.c

lbg: makefiles lib include average nrand vq
	mkdir -p $(TARGETDIR)/src/bin/lbg
	$(CP) $(SOURCEDIR)/src/bin/lbg/Makefile $(TARGETDIR)/src/bin/lbg/Makefile
	$(CP) $(SOURCEDIR)/src/bin/lbg/_lbg.c $(TARGETDIR)/src/bin/lbg/_lbg.c
	$(CP) $(SOURCEDIR)/src/bin/lbg/lbg.c $(TARGETDIR)/src/bin/lbg/lbg.c

levdur: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/levdur
	$(CP) $(SOURCEDIR)/src/bin/levdur/Makefile $(TARGETDIR)/src/bin/levdur/Makefile
	$(CP) $(SOURCEDIR)/src/bin/levdur/_levdur.c $(TARGETDIR)/src/bin/levdur/_levdur.c
	$(CP) $(SOURCEDIR)/src/bin/levdur/levdur.c $(TARGETDIR)/src/bin/levdur/levdur.c

linear_intpl: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/linear_intpl
	$(CP) $(SOURCEDIR)/src/bin/linear_intpl/Makefile $(TARGETDIR)/src/bin/linear_intpl/Makefile
	$(CP) $(SOURCEDIR)/src/bin/linear_intpl/linear_intpl.c $(TARGETDIR)/src/bin/linear_intpl/linear_intpl.c

lmadf: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/lmadf
	$(CP) $(SOURCEDIR)/src/bin/lmadf/Makefile $(TARGETDIR)/src/bin/lmadf/Makefile
	$(CP) $(SOURCEDIR)/src/bin/lmadf/_lmadf.c $(TARGETDIR)/src/bin/lmadf/_lmadf.c
	$(CP) $(SOURCEDIR)/src/bin/lmadf/lmadf.c $(TARGETDIR)/src/bin/lmadf/lmadf.c

lpc: makefiles lib include acorr levdur
	mkdir -p $(TARGETDIR)/src/bin/lpc
	$(CP) $(SOURCEDIR)/src/bin/lpc/Makefile $(TARGETDIR)/src/bin/lpc/Makefile
	$(CP) $(SOURCEDIR)/src/bin/lpc/_lpc.c $(TARGETDIR)/src/bin/lpc/_lpc.c
	$(CP) $(SOURCEDIR)/src/bin/lpc/lpc.c $(TARGETDIR)/src/bin/lpc/lpc.c

lpc2c: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/lpc2c
	$(CP) $(SOURCEDIR)/src/bin/lpc2c/Makefile $(TARGETDIR)/src/bin/lpc2c/Makefile
	$(CP) $(SOURCEDIR)/src/bin/lpc2c/_lpc2c.c $(TARGETDIR)/src/bin/lpc2c/_lpc2c.c
	$(CP) $(SOURCEDIR)/src/bin/lpc2c/lpc2c.c $(TARGETDIR)/src/bin/lpc2c/lpc2c.c

lpc2lsp: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/lpc2lsp
	$(CP) $(SOURCEDIR)/src/bin/lpc2lsp/Makefile $(TARGETDIR)/src/bin/lpc2lsp/Makefile
	$(CP) $(SOURCEDIR)/src/bin/lpc2lsp/_lpc2lsp.c $(TARGETDIR)/src/bin/lpc2lsp/_lpc2lsp.c
	$(CP) $(SOURCEDIR)/src/bin/lpc2lsp/lpc2lsp.c $(TARGETDIR)/src/bin/lpc2lsp/lpc2lsp.c

lpc2par: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/lpc2par
	$(CP) $(SOURCEDIR)/src/bin/lpc2par/Makefile $(TARGETDIR)/src/bin/lpc2par/Makefile
	$(CP) $(SOURCEDIR)/src/bin/lpc2par/_lpc2par.c $(TARGETDIR)/src/bin/lpc2par/_lpc2par.c
	$(CP) $(SOURCEDIR)/src/bin/lpc2par/lpc2par.c $(TARGETDIR)/src/bin/lpc2par/lpc2par.c

lsp2lpc: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/lsp2lpc
	$(CP) $(SOURCEDIR)/src/bin/lsp2lpc/Makefile $(TARGETDIR)/src/bin/lsp2lpc/Makefile
	$(CP) $(SOURCEDIR)/src/bin/lsp2lpc/_lsp2lpc.c $(TARGETDIR)/src/bin/lsp2lpc/_lsp2lpc.c
	$(CP) $(SOURCEDIR)/src/bin/lsp2lpc/lsp2lpc.c $(TARGETDIR)/src/bin/lsp2lpc/lsp2lpc.c

lspcheck: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/lspcheck
	$(CP) $(SOURCEDIR)/src/bin/lspcheck/Makefile $(TARGETDIR)/src/bin/lspcheck/Makefile
	$(CP) $(SOURCEDIR)/src/bin/lspcheck/_lspcheck.c $(TARGETDIR)/src/bin/lspcheck/_lspcheck.c
	$(CP) $(SOURCEDIR)/src/bin/lspcheck/lspcheck.c $(TARGETDIR)/src/bin/lspcheck/lspcheck.c

lspdf: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/lspdf
	$(CP) $(SOURCEDIR)/src/bin/lspdf/Makefile $(TARGETDIR)/src/bin/lspdf/Makefile
	$(CP) $(SOURCEDIR)/src/bin/lspdf/_lspdf.c $(TARGETDIR)/src/bin/lspdf/_lspdf.c
	$(CP) $(SOURCEDIR)/src/bin/lspdf/lspdf.c $(TARGETDIR)/src/bin/lspdf/lspdf.c

ltcdf: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/ltcdf
	$(CP) $(SOURCEDIR)/src/bin/ltcdf/Makefile $(TARGETDIR)/src/bin/ltcdf/Makefile
	$(CP) $(SOURCEDIR)/src/bin/ltcdf/_ltcdf.c $(TARGETDIR)/src/bin/ltcdf/_ltcdf.c
	$(CP) $(SOURCEDIR)/src/bin/ltcdf/ltcdf.c $(TARGETDIR)/src/bin/ltcdf/ltcdf.c

mc2b: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/mc2b
	$(CP) $(SOURCEDIR)/src/bin/mc2b/Makefile $(TARGETDIR)/src/bin/mc2b/Makefile
	$(CP) $(SOURCEDIR)/src/bin/mc2b/_mc2b.c $(TARGETDIR)/src/bin/mc2b/_mc2b.c
	$(CP) $(SOURCEDIR)/src/bin/mc2b/mc2b.c $(TARGETDIR)/src/bin/mc2b/mc2b.c

mcep: makefiles lib include fftr freqt
	mkdir -p $(TARGETDIR)/src/bin/mcep
	$(CP) $(SOURCEDIR)/src/bin/mcep/Makefile $(TARGETDIR)/src/bin/mcep/Makefile
	$(CP) $(SOURCEDIR)/src/bin/mcep/_mcep.c $(TARGETDIR)/src/bin/mcep/_mcep.c
	$(CP) $(SOURCEDIR)/src/bin/mcep/mcep.c $(TARGETDIR)/src/bin/mcep/mcep.c

merge: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/merge
	$(CP) $(SOURCEDIR)/src/bin/merge/Makefile $(TARGETDIR)/src/bin/merge/Makefile
	$(CP) $(SOURCEDIR)/src/bin/merge/merge.c $(TARGETDIR)/src/bin/merge/merge.c

mgc2mgc: makefiles lib include ignorm gnorm freqt gc2gc
	mkdir -p $(TARGETDIR)/src/bin/mgc2mgc
	$(CP) $(SOURCEDIR)/src/bin/mgc2mgc/Makefile $(TARGETDIR)/src/bin/mgc2mgc/Makefile
	$(CP) $(SOURCEDIR)/src/bin/mgc2mgc/_mgc2mgc.c $(TARGETDIR)/src/bin/mgc2mgc/_mgc2mgc.c
	$(CP) $(SOURCEDIR)/src/bin/mgc2mgc/mgc2mgc.c $(TARGETDIR)/src/bin/mgc2mgc/mgc2mgc.c

mgc2sp: makefiles lib include ignorm mgc2mgc c2sp
	mkdir -p $(TARGETDIR)/src/bin/mgc2sp
	$(CP) $(SOURCEDIR)/src/bin/mgc2sp/Makefile $(TARGETDIR)/src/bin/mgc2sp/Makefile
	$(CP) $(SOURCEDIR)/src/bin/mgc2sp/_mgc2sp.c $(TARGETDIR)/src/bin/mgc2sp/_mgc2sp.c
	$(CP) $(SOURCEDIR)/src/bin/mgc2sp/mgc2sp.c $(TARGETDIR)/src/bin/mgc2sp/mgc2sp.c

mgcep: makefiles lib include ignorm b2mc gnorm fftr gc2gc mc2b ifft
	mkdir -p $(TARGETDIR)/src/bin/mgcep
	$(CP) $(SOURCEDIR)/src/bin/mgcep/Makefile $(TARGETDIR)/src/bin/mgcep/Makefile
	$(CP) $(SOURCEDIR)/src/bin/mgcep/_mgcep.c $(TARGETDIR)/src/bin/mgcep/_mgcep.c
	$(CP) $(SOURCEDIR)/src/bin/mgcep/mgcep.c $(TARGETDIR)/src/bin/mgcep/mgcep.c

mgcep2: makefiles lib include ignorm b2mc gnorm fftr gc2gc mc2b ifft
	mkdir -p $(TARGETDIR)/src/bin/mgcep2
	$(CP) $(SOURCEDIR)/src/bin/mgcep2/Makefile $(TARGETDIR)/src/bin/mgcep2/Makefile
	$(CP) $(SOURCEDIR)/src/bin/mgcep2/_mgcep2.c $(TARGETDIR)/src/bin/mgcep2/_mgcep2.c
	$(CP) $(SOURCEDIR)/src/bin/mgcep2/mgcep2.c $(TARGETDIR)/src/bin/mgcep2/mgcep2.c

mglsadf: makefiles lib include mc2b gnorm
	mkdir -p $(TARGETDIR)/src/bin/mglsadf
	$(CP) $(SOURCEDIR)/src/bin/mglsadf/Makefile $(TARGETDIR)/src/bin/mglsadf/Makefile
	$(CP) $(SOURCEDIR)/src/bin/mglsadf/_mglsadf.c $(TARGETDIR)/src/bin/mglsadf/_mglsadf.c
	$(CP) $(SOURCEDIR)/src/bin/mglsadf/mglsadf.c $(TARGETDIR)/src/bin/mglsadf/mglsadf.c

minmax: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/minmax
	$(CP) $(SOURCEDIR)/src/bin/minmax/Makefile $(TARGETDIR)/src/bin/minmax/Makefile
	$(CP) $(SOURCEDIR)/src/bin/minmax/minmax.c $(TARGETDIR)/src/bin/minmax/minmax.c

mlpg: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/mlpg
	$(CP) $(SOURCEDIR)/src/bin/mlpg/Makefile $(TARGETDIR)/src/bin/mlpg/Makefile
	$(CP) $(SOURCEDIR)/src/bin/mlpg/mlpg.c $(TARGETDIR)/src/bin/mlpg/mlpg.c

mlsadf: makefiles lib include mc2b
	mkdir -p $(TARGETDIR)/src/bin/mlsadf
	$(CP) $(SOURCEDIR)/src/bin/mlsadf/Makefile $(TARGETDIR)/src/bin/mlsadf/Makefile
	$(CP) $(SOURCEDIR)/src/bin/mlsadf/_mlsadf.c $(TARGETDIR)/src/bin/mlsadf/_mlsadf.c
	$(CP) $(SOURCEDIR)/src/bin/mlsadf/mlsadf.c $(TARGETDIR)/src/bin/mlsadf/mlsadf.c

msvq: makefiles lib include imsvq vq
	mkdir -p $(TARGETDIR)/src/bin/msvq
	$(CP) $(SOURCEDIR)/src/bin/msvq/Makefile $(TARGETDIR)/src/bin/msvq/Makefile
	$(CP) $(SOURCEDIR)/src/bin/msvq/_msvq.c $(TARGETDIR)/src/bin/msvq/_msvq.c
	$(CP) $(SOURCEDIR)/src/bin/msvq/msvq.c $(TARGETDIR)/src/bin/msvq/msvq.c

nan: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/nan
	$(CP) $(SOURCEDIR)/src/bin/nan/Makefile $(TARGETDIR)/src/bin/nan/Makefile
	$(CP) $(SOURCEDIR)/src/bin/nan/nan.c $(TARGETDIR)/src/bin/nan/nan.c

norm0: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/norm0
	$(CP) $(SOURCEDIR)/src/bin/norm0/Makefile $(TARGETDIR)/src/bin/norm0/Makefile
	$(CP) $(SOURCEDIR)/src/bin/norm0/_norm0.c $(TARGETDIR)/src/bin/norm0/_norm0.c
	$(CP) $(SOURCEDIR)/src/bin/norm0/norm0.c $(TARGETDIR)/src/bin/norm0/norm0.c

nrand: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/nrand
	$(CP) $(SOURCEDIR)/src/bin/nrand/Makefile $(TARGETDIR)/src/bin/nrand/Makefile
	$(CP) $(SOURCEDIR)/src/bin/nrand/_nrand.c $(TARGETDIR)/src/bin/nrand/_nrand.c
	$(CP) $(SOURCEDIR)/src/bin/nrand/nrand.c $(TARGETDIR)/src/bin/nrand/nrand.c

par2lpc: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/par2lpc
	$(CP) $(SOURCEDIR)/src/bin/par2lpc/Makefile $(TARGETDIR)/src/bin/par2lpc/Makefile
	$(CP) $(SOURCEDIR)/src/bin/par2lpc/_par2lpc.c $(TARGETDIR)/src/bin/par2lpc/_par2lpc.c
	$(CP) $(SOURCEDIR)/src/bin/par2lpc/par2lpc.c $(TARGETDIR)/src/bin/par2lpc/par2lpc.c

phase: makefiles lib include fftr
	mkdir -p $(TARGETDIR)/src/bin/phase
	$(CP) $(SOURCEDIR)/src/bin/phase/Makefile $(TARGETDIR)/src/bin/phase/Makefile
	$(CP) $(SOURCEDIR)/src/bin/phase/_phase.c $(TARGETDIR)/src/bin/phase/_phase.c
	$(CP) $(SOURCEDIR)/src/bin/phase/phase.c $(TARGETDIR)/src/bin/phase/phase.c

pitch: makefiles lib include fftr
	mkdir -p $(TARGETDIR)/src/bin/pitch
	$(CP) $(SOURCEDIR)/src/bin/pitch/Makefile $(TARGETDIR)/src/bin/pitch/Makefile
	$(CP) $(SOURCEDIR)/src/bin/pitch/_pitch.c $(TARGETDIR)/src/bin/pitch/_pitch.c
	$(CP) $(SOURCEDIR)/src/bin/pitch/pitch.c $(TARGETDIR)/src/bin/pitch/pitch.c

poledf: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/poledf
	$(CP) $(SOURCEDIR)/src/bin/poledf/Makefile $(TARGETDIR)/src/bin/poledf/Makefile
	$(CP) $(SOURCEDIR)/src/bin/poledf/_poledf.c $(TARGETDIR)/src/bin/poledf/_poledf.c
	$(CP) $(SOURCEDIR)/src/bin/poledf/poledf.c $(TARGETDIR)/src/bin/poledf/poledf.c

psgr: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/psgr
	$(CP) $(SOURCEDIR)/src/bin/psgr/Makefile $(TARGETDIR)/src/bin/psgr/Makefile
	$(CP) $(SOURCEDIR)/src/bin/psgr/dict.c $(TARGETDIR)/src/bin/psgr/dict.c
	$(CP) $(SOURCEDIR)/src/bin/psgr/eps.c $(TARGETDIR)/src/bin/psgr/eps.c
	$(CP) $(SOURCEDIR)/src/bin/psgr/plot.c $(TARGETDIR)/src/bin/psgr/plot.c
	$(CP) $(SOURCEDIR)/src/bin/psgr/psgr.c $(TARGETDIR)/src/bin/psgr/psgr.c
	$(CP) $(SOURCEDIR)/src/bin/psgr/psgr.h $(TARGETDIR)/src/bin/psgr/psgr.h

ramp: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/ramp
	$(CP) $(SOURCEDIR)/src/bin/ramp/Makefile $(TARGETDIR)/src/bin/ramp/Makefile
	$(CP) $(SOURCEDIR)/src/bin/ramp/ramp.c $(TARGETDIR)/src/bin/ramp/ramp.c

reverse: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/reverse
	$(CP) $(SOURCEDIR)/src/bin/reverse/Makefile $(TARGETDIR)/src/bin/reverse/Makefile
	$(CP) $(SOURCEDIR)/src/bin/reverse/_reverse.c $(TARGETDIR)/src/bin/reverse/_reverse.c
	$(CP) $(SOURCEDIR)/src/bin/reverse/reverse.c $(TARGETDIR)/src/bin/reverse/reverse.c

rmse: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/rmse
	$(CP) $(SOURCEDIR)/src/bin/rmse/Makefile $(TARGETDIR)/src/bin/rmse/Makefile
	$(CP) $(SOURCEDIR)/src/bin/rmse/_rmse.c $(TARGETDIR)/src/bin/rmse/_rmse.c
	$(CP) $(SOURCEDIR)/src/bin/rmse/rmse.c $(TARGETDIR)/src/bin/rmse/rmse.c

root_pol: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/root_pol
	$(CP) $(SOURCEDIR)/src/bin/root_pol/Makefile $(TARGETDIR)/src/bin/root_pol/Makefile
	$(CP) $(SOURCEDIR)/src/bin/root_pol/_root_pol.c $(TARGETDIR)/src/bin/root_pol/_root_pol.c
	$(CP) $(SOURCEDIR)/src/bin/root_pol/root_pol.c $(TARGETDIR)/src/bin/root_pol/root_pol.c

sin: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/sin
	$(CP) $(SOURCEDIR)/src/bin/sin/Makefile $(TARGETDIR)/src/bin/sin/Makefile
	$(CP) $(SOURCEDIR)/src/bin/sin/sin.c $(TARGETDIR)/src/bin/sin/sin.c

smcep: makefiles lib include fftr ifft
	mkdir -p $(TARGETDIR)/src/bin/smcep
	$(CP) $(SOURCEDIR)/src/bin/smcep/Makefile $(TARGETDIR)/src/bin/smcep/Makefile
	$(CP) $(SOURCEDIR)/src/bin/smcep/_smcep.c $(TARGETDIR)/src/bin/smcep/_smcep.c
	$(CP) $(SOURCEDIR)/src/bin/smcep/smcep.c $(TARGETDIR)/src/bin/smcep/smcep.c

snr: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/snr
	$(CP) $(SOURCEDIR)/src/bin/snr/Makefile $(TARGETDIR)/src/bin/snr/Makefile
	$(CP) $(SOURCEDIR)/src/bin/snr/snr.c $(TARGETDIR)/src/bin/snr/snr.c

sopr: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/sopr
	$(CP) $(SOURCEDIR)/src/bin/sopr/Makefile $(TARGETDIR)/src/bin/sopr/Makefile
	$(CP) $(SOURCEDIR)/src/bin/sopr/sopr.c $(TARGETDIR)/src/bin/sopr/sopr.c

spec: makefiles lib include fftr
	mkdir -p $(TARGETDIR)/src/bin/spec
	$(CP) $(SOURCEDIR)/src/bin/spec/Makefile $(TARGETDIR)/src/bin/spec/Makefile
	$(CP) $(SOURCEDIR)/src/bin/spec/spec.c $(TARGETDIR)/src/bin/spec/spec.c

srcnv: makefiles lib include
	$(CP) $(SOURCEDIR)/src/bin/srcnv/Makefile $(TARGETDIR)/src/bin/srcnv/Makefile
	$(CP) $(SOURCEDIR)/src/bin/srcnv/srcnv.c $(TARGETDIR)/src/bin/srcnv/srcnv.c
	$(CP) $(SOURCEDIR)/src/bin/srcnv/lpfcoef.2to3f $(TARGETDIR)/src/bin/srcnv/lpfcoef.2to3f
	$(CP) $(SOURCEDIR)/src/bin/srcnv/lpfcoef.2to3s $(TARGETDIR)/src/bin/srcnv/lpfcoef.2to3s
	$(CP) $(SOURCEDIR)/src/bin/srcnv/lpfcoef.3to4 $(TARGETDIR)/src/bin/srcnv/lpfcoef.3to4
	$(CP) $(SOURCEDIR)/src/bin/srcnv/lpfcoef.4to5 $(TARGETDIR)/src/bin/srcnv/lpfcoef.4to5
	$(CP) $(SOURCEDIR)/src/bin/srcnv/lpfcoef.5to7 $(TARGETDIR)/src/bin/srcnv/lpfcoef.5to7
	$(CP) $(SOURCEDIR)/src/bin/srcnv/lpfcoef.5to8 $(TARGETDIR)/src/bin/srcnv/lpfcoef.5to8

step: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/step
	$(CP) $(SOURCEDIR)/src/bin/step/Makefile $(TARGETDIR)/src/bin/step/Makefile
	$(CP) $(SOURCEDIR)/src/bin/step/step.c $(TARGETDIR)/src/bin/step/step.c

swab: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/swab
	$(CP) $(SOURCEDIR)/src/bin/swab/Makefile $(TARGETDIR)/src/bin/swab/Makefile
	$(CP) $(SOURCEDIR)/src/bin/swab/swab.c $(TARGETDIR)/src/bin/swab/swab.c

train: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/train
	$(CP) $(SOURCEDIR)/src/bin/train/Makefile $(TARGETDIR)/src/bin/train/Makefile
	$(CP) $(SOURCEDIR)/src/bin/train/train.c $(TARGETDIR)/src/bin/train/train.c

uels: makefiles lib include fftr
	mkdir -p $(TARGETDIR)/src/bin/uels
	$(CP) $(SOURCEDIR)/src/bin/uels/Makefile $(TARGETDIR)/src/bin/uels/Makefile
	$(CP) $(SOURCEDIR)/src/bin/uels/_uels.c $(TARGETDIR)/src/bin/uels/_uels.c
	$(CP) $(SOURCEDIR)/src/bin/uels/uels.c $(TARGETDIR)/src/bin/uels/uels.c

ulaw: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/ulaw
	$(CP) $(SOURCEDIR)/src/bin/ulaw/Makefile $(TARGETDIR)/src/bin/ulaw/Makefile
	$(CP) $(SOURCEDIR)/src/bin/ulaw/_ulaw.c $(TARGETDIR)/src/bin/ulaw/_ulaw.c
	$(CP) $(SOURCEDIR)/src/bin/ulaw/ulaw.c $(TARGETDIR)/src/bin/ulaw/ulaw.c

vopr: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/vopr
	$(CP) $(SOURCEDIR)/src/bin/vopr/Makefile $(TARGETDIR)/src/bin/vopr/Makefile
	$(CP) $(SOURCEDIR)/src/bin/vopr/vopr.c $(TARGETDIR)/src/bin/vopr/vopr.c

vq: makefiles lib include ivq
	mkdir -p $(TARGETDIR)/src/bin/vq
	$(CP) $(SOURCEDIR)/src/bin/vq/Makefile $(TARGETDIR)/src/bin/vq/Makefile
	$(CP) $(SOURCEDIR)/src/bin/vq/_vq.c $(TARGETDIR)/src/bin/vq/_vq.c
	$(CP) $(SOURCEDIR)/src/bin/vq/vq.c $(TARGETDIR)/src/bin/vq/vq.c

vstat: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/vstat
	$(CP) $(SOURCEDIR)/src/bin/vstat/Makefile $(TARGETDIR)/src/bin/vstat/Makefile
	$(CP) $(SOURCEDIR)/src/bin/vstat/vstat.c $(TARGETDIR)/src/bin/vstat/vstat.c

vsum: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/vsum
	$(CP) $(SOURCEDIR)/src/bin/vsum/Makefile $(TARGETDIR)/src/bin/vsum/Makefile
	$(CP) $(SOURCEDIR)/src/bin/vsum/vsum.c $(TARGETDIR)/src/bin/vsum/vsum.c

window: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/window
	$(CP) $(SOURCEDIR)/src/bin/window/Makefile $(TARGETDIR)/src/bin/window/Makefile
	$(CP) $(SOURCEDIR)/src/bin/window/_window.c $(TARGETDIR)/src/bin/window/_window.c
	$(CP) $(SOURCEDIR)/src/bin/window/window.c $(TARGETDIR)/src/bin/window/window.c

x2x: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/x2x
	$(CP) $(SOURCEDIR)/src/bin/x2x/Makefile $(TARGETDIR)/src/bin/x2x/Makefile
	$(CP) $(SOURCEDIR)/src/bin/x2x/x2x.c $(TARGETDIR)/src/bin/x2x/x2x.c

xgr: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/xgr
	$(CP) $(SOURCEDIR)/src/bin/xgr/Makefile $(TARGETDIR)/src/bin/xgr/Makefile
	$(CP) $(SOURCEDIR)/src/bin/xgr/README.jis $(TARGETDIR)/src/bin/xgr/README.jis
	$(CP) $(SOURCEDIR)/src/bin/xgr/config.h $(TARGETDIR)/src/bin/xgr/config.h
	$(CP) $(SOURCEDIR)/src/bin/xgr/gcdata.h $(TARGETDIR)/src/bin/xgr/gcdata.h
	mkdir -p $(TARGETDIR)/src/bin/xgr/marks
	$(CP) $(SOURCEDIR)/src/bin/xgr/marks/mark0.dat $(TARGETDIR)/src/bin/xgr/marks/mark0.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/marks/mark1.dat $(TARGETDIR)/src/bin/xgr/marks/mark1.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/marks/mark10.dat $(TARGETDIR)/src/bin/xgr/marks/mark10.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/marks/mark11.dat $(TARGETDIR)/src/bin/xgr/marks/mark11.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/marks/mark12.dat $(TARGETDIR)/src/bin/xgr/marks/mark12.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/marks/mark13.dat $(TARGETDIR)/src/bin/xgr/marks/mark13.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/marks/mark14.dat $(TARGETDIR)/src/bin/xgr/marks/mark14.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/marks/mark15.dat $(TARGETDIR)/src/bin/xgr/marks/mark15.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/marks/mark2.dat $(TARGETDIR)/src/bin/xgr/marks/mark2.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/marks/mark3.dat $(TARGETDIR)/src/bin/xgr/marks/mark3.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/marks/mark4.dat $(TARGETDIR)/src/bin/xgr/marks/mark4.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/marks/mark5.dat $(TARGETDIR)/src/bin/xgr/marks/mark5.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/marks/mark6.dat $(TARGETDIR)/src/bin/xgr/marks/mark6.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/marks/mark7.dat $(TARGETDIR)/src/bin/xgr/marks/mark7.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/marks/mark8.dat $(TARGETDIR)/src/bin/xgr/marks/mark8.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/marks/mark9.dat $(TARGETDIR)/src/bin/xgr/marks/mark9.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/plot.c $(TARGETDIR)/src/bin/xgr/plot.c
	mkdir -p $(TARGETDIR)/src/bin/xgr/tills
	$(CP) $(SOURCEDIR)/src/bin/xgr/tills/till0.dat $(TARGETDIR)/src/bin/xgr/tills/till0.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/tills/till1.dat $(TARGETDIR)/src/bin/xgr/tills/till1.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/tills/till10.dat $(TARGETDIR)/src/bin/xgr/tills/till10.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/tills/till11.dat $(TARGETDIR)/src/bin/xgr/tills/till11.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/tills/till12.dat $(TARGETDIR)/src/bin/xgr/tills/till12.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/tills/till13.dat $(TARGETDIR)/src/bin/xgr/tills/till13.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/tills/till14.dat $(TARGETDIR)/src/bin/xgr/tills/till14.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/tills/till15.dat $(TARGETDIR)/src/bin/xgr/tills/till15.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/tills/till16.dat $(TARGETDIR)/src/bin/xgr/tills/till16.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/tills/till2.dat $(TARGETDIR)/src/bin/xgr/tills/till2.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/tills/till3.dat $(TARGETDIR)/src/bin/xgr/tills/till3.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/tills/till4.dat $(TARGETDIR)/src/bin/xgr/tills/till4.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/tills/till5.dat $(TARGETDIR)/src/bin/xgr/tills/till5.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/tills/till6.dat $(TARGETDIR)/src/bin/xgr/tills/till6.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/tills/till7.dat $(TARGETDIR)/src/bin/xgr/tills/till7.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/tills/till8.dat $(TARGETDIR)/src/bin/xgr/tills/till8.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/tills/till9.dat $(TARGETDIR)/src/bin/xgr/tills/till9.dat
	$(CP) $(SOURCEDIR)/src/bin/xgr/window.c $(TARGETDIR)/src/bin/xgr/window.c
	$(CP) $(SOURCEDIR)/src/bin/xgr/xgr.c $(TARGETDIR)/src/bin/xgr/xgr.c
	$(CP) $(SOURCEDIR)/src/bin/xgr/xgr.h $(TARGETDIR)/src/bin/xgr/xgr.h

zcross: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/zcross
	$(CP) $(SOURCEDIR)/src/bin/zcross/Makefile $(TARGETDIR)/src/bin/zcross/Makefile
	$(CP) $(SOURCEDIR)/src/bin/zcross/_zcross.c $(TARGETDIR)/src/bin/zcross/_zcross.c
	$(CP) $(SOURCEDIR)/src/bin/zcross/zcross.c $(TARGETDIR)/src/bin/zcross/zcross.c

zerodf: makefiles lib include
	mkdir -p $(TARGETDIR)/src/bin/zerodf
	$(CP) $(SOURCEDIR)/src/bin/zerodf/Makefile $(TARGETDIR)/src/bin/zerodf/Makefile
	$(CP) $(SOURCEDIR)/src/bin/zerodf/_zerodf.c $(TARGETDIR)/src/bin/zerodf/_zerodf.c
	$(CP) $(SOURCEDIR)/src/bin/zerodf/zerodf.c $(TARGETDIR)/src/bin/zerodf/zerodf.c

include: makefiles
	mkdir -p $(TARGETDIR)/src/include
	$(CP) $(SOURCEDIR)/src/include/Makefile $(TARGETDIR)/src/include/Makefile
	$(CP) $(SOURCEDIR)/src/include/SPTK.h $(TARGETDIR)/src/include/SPTK.h

lib: makefiles include
	mkdir -p $(TARGETDIR)/src/lib
	$(CP) $(SOURCEDIR)/src/lib/Makefile $(TARGETDIR)/src/lib/Makefile
	$(CP) $(SOURCEDIR)/src/lib/Makefile.lib $(TARGETDIR)/src/lib/Makefile.lib
	$(CP) $(SOURCEDIR)/src/lib/agexp.c $(TARGETDIR)/src/lib/agexp.c
	$(CP) $(SOURCEDIR)/src/lib/cholesky.c $(TARGETDIR)/src/lib/cholesky.c
	$(CP) $(SOURCEDIR)/src/lib/fileio.c $(TARGETDIR)/src/lib/fileio.c
	$(CP) $(SOURCEDIR)/src/lib/fillz.c $(TARGETDIR)/src/lib/fillz.c
	$(CP) $(SOURCEDIR)/src/lib/getfp.c $(TARGETDIR)/src/lib/getfp.c
	$(CP) $(SOURCEDIR)/src/lib/getmem.c $(TARGETDIR)/src/lib/getmem.c
	$(CP) $(SOURCEDIR)/src/lib/gexp.c $(TARGETDIR)/src/lib/gexp.c
	$(CP) $(SOURCEDIR)/src/lib/glog.c $(TARGETDIR)/src/lib/glog.c
	$(CP) $(SOURCEDIR)/src/lib/ifftr.c $(TARGETDIR)/src/lib/ifftr.c
	$(CP) $(SOURCEDIR)/src/lib/matrix.c $(TARGETDIR)/src/lib/matrix.c
	$(CP) $(SOURCEDIR)/src/lib/movem.c $(TARGETDIR)/src/lib/movem.c
	$(CP) $(SOURCEDIR)/src/lib/mseq.c $(TARGETDIR)/src/lib/mseq.c
	$(CP) $(SOURCEDIR)/src/lib/theq.c $(TARGETDIR)/src/lib/theq.c
	$(CP) $(SOURCEDIR)/src/lib/toeplitz.c $(TARGETDIR)/src/lib/toeplitz.c

script: makefiles
	mkdir -p $(TARGETDIR)/src/script
	$(CP) $(SOURCEDIR)/src/script/Makefile $(TARGETDIR)/src/script/Makefile

bell: script echo2
	$(CP) $(SOURCEDIR)/src/script/bell.sh $(TARGETDIR)/src/script/bell.sh

daplay: script echo2 da srcnv
	$(CP) $(SOURCEDIR)/src/script/daplay.sh $(TARGETDIR)/src/script/daplay.sh

glogsp: script echo2 fig bcut fdrw
	$(CP) $(SOURCEDIR)/src/script/glogsp.sh $(TARGETDIR)/src/script/glogsp.sh

grlogsp: script echo2 fig bcut fdrw
	$(CP) $(SOURCEDIR)/src/script/grlogsp.sh $(TARGETDIR)/src/script/grlogsp.sh

gwave: script x2x minmax fig bcut fdrw
	$(CP) $(SOURCEDIR)/src/script/gwave.sh $(TARGETDIR)/src/script/gwave.sh
