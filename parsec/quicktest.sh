#!/bin/sh


#    after iteration completes 16 Thread
sudo ../bin/rapl_measurement > data/X264_N_16_I_Native.out &
parsecmgmt -a run   -p x264 -n 16 -i native > parsec_logs/x264_N_16_I_Native_PARSEC.out
sudo killall rapl_measurement -s SIGINT

#    after iteration completes 32 Thread
sudo ../bin/rapl_measurement > data/X264_N_32_I_Native.out &
parsecmgmt -a run   -p x264 -n 32 -i native > parsec_logs/x264_N_32_I_Native_PARSEC.out
sudo killall rapl_measurement -s SIGINT
