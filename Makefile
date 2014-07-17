CXXFLAGS=-Wall -I /usr/bin/boost/include/
CXX_DEBUG_FLAGS=-g -g3
CXX_RELEASE_FLAGS=-O3
OUT_EXE=occamsNet_p2.exe

all: occamsNet_p2

.PHONY: debug  
debug: CXXFLAGS+=$(CXX_DEBUG_FLAGS)  
debug: OUT_EXE=debugOut
debug: all

.PHONY: release  
release: CXXFLAGS+=$(CXX_RELEASE_FLAGS)
release: all

.PHONY: tutorial
tutorial:
	g++ $(CXXFLAGS) tutorial.cpp -o tutorial.exe

.PHONY: test
test: testSubgraph.o
	g++ $(CXXFLAGS) testSubgraph.o -o $(OUT_EXE)

occamsNet_p2: main.o Graph.o VertexSet.o
	g++ $(CXXFLAGS) main.o Graph.o VertexSet.o -o $(OUT_EXE)

.o:
	g++ $(CXXFLAGS) -c $< -o %@

.PHONY: clean
clean:
	rm -rf *o occamsNet_p2 tutorial.exe
