CPPFLAGS = -Wall -O3 -std=c++11 -lm -w -mcmodel=medium
PROGRAMS = main

all: $(PROGRAMS)

main:main.cpp priority.h BOBHash.h PriorityBF.h CountingBF.h \
	CMSketch.h PriorityCM.h CUSketch.h PriorityCU.h
	g++ -o priority main.cpp $(CPPFLAGS)

clean:
	rm -f *.o $(PROGRAMS)
