#!/bin/sh

# Run BlackScholes with Thread Counts ( 1 2 4 8 16 )
# Make sure everything is built for blackscholes
parsecmgmt -a build -p blackscholes

# Start analyzer before each iteration and kill 
#    after iteration completes 1 Thread
sudo ../bin/rapl_measurement > data/BLACKSCHOLES_N_1_I_Native.out &
parsecmgmt -a run   -p blackscholes -n 1 -i native > parsec_logs/BLACKSCHOLES_N_1_I_Native_PARSEC.out
sudo killall rapl_measurement -s SIGINT

#    after iteration completes 2 Thread
sudo ../bin/rapl_measurement > data/BLACKSCHOLES_N_2_I_Native.out &
parsecmgmt -a run   -p blackscholes -n 2 -i native > parsec_logs/BLACKSCHOLES_N_2_I_Native_PARSEC.out
sudo killall rapl_measurement -s SIGINT

#    after iteration completes 4 Thread
sudo ../bin/rapl_measurement > data/BLACKSCHOLES_N_4_I_Native.out &
parsecmgmt -a run   -p blackscholes -n 4 -i native > parsec_logs/BLACKSCHOLES_N_4_I_Native_PARSEC.out
sudo killall rapl_measurement -s SIGINT

#    after iteration completes 8 Thread
sudo ../bin/rapl_measurement > data/BLACKSCHOLES_N_8_I_Native.out &
parsecmgmt -a run   -p blackscholes -n 8 -i native > parsec_logs/BLACKSCHOLES_N_8_I_Native_PARSEC.out
sudo killall rapl_measurement -s SIGINT

#    after iteration completes 16 Thread
sudo ../bin/rapl_measurement > data/BLACKSCHOLES_N_16_I_Native.out &
parsecmgmt -a run   -p blackscholes -n 16 -i native > parsec_logs/BLACKSCHOLES_N_16_I_Native_PARSEC.out
sudo killall rapl_measurement -s SIGINT

# Start x264 Benchmark
# Same as before 
parsecmgmt -a build -p x264

#    after iteration completes 1 Thread
sudo ../bin/rapl_measurement > data/X264_N_1_I_Native.out &
parsecmgmt -a run   -p x264 -n 1 -i native > parsec_logs/x264_N_1_I_Native_PARSEC.out
sudo killall rapl_measurement -s SIGINT

#    after iteration completes 2 Thread
sudo ../bin/rapl_measurement > data/X264_N_2_I_Native.out &
parsecmgmt -a run   -p x264 -n 2 -i native > parsec_logs/x264_N_2_I_Native_PARSEC.out
sudo killall rapl_measurement -s SIGINT

#    after iteration completes 4 Thread
sudo ../bin/rapl_measurement > data/X264_N_4_I_Native.out &
parsecmgmt -a run   -p x264 -n 4 -i native > parsec_logs/x264_N_4_I_Native_PARSEC.out
sudo killall rapl_measurement -s SIGINT

#    after iteration completes 8 Thread
sudo ../bin/rapl_measurement > data/X264_N_8_I_Native.out &
parsecmgmt -a run   -p x264 -n 8 -i native > parsec_logs/x264_N_8_I_Native_PARSEC.out
sudo killall rapl_measurement -s SIGINT

#    after iteration completes 16 Thread
sudo ../bin/rapl_measurement > data/X264_N_16_I_Native.out &
parsecmgmt -a run   -p x264 -n 16 -i native > parsec_logs/x264_N_16_I_Native_PARSEC.out
sudo killall rapl_measurement -s SIGINT

#    after iteration completes 32 Thread
sudo ../bin/rapl_measurement > data/X264_N_32_I_Native.out &
parsecmgmt -a run   -p x264 -n 32 -i native > parsec_logs/x264_N_32_I_Native_PARSEC.out
sudo killall rapl_measurement -s SIGINT

# Build Canneal app
parsecmgmt -a build -p canneal 

# Start streamcluster Benchmark
sudo ../bin/rapl_measurement > data/CANNEAL_N_1_I_Native.out &
parsecmgmt -a run   -p canneal -n 1 -i native > parsec_logs/canneal_N_1_I_Native_PARSEC.out
sudo killall rapl_measurement -s SIGINT

sudo ../bin/rapl_measurement > data/CANNEAL_N_2_I_Native.out &
parsecmgmt -a run   -p canneal -n 2 -i native > parsec_logs/canneal_N_2_I_Native_PARSEC.out
sudo killall rapl_measurement -s SIGINT

sudo ../bin/rapl_measurement > data/CANNEAL_N_4_I_Native.out &
parsecmgmt -a run   -p canneal -n 4 -i native > parsec_logs/canneal_N_4_I_Native_PARSEC.out
sudo killall rapl_measurement -s SIGINT

sudo ../bin/rapl_measurement > data/CANNEAL_N_8_I_Native.out &
parsecmgmt -a run   -p canneal -n 8 -i native > parsec_logs/canneal_N_8_I_Native_PARSEC.out
sudo killall rapl_measurement -s SIGINT

sudo ../bin/rapl_measurement > data/CANNEAL_N_16_I_Native.out &
parsecmgmt -a run   -p canneal -n 16 -i native > parsec_logs/canneal_N_16_I_Native_PARSEC.out
sudo killall rapl_measurement -s SIGINT
