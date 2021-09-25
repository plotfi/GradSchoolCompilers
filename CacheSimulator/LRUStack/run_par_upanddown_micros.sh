#!/bin/dash -x

if [ -z $1 ]
then
    echo "$0 <accesses>"
    exit
fi

export LD_LIBRARY_PATH=/opt/intel/cc/latest/lib:$LD_LIBRARY_PATH
export OMP_NUM_THREADS=2

echo "accesses $1"

export accesses=$1

################################################################################################

export size=16384

time -p -o upanddown_ap_par_`echo $size`_`echo $accesses`_2_0_native.time ./microbenchmarks/upanddown_ap.exe par $size $accesses 2 0

echo "Running a microbenchmark..."
time -p -o upanddown_ap_par_`echo $size`_`echo $accesses`_2_0_pin.time \
         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -benchname upanddown_ap_par_`echo $size`_`echo $accesses` -- ./microbenchmarks/upanddown_ap.exe par $size $accesses 2 0

#################################################################################################
#export size=65536
#
#time -p -o upanddown_ap_par_`echo $size`_`echo $accesses`_2_0_native.time ./microbenchmarks/upanddown_ap.exe par $size $accesses 2 0
#
#echo "Running a microbenchmark..."
#time -p -o upanddown_ap_par_`echo $size`_`echo $accesses`_2_0_pin.time \
#         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -benchname upanddown_ap_par_`echo $size`_`echo $accesses` -- ./microbenchmarks/upanddown_ap.exe par $size $accesses 2 0
#
#################################################################################################
export size=262144

time -p -o upanddown_ap_par_`echo $size`_`echo $accesses`_2_0_native.time ./microbenchmarks/upanddown_ap.exe par $size $accesses 2 0

echo "Running a microbenchmark..."
time -p -o upanddown_ap_par_`echo $size`_`echo $accesses`_2_0_pin.time \
         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -benchname upanddown_ap_par_`echo $size`_`echo $accesses` -- ./microbenchmarks/upanddown_ap.exe par $size $accesses 2 0

################################################################################################
export size=524288

time -p -o upanddown_ap_par_`echo $size`_`echo $accesses`_2_0_native.time ./microbenchmarks/upanddown_ap.exe par $size $accesses 2 0

echo "Running a microbenchmark..."
time -p -o upanddown_ap_par_`echo $size`_`echo $accesses`_2_0_pin.time \
         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -benchname upanddown_ap_par_`echo $size`_`echo $accesses` -- ./microbenchmarks/upanddown_ap.exe par $size $accesses 2 0

################################################################################################
export size=1048576

time -p -o upanddown_ap_par_`echo $size`_`echo $accesses`_2_0_native.time ./microbenchmarks/upanddown_ap.exe par $size $accesses 2 0

echo "Running a microbenchmark..."
time -p -o upanddown_ap_par_`echo $size`_`echo $accesses`_2_0_pin.time \
         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -benchname upanddown_ap_par_`echo $size`_`echo $accesses` -- ./microbenchmarks/upanddown_ap.exe par $size $accesses 2 0

################################################################################################
export size=2097152

time -p -o upanddown_ap_par_`echo $size`_`echo $accesses`_2_0_native.time ./microbenchmarks/upanddown_ap.exe par $size $accesses 2 0

echo "Running a microbenchmark..."
time -p -o upanddown_ap_par_`echo $size`_`echo $accesses`_2_0_pin.time \
         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -benchname upanddown_ap_par_`echo $size`_`echo $accesses` -- ./microbenchmarks/upanddown_ap.exe par $size $accesses 2 0

#################################################################################################
#export size=4194304
#
#time -p -o upanddown_ap_par_`echo $size`_`echo $accesses`_2_0_native.time ./microbenchmarks/upanddown_ap.exe par $size $accesses 2 0
#
#echo "Running a microbenchmark..."
#time -p -o upanddown_ap_par_`echo $size`_`echo $accesses`_2_0_pin.time \
#         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -benchname upanddown_ap_par_`echo $size`_`echo $accesses` -- ./microbenchmarks/upanddown_ap.exe par $size $accesses 2 0
#
#################################################################################################
#export size=8388608
#
#time -p -o upanddown_ap_par_`echo $size`_`echo $accesses`_2_0_native.time ./microbenchmarks/upanddown_ap.exe par $size $accesses 2 0
#
#echo "Running a microbenchmark..."
#time -p -o upanddown_ap_par_`echo $size`_`echo $accesses`_2_0_pin.time \
#         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -benchname upanddown_ap_par_`echo $size`_`echo $accesses` -- ./microbenchmarks/upanddown_ap.exe par $size $accesses 2 0
#
################################################################################################












rm -rf results
mkdir results
mv *upanddown_ap_par*.csv upanddown_ap_par*.time results

mv results upanddown_ap_par`echo $size`_`echo $accesses`

