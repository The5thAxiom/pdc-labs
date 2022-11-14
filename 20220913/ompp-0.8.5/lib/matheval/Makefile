
#
# Include makefile definitions
#
include ../../Makefile.defs


#
# List all object files of the application
#
OBJS = error.o matheval.o node.o parser.o scanner.o symbol_table.o \
       xmalloc.o xmath.o yywrap_fix.o


BUILDOPT_FLAGS+=-DHAVE_MATH_H -DSTDC_HEADERS


LIBMATHEVAL=libmatheval.a

all: $(LIBMATHEVAL)


$(LIBMATHEVAL) : $(OBJS)
	$(AR) r $(LIBMATHEVAL) $(OBJS)


%.o : %.c %.h
	$(OMPCC) $(OMPFLAG) $(BUILDOPT_FLAGS) -c -o $@ $<

%.o : %.c
	$(OMPCC) $(OMPFLAG) $(BUILDOPT_FLAGS) -c -o $@ $<


clean :
	rm -f *.o *~ $(LIBMATHEVAL)

