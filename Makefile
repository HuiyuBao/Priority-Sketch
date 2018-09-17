CPPFLAGS = -Wall -O3 -std=c++11 -lm -w
PROGRAMS = main

all: $(PROGRAMS)

main:main.cpp priority.h BOBHash.h CSketch.h PriorityC.h \
	CMSketch.h PriorityCM.h CUSketch.h PriorityCU.h
	g++ -o priority main.cpp $(CPPFLAGS)

clean:
	rm -f *.o $(PROGRAMS)
