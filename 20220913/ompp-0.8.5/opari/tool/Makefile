TOPDIR = ../..

include $(TOPDIR)/Makefile.defs
include $(TOPDIR)/mf/common.defs

.SUFFIXES: .cc .o

OBJS = opari.o process_c.o process_f.o process_omp.o \
	ompragma.o ompragma_c.o ompragma_f.o ompregion.o handler.o

all : opari

opari: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o opari $(OBJS) $(SFLAG)

.cc.o:
	$(CXX) $(CXXFLAGS) -c $*.cc

install: all
	$(MKDIR) -m 755 $(BINDIR) $(INCDIR) $(DOCDIR)/html
	$(INSTALL) -c -m 755 opari $(BINDIR)
	$(INSTALL) -c -m 644 ../lib/opari_omp.h $(INCDIR)
	$(INSTALL) -c -m 644 ../lib/pomp_lib.h $(INCDIR)
	$(INSTALL) -c -m 644 ../Readme.html $(DOCDIR)/html/opari.html
	$(INSTALL) -c -m 644 ../opari-logo-100.gif $(DOCDIR)/html

clean:
	rm -rf core a.out *.o opari out
	rm -rf Templates.DB tempinc ti_files ii_files rii_files *.ti *.ir
	rm -rf SunWS_cache cxx_repository
	(cd Test; $(MAKE) clean)

test: opari
	(cd Test; $(MAKE) test)

handler.o: handler.cc handler.h opari.h ompragma.h ompregion.h
ompragma.o: ompragma.cc ompragma.h
ompragma_f.o: ompragma_f.cc ompragma.h
ompragma_c.o: ompragma_c.cc ompragma.h
opari.o: opari.cc opari.h ompragma.h handler.h
process_c.o: process_c.cc opari.h ompragma.h handler.h
process_f.o: process_f.cc opari.h ompragma.h handler.h
process_omp.o: process_omp.cc opari.h ompragma.h handler.h
ompregion.o: ompregion.cc ompregion.h
