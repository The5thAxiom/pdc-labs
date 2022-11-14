
include ../Makefile.defs

ifeq ($(PAPI_LIBRARY), )
all : kinst
else
all : kinst kinst-papi
endif

kinst : 
	sed -e 's,@OMPCC@,$(OMPCC),' \
	    -e 's,@BINDIR@,$(BINDIR),' \
	    -e 's,@INCDIR@,$(INCDIR),' \
	    -e 's,@LIBDIR@,$(LIBDIR),' \
	    -e 's,@OMPFLAG@,$(OMPFLAG),' \
	    -e 's,-lelg.ompi,-lompp,' \
	    -e 's,-lelg.omp,-lompp,' \
	    -e 's,-lelg.mpi,-lompp,' \
	    -e 's,-lelg,-lompp,' \
	    -e 's,@METRLIB@,,' \
	    -e 's,@FMPILIB@,,' \
	    -e 's,@PMPILIB@,,' \
	    -e 's,@UTILLIB@,,' \
	    -e 's,@FPP@,,' \
	kinst-pomp.in > kinst-ompp

kinst-papi : 
	sed -e 's,@OMPCC@,$(OMPCC),' \
	    -e 's,@BINDIR@,$(BINDIR),' \
	    -e 's,@INCDIR@,$(INCDIR),' \
	    -e 's,@LIBDIR@,$(LIBDIR),' \
	    -e 's,@OMPFLAG@,$(OMPFLAG),' \
	    -e 's,-lelg.ompi,-lompp-papi,' \
	    -e 's,-lelg.omp,-lompp-papi,' \
	    -e 's,-lelg.mpi,-lompp-papi,' \
	    -e 's,-lelg,-lompp-papi,' \
	    -e 's,@METRLIB@,$(PAPI_LIBRARY),' \
	    -e 's,@FMPILIB@,,' \
	    -e 's,@PMPILIB@,,' \
	    -e 's,@UTILLIB@,,' \
	    -e 's,@FPP@,,' \
	kinst-pomp.in > kinst-ompp-papi


clean :
	rm -f *~
	rm -f kinst-ompp*
