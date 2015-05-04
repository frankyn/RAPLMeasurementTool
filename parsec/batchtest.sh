sudo ../bin/rapl_measurement > data/CANNEAL_N_8_I_Native.out &
parsecmgmt -a run   -p canneal -n 8 -i native > parsec_logs/canneal_N_8_I_Native_PARSEC.out
sudo killall rapl_measurement -s SIGINT

