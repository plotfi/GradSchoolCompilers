#!/bin/bash

rm -rf histogram*.csv results
make clean && make -j3

time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate 1000 -benchname c_fft6_seq  -- ./bench/c_fft6.seq `cat  bench/c_fft6.arg`

time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate 1000 -benchname c_fft6_par  -- ./bench/c_fft6.par `cat  bench/c_fft6.arg`

time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate 1000 -benchname c_fft_seq   -- ./bench/c_fft.seq  `cat  bench/c_fft.arg`

time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate 1000 -benchname c_fft_par   -- ./bench/c_fft.par  `cat  bench/c_fft.arg`

time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate 1000 -benchname c_lu_seq -- ./bench/c_lu.seq   `cat bench/c_lu.arg`

time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate 1000 -benchname c_lu_par -- ./bench/c_lu.par   `cat bench/c_lu.arg`

time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate 1000 -benchname c_md_seq -- ./bench/c_md.seq   `cat bench/c_md.arg`

time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate 1000 -benchname c_md_par -- ./bench/c_md.par   `cat bench/c_md.arg`

mkdir results
mv histogram*.csv results
dirname=~/Dropbox/Suitability/code/puyan/benchmarks/results_`date +%j_%H%M`
mkdir $dirname
mv results/* $dirname

