#include "rapl_read_api.h"
#include "core_util_api.h"

int32_t main(int32_t argc, char **argv) {
  // Container for stats  
  struct CPUStats stats;
  memset(&stats, 0x0, sizeof(struct CPUStats)); 

  // Store program name
  PROGRAM_NAME = argv[0];

  // Total available cores  
  // * Should be updated to automatically detect this value *
  int32_t total_cores = 1;
  int32_t core_index;

  // Timeout
  int32_t sleep_timer = 1;
  
  // Used to parse program options
  int32_t c; 

  // Array of Cores
  struct CoreData * cores_data;
  
  // Begin handling signals
  signal(SIGINT, handle_signal);

  // Allocate space for total cores 
  cores_data = (struct CoreData *)calloc(total_cores , sizeof(struct CoreData));
    
  // Detect the current architecture's CPU model
  cpu_model=detect_cpu();
  if (cpu_model<0) {
	  printf("Unsupported CPU type\n");
	  exit(-1);
  }
  
  // file descriptor for proc_file
  FILE * proc_file = get_proc_stat ( );
  
  // Core availability
  cpu_availability ( proc_file, &stats );  


  // Set status of program to true
  poll_status = TRUE;
  
  // Header for data
  printf("EC(J),E0,E1,E2,E3,E4,E5,E6,E7,TU%%,C0%%,C1%%,C2%%,C3%%,C4%%,C5%%,C6%%,C7%%\n");
    
  // Poll each core every sleep_timer seconds
  while ( poll_status ) { 
    poll_cores(total_cores,cores_data); 
    cpu_utilization ( proc_file , &stats );
    
    double consumption = cores_data[0].pp0_after-cores_data[0].pp0_before;
    // Consumption
    printf("%.6f", consumption);
   
    // Usage
    double total_usage_cores = 0.0f;    
    for ( core_index = 0 ; core_index < stats.cores-1; core_index++ ) {
      total_usage_cores += stats.per_core[core_index];
    }

    for ( core_index = 0 ; core_index < stats.cores-1; core_index++ ) {
      printf(",%3.2lf", consumption*(stats.per_core[core_index]/total_usage_cores));
    }
    printf(",%3.2lf", stats.total_usage);
    
    for ( core_index = 0 ; core_index < stats.cores-1; core_index++ ) {
      printf(",%3.2lf", stats.per_core[core_index]);
    }

    printf("\n");

  }
  
  // Close file proc_file
  fclose(proc_file);
  
  return 0;
}

void handle_signal ( int32_t sig ) {
  switch ( sig ) {
    case SIGINT:
      poll_status = FALSE;
    break;
    default:
      exit(0);
  }
}

// Print Usage for this program
void usage ( ) {
  printf ( USAGE_STRING, PROGRAM_NAME);
}

