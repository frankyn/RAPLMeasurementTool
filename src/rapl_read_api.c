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
#include "rapl_read_api.h"


// Open Model Specific Register (If not available use "sudo modprobe msr")
int32_t open_msr(int32_t core) {

  char msr_filename[BUFSIZ];
  int32_t fd;

  sprintf(msr_filename, "/dev/cpu/%d/msr", core);
  fd = open(msr_filename, O_RDONLY);
  if ( fd < 0 ) {
    if ( errno == ENXIO ) {
      fprintf(stderr, "rdmsr: No CPU %d\n", core);
      exit(2);
    } else if ( errno == EIO ) {
      fprintf(stderr, "rdmsr: CPU %d doesn't support MSRs\n", core);
      exit(3);
    } else {
      perror("rdmsr:open");
      fprintf(stderr,"Trying to open %s\n",msr_filename);
      exit(127);
    }
  }

  return fd;
}

long long read_msr(int32_t fd, int32_t which) {

  uint64_t data;

  if ( pread(fd, &data, sizeof data, which) != sizeof data ) {
    perror("rdmsr:pread");
    exit(127);
  }

  return (long long)data;
}


int32_t detect_cpu(void) {

	FILE *fff;

	int32_t family,model=-1;
	char buffer[BUFSIZ],*result;
	char vendor[BUFSIZ];

	fff=fopen("/proc/cpuinfo","r");
	if (fff==NULL) return -1;

	while(1) {
		result=fgets(buffer,BUFSIZ,fff);
		if (result==NULL) break;

		if (!strncmp(result,"vendor_id",8)) {
			sscanf(result,"%*s%*s%s",vendor);

			if (strncmp(vendor,"GenuineIntel",12)) {
				printf("%s not an Intel chip\n",vendor);
				return -1;
			}
		}

		if (!strncmp(result,"cpu family",10)) {
			sscanf(result,"%*s%*s%*s%d",&family);
			if (family!=6) {
				printf("Wrong CPU family %d\n",family);
				return -1;
			}
		}

		if (!strncmp(result,"model",5)) {
			sscanf(result,"%*s%*s%d",&model);
		}

	}

	fclose(fff);

	switch(model) {
		case CPU_SANDYBRIDGE:
			printf("Found Sandybridge CPU\n");
			break;
		case CPU_SANDYBRIDGE_EP:
			printf("Found Sandybridge-EP CPU\n");
			break;
		case CPU_IVYBRIDGE:
			printf("Found Ivybridge CPU\n");
			break;
		case CPU_IVYBRIDGE_EP:
			printf("Found Ivybridge-EP CPU\n");
			break;
		case CPU_HASWELL:
			printf("Found Haswell CPU\n");
			break;
		case CPU_HASWELL_EP:
			printf("Found Haswell-EP CPU\n");
			break;
		default:	printf("Unsupported model %d\n",model);
				model=-1;
				break;
	}

	return model;
}


int32_t poll_cores ( int32_t cores , struct CoreData * core_data ) {
  int32_t core_index;
  int32_t cpu_model;

  
  // Collect baseline data per core
  for ( core_index = 0 ; core_index < cores ; ++core_index ) {
    core_data[core_index].msr_fd = open_msr(core_index);
    core_data_baseline ( core_index, core_data );
  }
  
  //printf("\nSleeping 1 second\n\n");
  sleep(1);
  
  // Collect after data per core
  for ( core_index = 0 ; core_index < cores ; ++core_index ) {
    core_data_elapsed ( core_index, core_data );
    close(core_data[core_index].msr_fd);
  }
  

}

