SRC=hw4.cpp modelCallbacks.c view.c model.c action.c  # change these to your source file names
HEADERS= modelCallbacks.h view.h model.h action.h # change these to your header file names
LIBS=./libs/array2d.a  # you should use your hw2 code as a library
CPPFLAGS = -Wall -Wpedantic -Wno-variadic-macros -Wno-pedantic -Wno-write-strings -Wno-int-to-pointer-cast -Wno-sign-compare
CFLAGS = -Wall -Wpedantic -Wno-int-to-pointer-cast -std=c11
IFLAGS =  -I. -I./libs/jansson/include
GTKCFLAGS = `pkg-config --cflags gtk+-3.0`
LDFLAGS = -L. -L./libs/jansson/lib -l:libjansson.a
GTKLDFLAGS = `pkg-config --libs gtk+-3.0`

SRCCPP = game.cpp gamestate.cpp extensionboard.cpp model.c
HEADERSCPP = game.h gamestate.h extensionboard.h model.h

AI = -I. -I./libs 
AL = -L. -L./libs -l:array2d.a

OBJS = view.o action.o model.o modelCallbacks.o game.o gamestate.o extensionboard.o hw4.o

all: CC CCPP hw4

hw4:
	g++ $(CPPFLAGS) -g -O0 $(AI) $(IFLAGS) $(GTKCFLAGS) -o hw4 $(OBJS) $(AL) $(LDFLAGS) $(GTKLDFLAGS)

CCPP: game.o gamestate.o extensionboard.o hw4.o

game.o:
	g++ -c $(CPPFLAGS) $(AI) $(IFLAGS) -o game.o game.cpp $(AL) $(LDFLAGS)

gamestate.o:
	g++ -c $(CPPFLAGS) $(AI) $(IFLAGS) -o gamestate.o gamestate.cpp $(AL) $(LDFLAGS)

extensionboard.o:
	g++ -c $(CPPFLAGS) $(AI) $(IFLAGS) -o extensionboard.o extensionboard.cpp $(AL) $(LDFLAGS)

hw4.o:
	g++ -c $(CPPFLAGS) $(AI) $(IFLAGS) $(GTKCFLAGS) -o hw4.o hw4.cpp $(AL) $(LDFLAGS) $(GTKLDFLAGS)

CC: view.o action.o model.o modelCallbacks.o

view.o: 
	gcc -c $(CFLAGS) $(AI) $(IFLAGS) $(GTKCFLAGS) -o view.o view.c $(AL) $(LDFLAGS) $(GTKLDFLAGS)

action.o:
	gcc -c $(CFLAGS) -o action.o action.c

model.o:
	gcc -c $(CFLAGS) $(AI) $(IFLAGS) -o model.o model.c $(AL) $(LDFLAGS)

modelCallbacks.o: 
	gcc -c $(CFLAGS) $(AI) $(IFLAGS) -o modelCallbacks.o modelCallbacks.c $(AL) $(LDFLAGS)

clean:
	rm -f *.o *~ hw4
