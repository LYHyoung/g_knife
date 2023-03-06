CXX = g++
CXXFLAGS = -std=c++11 -g
LD = $(CXX)
LDFLAGS = -lm -lrt

all : test
test : test.o Rapl.o
	$(LD) $(LDFLAGS) -o $@ $^
test.o : test.cpp Rapl.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<
Rapl.o : Rapl.cpp Rapl.h
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

clean :
	rm -f *.o 
	rm -f rapl.csv
	rm -f test
