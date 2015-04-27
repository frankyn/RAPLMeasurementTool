#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>

#ifndef CORE_UTIL_API_HEADER
#define CORE_UTIL_API_HEADER
 
#define BUF_MAX 1024
#define MAX_CPU 128 // this is defined as the maximum cores.
#define PROC_STAT_OPT_LINE 10

/* Container for CPU Statistics per core and total usage */
struct CPUStats {
   int32_t  cores;
   double   per_core[MAX_CPU];
   double   total_usage;
   unsigned long long int total_tick[MAX_CPU];
   unsigned long long int total_tick_old[MAX_CPU];
   unsigned long long int idle[MAX_CPU];
   unsigned long long int idle_old[MAX_CPU];
   unsigned long long int del_total_tick[MAX_CPU];
   unsigned long long int del_idle[MAX_CPU];
   unsigned long long int fields[PROC_STAT_OPT_LINE];
};

/* Find available cores */
void cpu_availability (  FILE * proc_stat_fd, struct CPUStats * stats );

/* Gather statistics from the current state of cores */
void cpu_utilization ( FILE * proc_stat_fd, struct CPUStats * stats );

/* Reads information from /proc/stat file */
int read_fields (FILE * proc_stat_fd, unsigned long long int * fields);

/* Gets a file descriptor for /proc/stat file */
FILE * get_proc_stat ( );

#endif
