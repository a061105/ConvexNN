CXX = g++
FLAG = -O3 -fopenmp
LIBS = blas/blas.a
SHVER = 2

all: testSolve run

testSolve: tron.o blas/blas.a
	$(CXX) $(FLAG) -o testSolve testSolve.cpp tron.o $(LIBS)

run:
	./testSolve ../../data/australian_scale

tron.o: tron.cpp tron.h
	$(CXX) $(CFLAGS) -c -o tron.o tron.cpp

blas/blas.a: blas/*.c blas/*.h
	make -C blas OPTFLAGS='$(CFLAGS)' CC='$(CC)';

clean:
	make -C blas clean
	rm -f *~ tron.o testSolve
