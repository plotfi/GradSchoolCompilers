#!/bin/dash -x

if [ -z $1 ]
then
    echo "$0 <accesses>"
    exit
fi

export LD_LIBRARY_PATH=/opt/intel/cc/latest/lib:$LD_LIBRARY_PATH

echo "accesses $1"

export accesses=$1

################################################################################################

#export size=16384
#
#time -p -o blocksized_ap_pred_`echo $size`_`echo $accesses`_1_0_native.time ./microbenchmarks/blocksized_ap.exe seq $size $accesses 1 0
#
#echo "Running a microbenchmark..."
#time -p -o blocksized_ap_pred_`echo $size`_`echo $accesses`_1_0_pin.time \
#         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -predict_for_threads 1 -benchname blocksized_ap_pred_`echo $size`_`echo $accesses` -- ./microbenchmarks/blocksized_ap.exe seq $size $accesses 1 0
#
#################################################################################################
#export size=65536
#
#time -p -o blocksized_ap_pred_`echo $size`_`echo $accesses`_1_0_native.time ./microbenchmarks/blocksized_ap.exe seq $size $accesses 1 0
#
#echo "Running a microbenchmark..."
#time -p -o blocksized_ap_pred_`echo $size`_`echo $accesses`_1_0_pin.time \
#         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -predict_for_threads 1 -benchname blocksized_ap_pred_`echo $size`_`echo $accesses` -- ./microbenchmarks/blocksized_ap.exe seq $size $accesses 1 0
#
#################################################################################################
#export size=262144
#
#time -p -o blocksized_ap_pred_`echo $size`_`echo $accesses`_1_0_native.time ./microbenchmarks/blocksized_ap.exe seq $size $accesses 1 0
#
#echo "Running a microbenchmark..."
#time -p -o blocksized_ap_pred_`echo $size`_`echo $accesses`_1_0_pin.time \
#         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -predict_for_threads 1 -benchname blocksized_ap_pred_`echo $size`_`echo $accesses` -- ./microbenchmarks/blocksized_ap.exe seq $size $accesses 1 0
#
################################################################################################
export size=524288

#time -p -o blocksized_ap_pred_`echo $size`_`echo $accesses`_1_0_native.time ./microbenchmarks/blocksized_ap.exe seq $size $accesses 1 0

echo "Running a microbenchmark..."
time -p -o blocksized_ap_pred_`echo $size`_`echo $accesses`_1_0_pin.time \
         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -predict_for_threads 2 -benchname blocksized_ap_pred_`echo $size`_`echo $accesses` -- ./microbenchmarks/blocksized_ap.exe seq $size $accesses 1 0

################################################################################################
#export size=1048576
#
#time -p -o blocksized_ap_pred_`echo $size`_`echo $accesses`_1_0_native.time ./microbenchmarks/blocksized_ap.exe seq $size $accesses 1 0
#
#echo "Running a microbenchmark..."
#time -p -o blocksized_ap_pred_`echo $size`_`echo $accesses`_1_0_pin.time \
#         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -predict_for_threads 1 -benchname blocksized_ap_pred_`echo $size`_`echo $accesses` -- ./microbenchmarks/blocksized_ap.exe seq $size $accesses 1 0
#
################################################################################################
#export size=2097152
#
#time -p -o blocksized_ap_pred_`echo $size`_`echo $accesses`_1_0_native.time ./microbenchmarks/blocksized_ap.exe seq $size $accesses 1 0
#
#echo "Running a microbenchmark..."
#time -p -o blocksized_ap_pred_`echo $size`_`echo $accesses`_1_0_pin.time \
#         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -predict_for_threads 1 -benchname blocksized_ap_pred_`echo $size`_`echo $accesses` -- ./microbenchmarks/blocksized_ap.exe seq $size $accesses 1 0
#
#################################################################################################
#export size=4194304
#
#time -p -o blocksized_ap_pred_`echo $size`_`echo $accesses`_1_0_native.time ./microbenchmarks/blocksized_ap.exe seq $size $accesses 1 0
#
#echo "Running a microbenchmark..."
#time -p -o blocksized_ap_pred_`echo $size`_`echo $accesses`_1_0_pin.time \
#         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -predict_for_threads 1 -benchname blocksized_ap_pred_`echo $size`_`echo $accesses` -- ./microbenchmarks/blocksized_ap.exe seq $size $accesses 1 0
#
#################################################################################################
#export size=8388608
#
#time -p -o blocksized_ap_pred_`echo $size`_`echo $accesses`_1_0_native.time ./microbenchmarks/blocksized_ap.exe seq $size $accesses 1 0
#
#echo "Running a microbenchmark..."
#time -p -o blocksized_ap_pred_`echo $size`_`echo $accesses`_1_0_pin.time \
#         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -predict_for_threads 1 -benchname blocksized_ap_pred_`echo $size`_`echo $accesses` -- ./microbenchmarks/blocksized_ap.exe seq $size $accesses 1 0
#
################################################################################################


rm -rf results
mkdir results
mv *blocksized_ap_seq*.csv blocksized_ap_seq*.time results

mv results blocksized_ap_seq`echo $size`_`echo $accesses`

