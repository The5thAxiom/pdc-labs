include ../../Makefile.defs

#
# list all object files of the application
#
OBJS =  list.o hashtab.o partition.o color.o jobenv.o

LIBUTILS=libutils.a

all : lib

lib: $(LIBUTILS)

$(LIBUTILS) : $(OBJS)
	$(AR) r $(LIBUTILS) $(OBJS)

clean :
	rm -f *.o *~ $(LIBUTILS)


%.o : %.c 
	$(OMPCC) $(OMPFLAG) -DOMPP_RELEASE -c -o $@ $<
