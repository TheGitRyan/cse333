SRC=hw3.c modelCallbacks.c view.c model.c action.c  # change these to your source file names
HEADERS= modelCallbacks.h view.h model.h action.h# change these to your header file names
LIBS=./libs/array2d.a  # you should use your hw2 code as a library
CFLAGS = -Wall -Wpedantic -std=c11 
IFLAGS =  -I. -I./libs/jansson/include
GTKCFLAGS = `pkg-config --cflags gtk+-3.0`
LDFLAGS = -L. -L./libs/jansson/lib -l:libjansson.a
GTKLDFLAGS = `pkg-config --libs gtk+-3.0`

TESTSRC=test.c model.c action.c modelCallbacks.c
TESTH= model.h action.h modelCallbacks.h 

ASRC = array_test.c modelCallbacks.c
AH = modelCallbacks.h
AI = -I. -I./libs 
AL = -L. -L./libs -l:array2d.a

all: hw3

hw3: $(SRC) $(HEADERS)
	gcc $(CFLAGS) -g -O0 $(AI) $(IFLAGS) $(GTKCFLAGS) -o hw3 $(SRC) $(AL) $(LDFLAGS) $(GTKLDFLAGS)

array_test: $(ASRC) $(AH)
	gcc $(CFLAGS) -g -O0 $(AI) $(IFLAGS) -o array_test $(ASRC) $(AL) $(LDFLAGS) 

test: $(TESTSRC) $(TESTH) $(LIBS)
	gcc $(CFLAGS) -g -O0 $(AI) $(IFLAGS) -o test $(TESTSRC) $(AL) $(LDFLAGS)
 
clean:
	rm -f *.o *~ hw3
