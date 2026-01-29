#**************************************
# Makefile
#
# Makefile for lang compiler
#
# Author: Phil Howard 
# phil.howard@oit.edu
#
# Date: Jan. 12, 2016
#

COPTS=-Wall -g -c -O0 -std=c++11 
OBJS=main.o \
	 langlex.o \
	 langparse.o \
	 cVisitor.o

all: lang

clean:
	rm -f $(OBJS)
	rm -f *.o
	rm -f langlex.c
	rm -f lang
	rm -f out.xml
	rm -f out2.xml
	rm -f langlex.c
	rm -f langparse.c
	rm -f langparse.h


.c.o:
	g++ $(COPTS) $? -o $@

.cpp.o:
	g++ $(COPTS) $? -o $@

main.o: main.cpp langlex.c langparse.c
	g++ $(COPTS) main.cpp -o main.o

langlex.c: lang.l langparse.c
	flex -o langlex.c lang.l

langparse.c: lang.y
	bison --defines=langparse.h lang.y -o langparse.c

langlex.o: langlex.c
	g++ $(COPTS) -Wno-sign-compare $? -o $@

lang: $(OBJS)
	g++ $(OBJS) -o lang