// Extract each core RRPL Data baseline data
int32_t core_data_baseline(int32_t core, struct CoreData * core_data) {
  
  long long result;
  
  //printf("Checking core #%d\n",core);
  core_data[core].core_id = core;
  core_data[core].pp0_before = 0.0f;
  core_data[core].pp1_before = 0.0f;
  core_data[core].dram_before = 0.0f;  

  /* Calculate the units used */
  result=read_msr(core_data[core].msr_fd,MSR_RAPL_POWER_UNIT);
  
  core_data[core].power_units=pow(0.5,(double)(result&0xf));
  core_data[core].energy_units=pow(0.5,(double)((result>>8)&0x1f));
  core_data[core].time_units=pow(0.5,(double)((result>>16)&0xf));

  
  /* Show package power info */
  result=read_msr(core_data[core].msr_fd,MSR_PKG_POWER_INFO);
  core_data[core].thermal_spec_power=core_data[core].power_units*(double)(result&0x7fff);
  core_data[core].minimum_power=core_data[core].power_units*(double)((result>>16)&0x7fff);
  core_data[core].maximum_power=core_data[core].power_units*(double)((result>>32)&0x7fff);
  core_data[core].time_window=core_data[core].time_units*(double)((result>>48)&0x7fff);

  /* Show package power limit */
  //result=read_msr(core_data[core].msr_fd,MSR_PKG_RAPL_POWER_LIMIT);
  result=read_msr(core_data[core].msr_fd,MSR_PKG_RAPL_POWER_LIMIT);
  core_data[core].pkg_power_limit_1 = core_data[core].power_units*(double)((result>>0)&0x7FFF);
  core_data[core].pkg_time_window_1 = core_data[core].time_units*(double)((result>>17)&0x007F);
  core_data[core].pkg_power_limit_2 = core_data[core].power_units*(double)((result>>32)&0x7FFF);
  core_data[core].pkg_time_window_2 = core_data[core].time_units*(double)((result>>49)&0x007F);

  result=read_msr(core_data[core].msr_fd,MSR_PKG_ENERGY_STATUS);
  core_data[core].package_before=(double)result*core_data[core].energy_units;

  /* only available on *Bridge-EP */
  if ((cpu_model==CPU_SANDYBRIDGE_EP) || (cpu_model==CPU_IVYBRIDGE_EP))
  {
    result=read_msr(core_data[core].msr_fd,MSR_PKG_PERF_STATUS);
    core_data[core].acc_pkg_throttled_time=(double)result*core_data[core].time_units;
  }  

  result=read_msr(core_data[core].msr_fd,MSR_PP0_ENERGY_STATUS);
  core_data[core].pp0_before=(double)result*core_data[core].energy_units;

  result=read_msr(core_data[core].msr_fd,MSR_PP0_POLICY);
  core_data[core].pp0_policy=(int)result&0x001f;

  /* only available on *Bridge-EP */
  if ((cpu_model==CPU_SANDYBRIDGE_EP) || (cpu_model==CPU_IVYBRIDGE_EP))
  {
    result=read_msr(core_data[core].msr_fd,MSR_PP0_PERF_STATUS);
    core_data[core].acc_pp0_throttled_time=(double)result*core_data[core].time_units;
  }

  /* not available on *Bridge-EP */
  if ((cpu_model==CPU_SANDYBRIDGE) || (cpu_model==CPU_IVYBRIDGE) ||
	(cpu_model==CPU_HASWELL) || (cpu_model==CPU_HASWELL_EP)) {
     result=read_msr(core_data[core].msr_fd,MSR_PP1_ENERGY_STATUS);
     core_data[core].pp1_before=(double)result*core_data[core].energy_units;
     
     result=read_msr(core_data[core].msr_fd,MSR_PP1_POLICY);
     core_data[core].pp1_policy=(int)result&0x001f;
  }

	/* Despite documentation saying otherwise, it looks like */
	/* You can get DRAM readings on regular Haswell          */
  if ((cpu_model==CPU_SANDYBRIDGE_EP) || (cpu_model==CPU_IVYBRIDGE_EP) ||
	(cpu_model==CPU_HASWELL)  || (cpu_model==CPU_HASWELL_EP)) {
     result=read_msr(core_data[core].msr_fd,MSR_DRAM_ENERGY_STATUS);
     core_data[core].dram_before=(double)result*core_data[core].energy_units;
  }
}

// Extract each core RRPL Data elapsed data
int32_t core_data_elapsed ( int32_t core , struct CoreData * core_data ) {  
  long long result;
  
  if ( core_data[core].core_id != core ) { exit ( 0 ); }
  result=read_msr(core_data[core].msr_fd,MSR_PKG_ENERGY_STATUS);
  core_data[core].package_after=(double)result*core_data[core].energy_units;
  
  result=read_msr(core_data[core].msr_fd,MSR_PP0_ENERGY_STATUS);
  core_data[core].pp0_after=(double)result*core_data[core].energy_units;
  
  /* not available on SandyBridge-EP */
  if ((cpu_model==CPU_SANDYBRIDGE) || (cpu_model==CPU_IVYBRIDGE) ||
	(cpu_model==CPU_HASWELL) || (cpu_model==CPU_HASWELL_EP) ) {
     result=read_msr(core_data[core].msr_fd,MSR_PP1_ENERGY_STATUS);
     core_data[core].pp1_after=(double)result*core_data[core].energy_units;
  }

  if ((cpu_model==CPU_SANDYBRIDGE_EP) || (cpu_model==CPU_IVYBRIDGE_EP) ||
	(cpu_model==CPU_HASWELL) || (cpu_model==CPU_HASWELL_EP) ) {
     result=read_msr(core_data[core].msr_fd,MSR_DRAM_ENERGY_STATUS);
     core_data[core].dram_after=(double)result*core_data[core].energy_units;
  }


}


