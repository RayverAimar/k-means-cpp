CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra
SHARED   := src/kmeans.cpp

.PHONY: all clean

all: kmeans benchmark

kmeans: src/main.cpp $(SHARED) src/kmeans.h src/point.h
	$(CXX) $(CXXFLAGS) src/main.cpp $(SHARED) -o $@

benchmark: src/benchmark.cpp $(SHARED) src/kmeans.h src/point.h
	$(CXX) $(CXXFLAGS) src/benchmark.cpp $(SHARED) -o $@

clean:
	rm -f kmeans benchmark
