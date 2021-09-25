#!/bin/dash -x

export samplerate=100000
export sampleburst=100
export OMP_NUM_THREADS=2
export thread=par

export bench=c_fft6

export size=1024
export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size 4



