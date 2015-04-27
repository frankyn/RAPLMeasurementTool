/* Read the RAPL registers on a sandybridge-ep machine                */
/* Code based on Intel RAPL driver by Zhang Rui <rui.zhang@intel.com> */
/*                                                                    */
/* The /dev/cpu/??/msr driver must be enabled and permissions set     */
/* to allow read access for this to work.                             */
/*                                                                    */
/* Code to properly get this info from Linux through a real device    */
/*   driver and the perf tool should be available as of Linux 3.14    */
/* Compile with:   gcc -O2 -Wall -o rapl-read rapl-read.c -lm         */
/*                                                                    */
/* Vince Weaver -- vincent.weaver @ maine.edu -- 29 November 2013     */
/*                                                                    */
/* Additional contributions by:                                       */
/*   Romain Dolbeau -- romain @ dolbeau.org                           */
/*   NMSU Group -- 	Frank N., Tasha N., and Abdula (NEED LAST INTIAL) */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <inttypes.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <signal.h>
#include "core_util_api.h"


#ifndef RAPL_READ_API_HEADER 
#define RAPL_READ_API_HEADER

#define MSR_RAPL_POWER_UNIT		    0x606

/*
 * Platform specific RAPL Domains.
 * Note that PP1 RAPL Domain is supported on 062A only
 * And DRAM RAPL Domain is supported on 062D only
 */
/* Package RAPL Domain */
#define MSR_PKG_RAPL_POWER_LIMIT	0x610
#define MSR_PKG_ENERGY_STATUS		  0x611
#define MSR_PKG_PERF_STATUS		    0x613
#define MSR_PKG_POWER_INFO		    0x614

/* PP0 RAPL Domain */
#define MSR_PP0_POWER_LIMIT		    0x638
#define MSR_PP0_ENERGY_STATUS		  0x639
#define MSR_PP0_POLICY			      0x63A
#define MSR_PP0_PERF_STATUS		    0x63B

/* PP1 RAPL Domain, may reflect to uncore devices */
#define MSR_PP1_POWER_LIMIT		    0x640
#define MSR_PP1_ENERGY_STATUS		  0x641
#define MSR_PP1_POLICY			      0x642

/* DRAM RAPL Domain */
#define MSR_DRAM_POWER_LIMIT		  0x618
#define MSR_DRAM_ENERGY_STATUS		0x619
#define MSR_DRAM_PERF_STATUS		  0x61B
#define MSR_DRAM_POWER_INFO		    0x61C

/* RAPL UNIT BITMASK */
#define POWER_UNIT_OFFSET	        0
#define POWER_UNIT_MASK		        0x0F

#define ENERGY_UNIT_OFFSET	      0x08
#define ENERGY_UNIT_MASK	        0x1F00

#define TIME_UNIT_OFFSET	        0x10
#define TIME_UNIT_MASK		        0xF000

#define CPU_SANDYBRIDGE		        42
#define CPU_SANDYBRIDGE_EP 	      45
#define CPU_IVYBRIDGE		          58
#define CPU_IVYBRIDGE_EP	        62
#define CPU_HASWELL		            60
#define CPU_HASWELL_EP		        70

// Per-core structure to maintain data collected
struct CoreData{
  int32_t   core_id;
  int32_t   msr_fd; 
  double    power_units;
	double    energy_units;
	double    time_units;
  double    package_before;
	double    package_after;
  double    pp0_before;
	double    pp0_after;
  double    pp1_before;
  double    pp1_after;
 	double    dram_before; /* Shouldn't be available    */
	double    dram_after;  /* in non-server-use CPU's.  */
  double    thermal_spec_power;
	double    minimum_power;
	double    maximum_power;
	double    time_window;
  double    pkg_power_limit_1;
  double    pkg_time_window_1;
  double    pkg_power_limit_2;
  double    pkg_time_window_2;
  int32_t    pp0_policy;
  int32_t    pp1_policy;
  double    acc_pkg_throttled_time; /* SandyBridge & IvyBridge */
  double    acc_pp0_throttled_time; /* SandyBridge & IvyBridge */
};

// Open Model Specific Register (If not available use "sudo modprobe msr")
int32_t open_msr(int32_t core);

// Read a specific value from the register
long long read_msr(int32_t fd, int32_t which);

// Detect if RAPL is available for the current architecture
int32_t detect_cpu(void);

// Poll each core for RAPL data
int32_t poll_cores(int32_t cores, struct CoreData * core_data);

// Extract each core RAPL Data baseline data
int32_t core_data_baseline(int32_t core, struct CoreData * core_data);

// Extract each core RRPL Data elapsed data
int32_t core_data_elapsed(int32_t core, struct CoreData * core_data);

// Print all core data
int32_t print_core_data(int32_t cores, struct CoreData * core_data);

// Handle termination gracefully
void handle_signal(int32_t signal);

// Print Usage for this program
void    usage();

// Usage string
static uint8_t const* const USAGE_STRING="Usage: %s -c [CORES] -s [SLEEP]\n";

// PROGRAM NAME string
uint8_t * PROGRAM_NAME;

// Program Status 
uint8_t   poll_status;

// Define STATUSES
#define TRUE  1
#define FALSE 0


// CPU Model container
int32_t cpu_model;

#endif

