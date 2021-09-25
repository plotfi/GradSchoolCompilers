#!/bin/bash

echo ""
echo "iw(1) ooo(0) bhr(8)"
../../../pin -t ../lab2/obj-intel64/sim.so -readtrace 1 -printinst 0 -max_sim_count 0 -max_inst_count 1000000  -print_pipe_freq 0 -tracename firefoxtrace.pzip -issue_width 1 -ooo_scheduler 0 -ghr_length 8 -debug_print 0  -o sim.out -sched_size 8 -rob_size 64  -- /usr/lib/firefox-3.6.8/firefox-bin

echo ""
echo "iw(2) ooo(0) bhr(8)"
../../../pin -t ../lab2/obj-intel64/sim.so -readtrace 1 -printinst 0 -max_sim_count 0 -max_inst_count 1000000  -print_pipe_freq 0 -tracename firefoxtrace.pzip -issue_width 2 -ooo_scheduler 0 -ghr_length 8 -debug_print 0  -o sim.out -sched_size 8 -rob_size 64  -- /usr/lib/firefox-3.6.8/firefox-bin

echo ""
echo "iw(3) ooo(0) bhr(8)"
../../../pin -t ../lab2/obj-intel64/sim.so -readtrace 1 -printinst 0 -max_sim_count 0 -max_inst_count 1000000  -print_pipe_freq 0 -tracename firefoxtrace.pzip -issue_width 3 -ooo_scheduler 0 -ghr_length 8 -debug_print 0  -o sim.out -sched_size 8 -rob_size 64  -- /usr/lib/firefox-3.6.8/firefox-bin


echo ""
echo "iw(1) ooo(1) bhr(8)"
../../../pin -t ../lab2/obj-intel64/sim.so -readtrace 1 -printinst 0 -max_sim_count 0 -max_inst_count 1000000  -print_pipe_freq 0 -tracename firefoxtrace.pzip -issue_width 1 -ooo_scheduler 1 -ghr_length 8 -debug_print 0  -o sim.out -sched_size 8 -rob_size 64  -- /usr/lib/firefox-3.6.8/firefox-bin

echo ""
echo "iw(2) ooo(1) bhr(8)"
../../../pin -t ../lab2/obj-intel64/sim.so -readtrace 1 -printinst 0 -max_sim_count 0 -max_inst_count 1000000  -print_pipe_freq 0 -tracename firefoxtrace.pzip -issue_width 2 -ooo_scheduler 1 -ghr_length 8 -debug_print 0  -o sim.out -sched_size 8 -rob_size 64  -- /usr/lib/firefox-3.6.8/firefox-bin

echo ""
echo "iw(3) ooo(1) bhr(8)"
../../../pin -t ../lab2/obj-intel64/sim.so -readtrace 1 -printinst 0 -max_sim_count 0 -max_inst_count 1000000  -print_pipe_freq 0 -tracename firefoxtrace.pzip -issue_width 3 -ooo_scheduler 1 -ghr_length 8 -debug_print 0  -o sim.out -sched_size 8 -rob_size 64  -- /usr/lib/firefox-3.6.8/firefox-bin


