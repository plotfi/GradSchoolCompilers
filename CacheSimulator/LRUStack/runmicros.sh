#!/bin/dash

if [ -z $1 ]
then
    echo "$0 <size> <accesses> <sample rate> <sample burst>"
    exit
fi

if [ -z $2 ]
then
    echo "$0 <size> <accesses> <sample rate> <sample burst>"
    exit
fi

if [ -z $3 ]
then
    echo "$0 <size> <accesses> <sample rate> <sample burst>"
    exit
fi

if [ -z $4 ]
then
    echo "$0 <size> <accesses> <sample rate> <sample burst>"
    exit
fi

export LD_LIBRARY_PATH=/opt/intel/cc/latest/lib:$LD_LIBRARY_PATH

echo "size $1"
echo "accesses $2"
echo "sample rate $3"
echo "sample burst $4"

export size=$1
export accesses=$2
export samplerate=$3
export sampleburst=$4

echo "Running a microbenchmark..."
time -p -o even_ap_seq_`echo $size`_`echo $accesses`_1_0_native.time       ./microbenchmarks/even_ap.exe       seq $size $accesses 1 0
time -p -o even_ap_par_`echo $size`_`echo $accesses`_1_0_native.time       ./microbenchmarks/even_ap.exe       par $size $accesses 2 0

echo "Running a microbenchmark..."
time -p -o frontseq_ap_seq_`echo $size`_`echo $accesses`_1_0_native.time   ./microbenchmarks/frontseq_ap.exe   seq $size $accesses 1 0
time -p -o frontseq_ap_par_`echo $size`_`echo $accesses`_1_0_native.time   ./microbenchmarks/frontseq_ap.exe   par $size $accesses 2 0

#echo "Running a microbenchmark..."
#time -p -o pseudorand_ap_seq_`echo $size`_`echo $accesses`_1_0_native.time ./microbenchmarks/pseudorand_ap.exe seq $size $accesses 1 0
#time -p -o pseudorand_ap_par_`echo $size`_`echo $accesses`_1_0_native.time ./microbenchmarks/pseudorand_ap.exe par $size $accesses 2 0

echo "Running a microbenchmark..."
time -p -o blocksized_ap_seq_`echo $size`_`echo $accesses`_1_0_native.time ./microbenchmarks/blocksized_ap.exe seq $size $accesses 1 0
time -p -o blocksized_ap_par_`echo $size`_`echo $accesses`_1_0_native.time ./microbenchmarks/blocksized_ap.exe par $size $accesses 2 0


################################################################################################

echo "Running a microbenchmark..."
time -p -o even_ap_seq_`echo $size`_`echo $accesses`_1_0_pin.time       \
         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -benchname even_ap_seq       -- ./microbenchmarks/even_ap.exe seq $size $accesses 1 0

time -p -o even_ap_par_`echo $size`_`echo $accesses`_1_0_pin.time       \
         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -benchname even_ap_par       -- ./microbenchmarks/even_ap.exe par $size $accesses 2 0

################################################################################################

echo "Running a microbenchmark..."
time -p -o frontseq_ap_seq_`echo $size`_`echo $accesses`_1_0_pin.time   \
         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -benchname frontseq_ap_seq   -- ./microbenchmarks/frontseq_ap.exe   seq $size $accesses 1 0

time -p -o frontseq_ap_par_`echo $size`_`echo $accesses`_1_0_pin.time   \
         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -benchname frontseq_ap_par   -- ./microbenchmarks/frontseq_ap.exe   par $size $accesses 2 0

################################################################################################

#echo "Running a microbenchmark..."
#time -p -o pseudorand_ap_seq_`echo $size`_`echo $accesses`_1_0_pin.time \
#         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -benchname pseudorand_ap_seq -- ./microbenchmarks/pseudorand_ap.exe seq $size $accesses 1 0
#
#time -p -o pseudorand_ap_par_`echo $size`_`echo $accesses`_1_0_pin.time \
#         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -benchname pseudorand_ap_par -- ./microbenchmarks/pseudorand_ap.exe par $size $accesses 2 0

################################################################################################

echo "Running a microbenchmark..."
time -p -o blocksized_ap_seq_`echo $size`_`echo $accesses`_1_0_pin.time \
         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -benchname blocksized_ap_seq -- ./microbenchmarks/blocksized_ap.exe seq $size $accesses 1 0

time -p -o blocksized_ap_par_`echo $size`_`echo $accesses`_1_0_pin.time \
         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -benchname blocksized_ap_par -- ./microbenchmarks/blocksized_ap.exe par $size $accesses 2 0

################################################################################################

time -p -o empty_ap_`echo $size`_`echo $accesses`_1_0_pin.time \
         ../../../intel64/bin/pinbin -t obj-intel64/tracer.so -samplerate $samplerate -sampleburst $sampleburst -benchname empty_ap -- ./microbenchmarks/empty_ap.exe seq $size $accesses 1 0



rm -rf results
mkdir results
mv histogram*.csv *.time results
dirname=~/Dropbox/Suitability/code/puyan/benchmarks/results_`date +%j_%H%M`
mkdir $dirname
mv results/* $dirname

