# define the commands we will use for compilation and library building
AR = ar
ARFLAGS = rcs
CC = gcc

# define useful flags to cc/ld/etc.
CFLAGS += -std=c11 -g -Wall -Wpedantic -I. -I./jansson/include -O0
LDFLAGS += -L. -L./jansson/lib -lhw2 -l:libjansson.a

#replace these definitions with lists of your source and header files
LIBOBJS=array2d.o 
HEADERS=array2d.h

all: array2d.a 

array2d.a: $(LIBOBJS) $(HEADERS)
	$(AR) $(ARFLAGS) array2d.a $(LIBOBJS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

clean:
	/bin/rm -f *.o *~ hw2Driver libhw2.a json.out


