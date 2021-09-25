#!/bin/bash -x

export CC=icc
export FOPENMP=-openmp -g

all:
	$(CC) main.cpp -O0 -c -o main.o
	$(CC) $(FOPENMP) -O0 pseudorand_ap.cpp main.o -o  pseudorand_ap.exe
	$(CC) $(FOPENMP) -O0 blocksized_ap.cpp main.o -o  blocksized_ap.exe
	$(CC) $(FOPENMP) -O0 frontseq_ap.cpp   main.o -o  frontseq_ap.exe
	$(CC) $(FOPENMP) -O0 backseq_ap.cpp    main.o -o  backseq_ap.exe
	$(CC) $(FOPENMP) -O0 divconq_ap.cpp    main.o -o  divconq_ap.exe
	$(CC) $(FOPENMP) -O0 tiled_ap.cpp      main.o -o  tiled_ap.exe
	$(CC) $(FOPENMP) -O0 even_ap.cpp       main.o -o  even_ap.exe
	$(CC) $(FOPENMP) -O0 multipass_ap.cpp  main.o -o  multiplepass_ap.exe
	$(CC) $(FOPENMP) -O0 subsections_ap.cpp main.o -o subsection_ap.exe
	$(CC) $(FOPENMP) -O0 upanddown_ap.cpp main.o -o upanddown_ap.exe
	$(CC) $(FOPENMP) -O0 empty_ap.cpp      main.o -o empty_ap.exe


clean:
	rm *.exe *.o *.time *.csv