// Print all core data
int32_t print_core_data(int32_t cores, struct CoreData * core_data) {
  int32_t core;
  
  for ( core = 0 ; core < cores ; core++ ) {
    printf("###################CORE[%d]###################\n",core_data[core].core_id);
    printf("Power units = %.3fW\n",core_data[core].power_units);
    printf("Energy units = %.8fJ\n",core_data[core].energy_units);
    printf("Time units = %.8fs\n",core_data[core].time_units);
    
    printf("Package thermal spec: %.3fW\n",
                          core_data[core].thermal_spec_power);  
    printf("Package minimum power: %.3fW\n",core_data[core].minimum_power);
    printf("Package maximum power: %.3fW\n",core_data[core].maximum_power);    
    printf("Package maximum time window: %.6fs\n",core_data[core].time_window);
    
    //printf("Package power limits are %s\n", (core_data->result >> 63) ? "locked" : "unlocked");
    
    printf("Package power limit #1: %.3fW for %.6fs\n",  //(%s, %s)\n", 
             core_data[core].pkg_power_limit_1, 
             core_data[core].pkg_time_window_1);
    //         (core_data->result & (1LL<<15)) ? "enabled" : "disabled",
    //         (core_data->result & (1LL<<16)) ? "clamped" : "not_clamped");
    
    //double pkg_power_limit_2 = core_data->power_units*(double)((core_data->result>>32)&0x7FFF);
    //double pkg_time_window_2 = core_data->time_units*(double)((core_data->result>>49)&0x007F);
    printf("Package power limit #2: %.3fW for %.6fs\n", //(%s, %s)\n", 
              core_data[core].pkg_power_limit_2, 
              core_data[core].pkg_time_window_2);
    //        (core_data->result & (1LL<<47)) ? "enabled" : "disabled",
    //        (core_data->result & (1LL<<48)) ? "clamped" : "not_clamped");
    
    printf("PowerPlane0 (core) for core %d energy before: %.6fJ\n",
                        core,
                        core_data[core].pp0_before);
    
    printf("Package energy before: %.6fJ\n",core_data[core].package_before);

    /* only available on *Bridge-EP */
    if ((cpu_model==CPU_SANDYBRIDGE_EP) || (cpu_model==CPU_IVYBRIDGE_EP)) {
      printf("Accumulated Package Throttled Time : %.6fs\n",
                            core_data[core].acc_pkg_throttled_time);
    }

  
    /* only available on *Bridge-EP */
    if ((cpu_model==CPU_SANDYBRIDGE_EP) || (cpu_model==CPU_IVYBRIDGE_EP)) {
      printf("PowerPlane0 (core) Accumulated Throttled Time : %.6fs\n",
                            core_data[core].acc_pp0_throttled_time);
    }

    /* not available on *Bridge-EP */
    if ((cpu_model==CPU_SANDYBRIDGE) || (cpu_model==CPU_IVYBRIDGE) ||
	  (cpu_model==CPU_HASWELL) || (cpu_model==CPU_HASWELL)) {
       printf("PowerPlane1 (on-core GPU if avail) before: %.6fJ\n",core_data[core].pp1_before);
       printf("PowerPlane1 (on-core GPU if avail) %d policy: %d\n",core,core_data[core].pp1_policy);
    }

	  /* Despite documentation saying otherwise, it looks like */
	  /* You can get DRAM readings on regular Haswell          */
    if ((cpu_model==CPU_SANDYBRIDGE_EP) || (cpu_model==CPU_IVYBRIDGE_EP) ||
	  (cpu_model==CPU_HASWELL) || (cpu_model==CPU_HASWELL)) {
       printf("DRAM energy before: %.6fJ\n",core_data[core].dram_before);
    }
    
    // AFTER RUN
    
    printf("Package energy after: %.6f  (%.6fJ consumed)\n",
	    core_data[core].package_after,
      core_data[core].package_after-core_data[core].package_before);
    
    printf("PowerPlane0 (core) for core %d energy after: %.6f  (%.6fJ consumed)\n",
	    core,
      core_data[core].pp0_after,
      core_data[core].pp0_after-core_data[core].pp0_before);
  
    /* not available on SandyBridge-EP */
    if ((cpu_model==CPU_SANDYBRIDGE) || (cpu_model==CPU_IVYBRIDGE) ||
	  (cpu_model==CPU_HASWELL) || (cpu_model==CPU_HASWELL) ) {
       printf("PowerPlane1 (on-core GPU if avail) after: %.6f  (%.6fJ consumed)\n",
	     core_data[core].pp1_after,
       core_data[core].pp1_after-core_data[core].pp1_before);
    }

    if ((cpu_model==CPU_SANDYBRIDGE_EP) || (cpu_model==CPU_IVYBRIDGE_EP) ||
	  (cpu_model==CPU_HASWELL) || (cpu_model==CPU_HASWELL_EP)) {
       printf("DRAM energy after: %.6f  (%.6fJ consumed)\n",
	     core_data[core].dram_after,
       core_data[core].dram_after-core_data[core].dram_before);
    }
    printf("##################COREEND[%d]#################\n",core);
    
  }
}

