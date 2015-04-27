#include "core_util_api.h"

/* Reads information from /proc/stat file */
int read_fields (FILE *fp, unsigned long long int *fields)
{
  int retval;
  char buffer[BUF_MAX];
 
 
  if (!fgets (buffer, BUF_MAX, fp))
  { perror ("Error"); }
  /* line starts with c and a string. This is to handle cpu, cpu[0-9]+ */
  retval = sscanf (buffer, "c%*s %Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu",
                            &fields[0], 
                            &fields[1], 
                            &fields[2], 
                            &fields[3], 
                            &fields[4], 
                            &fields[5], 
                            &fields[6], 
                            &fields[7], 
                            &fields[8], 
                            &fields[9]); 
  if (retval == 0)
  { return -1; }
  if (retval < 4) /* Atleast 4 fields is to be read */
  {
    fprintf (stderr, "Error reading /proc/stat cpu field\n");
    return 0;
  }
  return 1;
}

/* Gets a file descriptor for /proc/stat file */
FILE * get_proc_stat ( ) {
  
  FILE * fp = fopen ("/proc/stat", "r");
  
  if (fp == NULL) {
    perror ("fopen");
    exit(0);
  }
  
  return fp;
}

/* Find available cores */
void cpu_availability (  FILE * proc_stat_fd, struct CPUStats * stats ) {
  int32_t i;
  stats->cores = 0;
  while (read_fields (proc_stat_fd, stats->fields) != -1) {
    // Collects data per core availability
    for (i=0, stats->total_tick[stats->cores] = 0; i<PROC_STAT_OPT_LINE; i++) { 
      stats->total_tick[stats->cores] += stats->fields[i]; 
    }
    stats->idle[stats->cores] = stats->fields[3]; /* idle ticks index */

    // Increment to next cpu after total ticks per cpu is collected
    stats->cores++;
  }

}

/* Gather statistics from the current state of cores */
void cpu_utilization ( FILE * proc_stat_fd, struct CPUStats * stats ) {
  int32_t i;
  int32_t count;
  double  percent_usage=0;
 
  // Rewind by to the starting point of the file /proc/stat
  fseek (proc_stat_fd, 0, SEEK_SET);
  fflush (proc_stat_fd);

  for (count = 0; count < stats->cores; count++) { 
    stats->total_tick_old[count] = stats->total_tick[count];
    stats->idle_old[count] = stats->idle[count];

    // read data from /proc/stat 
    if (!read_fields (proc_stat_fd,  stats->fields )) { 
      // failed to read from /proc/stat
      printf("Unable to read /proc/stat");
      exit(0); 
    }
    
    // depending on ticks available from first line (currently 10)
    for (i=0, stats->total_tick[count] = 0; i<PROC_STAT_OPT_LINE; i++) {
      stats->total_tick[count] += stats->fields[i]; 
    }

    stats->idle[count] = stats->fields[3];

    stats->del_total_tick[count] = stats->total_tick[count] - stats->total_tick_old[count];
    stats->del_idle[count] = stats->idle[count] - stats->idle_old[count];

    percent_usage = ((stats->del_total_tick[count] - stats->del_idle[count]) / (double) stats->del_total_tick[count]) * 100;
    if (count == 0) {
      stats->total_usage = percent_usage; 
    } else { 
      stats->per_core[count-1] = percent_usage;
    }
  }

}


