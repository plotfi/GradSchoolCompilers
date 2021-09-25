#!/bin/dash

if [ -z $1 ]
then
    echo "$0 <accesses>"
    exit
fi

export LD_LIBRARY_PATH=/opt/intel/cc/latest/lib:$LD_LIBRARY_PATH

echo "accesses $1"

export accesses=$1

################################################################################################

export size=16384

time -p -o upanddown_ap_seq_`echo $size`_`echo $accesses`_1_0_native.time ./microbenchmarks/upanddown_ap.exe seq $size $accesses 1 0

echo "Running a microbenchmark..."
time -p -o upanddown_ap_seq_`echo $size`_`echo $accesses`_1_0_pin.time \
         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -benchname upanddown_ap_seq_`echo $size`_`echo $accesses` -- ./microbenchmarks/upanddown_ap.exe seq $size $accesses 1 0

#################################################################################################
#export size=65536
#
#time -p -o upanddown_ap_seq_`echo $size`_`echo $accesses`_1_0_native.time ./microbenchmarks/upanddown_ap.exe seq $size $accesses 1 0
#
#echo "Running a microbenchmark..."
#time -p -o upanddown_ap_seq_`echo $size`_`echo $accesses`_1_0_pin.time \
#         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -benchname upanddown_ap_seq_`echo $size`_`echo $accesses` -- ./microbenchmarks/upanddown_ap.exe seq $size $accesses 1 0
#
#################################################################################################
export size=262144

time -p -o upanddown_ap_seq_`echo $size`_`echo $accesses`_1_0_native.time ./microbenchmarks/upanddown_ap.exe seq $size $accesses 1 0

echo "Running a microbenchmark..."
time -p -o upanddown_ap_seq_`echo $size`_`echo $accesses`_1_0_pin.time \
         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -benchname upanddown_ap_seq_`echo $size`_`echo $accesses` -- ./microbenchmarks/upanddown_ap.exe seq $size $accesses 1 0

################################################################################################
export size=524288

time -p -o upanddown_ap_seq_`echo $size`_`echo $accesses`_1_0_native.time ./microbenchmarks/upanddown_ap.exe seq $size $accesses 1 0

echo "Running a microbenchmark..."
time -p -o upanddown_ap_seq_`echo $size`_`echo $accesses`_1_0_pin.time \
         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -benchname upanddown_ap_seq_`echo $size`_`echo $accesses` -- ./microbenchmarks/upanddown_ap.exe seq $size $accesses 1 0

################################################################################################
export size=1048576

time -p -o upanddown_ap_seq_`echo $size`_`echo $accesses`_1_0_native.time ./microbenchmarks/upanddown_ap.exe seq $size $accesses 1 0

echo "Running a microbenchmark..."
time -p -o upanddown_ap_seq_`echo $size`_`echo $accesses`_1_0_pin.time \
         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -benchname upanddown_ap_seq_`echo $size`_`echo $accesses` -- ./microbenchmarks/upanddown_ap.exe seq $size $accesses 1 0

################################################################################################
export size=2097152

time -p -o upanddown_ap_seq_`echo $size`_`echo $accesses`_1_0_native.time ./microbenchmarks/upanddown_ap.exe seq $size $accesses 1 0

echo "Running a microbenchmark..."
time -p -o upanddown_ap_seq_`echo $size`_`echo $accesses`_1_0_pin.time \
         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -benchname upanddown_ap_seq_`echo $size`_`echo $accesses` -- ./microbenchmarks/upanddown_ap.exe seq $size $accesses 1 0

#################################################################################################
#export size=4194304
#
#time -p -o upanddown_ap_seq_`echo $size`_`echo $accesses`_1_0_native.time ./microbenchmarks/upanddown_ap.exe seq $size $accesses 1 0
#
#echo "Running a microbenchmark..."
#time -p -o upanddown_ap_seq_`echo $size`_`echo $accesses`_1_0_pin.time \
#         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -benchname upanddown_ap_seq_`echo $size`_`echo $accesses` -- ./microbenchmarks/upanddown_ap.exe seq $size $accesses 1 0
#
#################################################################################################
#export size=8388608
#
#time -p -o upanddown_ap_seq_`echo $size`_`echo $accesses`_1_0_native.time ./microbenchmarks/upanddown_ap.exe seq $size $accesses 1 0
#
#echo "Running a microbenchmark..."
#time -p -o upanddown_ap_seq_`echo $size`_`echo $accesses`_1_0_pin.time \
#         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -benchname upanddown_ap_seq_`echo $size`_`echo $accesses` -- ./microbenchmarks/upanddown_ap.exe seq $size $accesses 1 0
#
################################################################################################












rm -rf results
mkdir results
mv *upanddown_ap_seq*.csv upanddown_ap_seq*.time results

mv results upanddown_ap_seq`echo $size`_`echo $accesses`

