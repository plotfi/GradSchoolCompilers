#!/bin/dash -x

export samplerate=100000
export sampleburst=100

####################################################################################


export OMP_NUM_THREADS=2
export thread=par

export bench=c_fft

export size=8192
export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size

export size=10240
export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size

export size=12288
export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size

export size=14336
export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size

#export size=16384
#export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
#time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size
#
#export size=32768
#export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
#time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size
#
#export size=49512
#export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
#time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size
#
#export size=98304
#export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
#time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size
#
#export size=196608
#export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
#time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size
#
#
####################################################################################

export bench=c_fft6

export size=1024
export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size 4

export size=2048
export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size 4

#export size=3072
#export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
#time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size 4


####################################################################################

export bench=c_lu

export size=1024
export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size

export size=2048
export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size

export size=4096
export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size

export size=8192
export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size

####################################################################################


export OMP_NUM_THREADS=4


export bench=c_fft

export size=8192
export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size

export size=10240
export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size

export size=12288
export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size

export size=14336
export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size

#export size=16384
#export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
#time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size
#
#export size=32768
#export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
#time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size
#
#export size=49512
#export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
#time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size
#
#export size=98304
#export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
#time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size
#
#export size=196608
#export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
#time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size
#
#
####################################################################################

export bench=c_fft6

export size=1024
export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size 4

export size=2048
export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size 4

#export size=3072
#export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
#time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size 4


####################################################################################

export bench=c_lu

export size=1024
export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size

export size=2048
export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size

export size=4096
export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size

export size=8192
export name="`echo $bench`_`echo $thread`_`echo $OMP_NUM_THREADS`_`echo $size`"
time -p -o `echo $name`.time ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -enable_sampling 1 -benchname $name -- ./bench/`echo $bench`.`echo $thread` $size

####################################################################################

