# Makefile for Prospector-LLVM profiling

CC        = llvm-gcc
CXX       = llvm-g++
CFLAGS    = -emit-llvm -c -O3 --std=c99
CXXFLAGS  = -emit-llvm -c -O3
OUTPUTDIR = Release+Debug

output: cfiles cppfiles
	mkdir -p $(OUTPUTDIR)
	mv *.o $(OUTPUTDIR)

cppfiles:
	$(CXX) $(CXXFLAGS) *.cpp

cfiles:
	$(CC) $(CFLAGS) *.c

clean:
	rm -rf *.ll *.bc *~ *.o *.s a.out *.csv $(OUTPUTDIR) tags

tags:
	ctags -R --c++-kinds=+p --fields=+iaS --extra=+q . /usr/include/*.h /usr/include/c++/

