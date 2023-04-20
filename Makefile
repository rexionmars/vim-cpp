TARGET=a.out
CXX=clang++
DEBUG=-g
OPT=-O0
WARN=-Wall -Wno-unknown-pragmas
NCURSES=-lncurses -ltinfo
CXXFLAGS=$(DEBUG) $(OPT) $(WARN) $(NCURSES) -pipe
LD=clang++
LDFLAGS=$(NCURSES)
OBJS=main.o corevim.o
all: $(OBJS)
		$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)
		@rm -rfv *.o

main.o: main.cpp
		$(CXX) -c $(CXXFLAGS) main.cpp -o main.o

corevim.o: corevim.cpp
		$(CXX) -c $(CXXFLAGS) corevim.cpp -o corevim.o
