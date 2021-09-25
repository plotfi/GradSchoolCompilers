#!/bin/dash -x

export samplerate=100000
export sampleburst=100

export OMP_NUM_THREADS=1
export thread=seq

export bench=c_lu

export size=2048
export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size

