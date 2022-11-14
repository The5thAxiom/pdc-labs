#
# ompP 0.8.5 Makefile
# do not change this file, change Makefile.defs
#

include Makefile.defs

ifeq ($(INSTDIR), )
error1:
	@echo "Error: Installation directory not set."
	@echo "       set INSTDIR in Makefile.defs"
endif

ifeq ($(OMPCC), )
error2:
	@echo "Error: No OpenMP C compiler specified."
	@echo "       set OMPCC in Makefile.defs"
endif

ifeq ($(OMPFLAG), )
error3:
	@echo "Error: No OpenMP compiler-flag specified."
	@echo "       set OMPFLAG in Makefile.defs"
endif


all : ompp-lib opari-tool kinst-tool

ompp-lib :
	$(MAKE) -C lib/ 

opari-tool :
	$(MAKE) -C opari/tool/

kinst-tool :
	$(MAKE) -C kinst/

clean : 
	rm -f *~
	$(MAKE) -C lib/ clean
	$(MAKE) -C kinst/ clean
	$(MAKE) -C opari/tool/ clean

archive : 

install : all
	mkdir -p -m 755 $(LIBDIR) $(BINDIR) $(INCDIR) $(DOCDIR) $(TESTDIR)
	cp opari/tool/opari $(BINDIR)
	cp lib/libompp*.a $(LIBDIR)
	cp include/opari_omp.h $(INCDIR)
	cp include/pomp_lib.h $(INCDIR)
	cp kinst/kinst-ompp* $(BINDIR)
	cp doc/* $(DOCDIR)
	cp -r test/* $(TESTDIR)
	chmod 755 $(BINDIR)/opari
	chmod 755 $(BINDIR)/kinst-ompp*
	chmod 644 $(LIBDIR)/libompp*.a
	chmod 644 $(INCDIR)/pomp_lib.h
	chmod 644 $(INCDIR)/opari_omp.h
	chmod 644 $(DOCDIR)/*

