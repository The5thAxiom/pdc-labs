
include ../Makefile.defs

BUILDOPT_FLAGS=-DOMPP_RELEASE

#
# list all object files of the application
#
OBJS =  ompp.o ompp_region.o ompp_pomp.o \
	ompp_report.o ompp_report_callgraph.o \
	ompp_fortran_1.o ompp_fortran_2.o \
	ompp_fortran_3.o ompp_fortran_4.o \
	ompp_fortran_5.o ompp_fortran_6.o \
	ompp_regstat.o ompp_overheads.o ompp_papi.o  \
	ompp_properties.o ompp_eval.o                \
	ompp_incremental.o ompp_rstack.o ompp_time.o \
	ompp_gcc.o ompp_controlflow.o ompp_report_cube.o \
	ompp_report_xml.o

ifneq ($(HYBCC), )
OBJS+=ompp_mpi.o
BUILDOPT_FLAGS+=-DOMPP_MPI
endif

PAPIOBJS = $(addprefix papi., $(OBJS))


#
# The name of the ompP target library
#
LIBOMPP=libompp.a
LIBOMPPPAPI=libompp-papi.a

LIBMATHEVAL = matheval/libmatheval.a
LIBUTILS    = utils/libutils.a
LIBCUBE     = cubew/libcubew3.a



ifeq ($(PAPI_LIBRARY), )
EXTRAFLAGS=
all: $(LIBOMPP) 
else
EXTRAFLAGS+=-DOMPP_USE_PAPI -I$(PAPI_INCLUDE)
all: $(LIBOMPP) $(LIBOMPPPAPI)

endif


$(LIBOMPP) : $(OBJS) $(LIBUTILS) $(LIBCUBE)
	$(AR) x $(LIBUTILS)
	$(AR) x $(LIBCUBE)
	$(AR) r $(LIBOMPP) $(OBJS) `$(AR) t $(LIBUTILS)` \
                           `$(AR) t $(LIBCUBE)`
	rm -f `$(AR) t $(LIBUTILS)`
	rm -f `$(AR) t $(LIBCUBE)`

BUILDOPT_FLAGS+=$(EXTRAFLAGS)

$(LIBOMPPPAPI) : $(PAPIOBJS) $(LIBMATHEVAL) $(LIBUTILS) 
	$(AR) x $(LIBMATHEVAL)
	$(AR) t $(LIBMATHEVAL)  | xargs ./ompp_rename.pl 's/\.o/\.MATHEVAL\.o/' {}
	$(AR) x $(LIBUTILS)
	$(AR) t $(LIBUTILS)     | xargs ./ompp_rename.pl 's/\.o/\.UTILS\.o/' {}
	$(AR) x $(LIBCUBE)
	$(AR) t $(LIBCUBE)      | xargs ./ompp_rename.pl 's/\.o/\.CUBE\.o/' {}
	$(AR) r $(LIBOMPPPAPI) $(PAPIOBJS) *.MATHEVAL.o *.UTILS.o *.CUBE.o
	rm -f *.MATHEVAL.o
	rm -f *.UTILS.o
	rm -f *.CUBE.o

$(LIBMATHEVAL) :
	$(MAKE) -C matheval/

$(LIBUTILS) :
	$(MAKE) -C utils/


$(LIBCUBE) : 
	$(MAKE) -C cubew/ CCC=$(OMPCC)


###############
# build rules #
###############


#
# Intel, IBM on AIX
#
ompp_fortran_1.o : ompp_fortran.c
	$(OMPCC) $(OMPFLAG) $(BUILDOPT_FLAGS) -c -o $@ -D F_LOWER -D F_SFX_USC $<
papi.ompp_fortran_1.o : ompp_fortran.c
	$(OMPCC) $(OMPFLAG) $(BUILDOPT_FLAGS) -c -o $@ -D F_LOWER -D F_SFX_USC $<

#
# PGI on x86_64
#
ompp_fortran_2.o : ompp_fortran.c
	$(OMPCC) $(OMPFLAG) $(BUILDOPT_FLAGS) -c -o $@ -D F_LOWER -D F_SFX_2USC $<
papi.ompp_fortran_2.o : ompp_fortran.c
	$(OMPCC) $(OMPFLAG) $(BUILDOPT_FLAGS) -c -o $@ -D F_LOWER -D F_SFX_2USC $<

#
ompp_fortran_3.o : ompp_fortran.c
	$(OMPCC) $(OMPFLAG) $(BUILDOPT_FLAGS) -c -o $@ -D F_UPPER -D F_SFX_USC $<
papi.ompp_fortran_3.o : ompp_fortran.c
	$(OMPCC) $(OMPFLAG) $(BUILDOPT_FLAGS) -c -o $@ -D F_UPPER -D F_SFX_USC $<

#
ompp_fortran_4.o : ompp_fortran.c
	$(OMPCC) $(OMPFLAG) $(BUILDOPT_FLAGS) -c -o $@ -D F_UPPER -D F_SFX_2USC $<
papi.ompp_fortran_4.o : ompp_fortran.c
	$(OMPCC) $(OMPFLAG) $(BUILDOPT_FLAGS) -c -o $@ -D F_UPPER -D F_SFX_2USC $<

#
ompp_fortran_5.o : ompp_fortran.c
	$(OMPCC) $(OMPFLAG) $(BUILDOPT_FLAGS) -c -o $@ -D F_UPPER -D F_NO_SFX $<
papi.ompp_fortran_5.o : ompp_fortran.c
	$(OMPCC) $(OMPFLAG) $(BUILDOPT_FLAGS) -c -o $@ -D F_UPPER -D F_NO_SFX $<

#
ompp_fortran_6.o : ompp_fortran.c
	$(OMPCC) $(OMPFLAG) $(BUILDOPT_FLAGS) -c -o $@ -D F_LOWER -D F_NO_SFX $<
papi.ompp_fortran_6.o : ompp_fortran.c
	$(OMPCC) $(OMPFLAG) $(BUILDOPT_FLAGS) -c -o $@ -D F_LOWER -D F_NO_SFX $<

ompp_mpi.o : ompp_mpi.c ompp_mpi.h
	$(HYBCC) $(HYBFLAG) $(BUILDOPT_FLAGS) -c -o $@ $<

papi.ompp_mpi.o : ompp_mpi.c ompp_mpi.h
	$(HYBCC) $(HYBFLAG) $(BUILDOPT_FLAGS) -c -o $@ $<


archive :
	tar -xvzf ompp.tgz *.c *.cc *.h Makefile 


clean : 
	rm -f *.o *~ $(LIBOMPP) $(LIBOMPPPAPI)
	rm -f ompp.tgz


remake : clean all

%.o : %.c %.h
	$(OMPCC) $(OMPFLAG) $(BUILDOPT_FLAGS) -c -o $@ $<

papi.%.o : %.c %.h
	$(OMPCC) $(OMPFLAG) $(BUILDOPT_FLAGS) -c -o $@ $<
