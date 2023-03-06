/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * This program is based on rapl-read:
 * https://github.com/deater/uarch-configure/tree/master/rapl-read
 * 
 * compile with:
 * gcc -o ryzen ryzen.c -lm
 */


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

#include <sys/syscall.h>
#include <linux/perf_event.h>

#define AMD_MSR_PWR_UNIT 0xC0010299
#define AMD_MSR_CORE_ENERGY 0xC001029A
#define AMD_MSR_PACKAGE_ENERGY 0xC001029B

#define AMD_TIME_UNIT_MASK 0xF0000
#define AMD_ENERGY_UNIT_MASK 0x1F00
#define AMD_POWER_UNIT_MASK 0xF
#define STRING_BUFFER 1024

#define MAX_CPUS	1024
#define MAX_PACKAGES	16

static int total_cores=0,total_packages=0;
static int package_map[MAX_PACKAGES];

static int open_msr(int core) {

	char msr_filename[BUFSIZ];
	int fd;

	sprintf(msr_filename, "/dev/cpu/%d/msr", core);
	fd = open(msr_filename, O_RDONLY);
	if ( fd < 0 ) {
		if ( errno == ENXIO ) {
			fprintf(stderr, "rdmsr: No CPU %d\n", core);
			exit(2);
		} else if ( errno == EIO ) {
			fprintf(stderr, "rdmsr: CPU %d doesn't support MSRs\n",
					core);
			exit(3);
		} else {
			perror("rdmsr:open");
			fprintf(stderr,"Trying to open %s\n",msr_filename);
			exit(127);
		}
	}

	return fd;
}

static long long read_msr(int fd, unsigned int which) {

	uint64_t data;

	if ( pread(fd, &data, sizeof data, which) != sizeof data ) {
		perror("rdmsr:pread");
		exit(127);
	}

	return (long long)data;
}

static int rapl_msr_amd_core(int core) {
	unsigned int time_unit, energy_unit, power_unit;
	double time_unit_d, energy_unit_d, power_unit_d;
	
	double core_energy;
	double core_energy_delta;

	double package;
	double package_delta;

	int fd;
		
	fd = open_msr(core);
		
	int core_energy_units = read_msr(fd, AMD_MSR_PWR_UNIT);

	
	time_unit = (core_energy_units & AMD_TIME_UNIT_MASK) >> 16;
	energy_unit = (core_energy_units & AMD_ENERGY_UNIT_MASK) >> 8;
	power_unit = (core_energy_units & AMD_POWER_UNIT_MASK);
	
	time_unit_d = pow(0.5,(double)(time_unit));
	energy_unit_d = pow(0.5,(double)(energy_unit));
	power_unit_d = pow(0.5,(double)(power_unit));

	
	int core_energy_raw;
	int package_raw;

	// Read per core energy values
	
	for(int i = 1;;i++){
		core_energy_raw = read_msr(fd, AMD_MSR_CORE_ENERGY);
		core_energy = core_energy_raw * energy_unit_d;

		//package_raw = read_msr(fd, AMD_MSR_PACKAGE_ENERGY);
		//package = package_raw*energy_unit_d;
		
		usleep(1000);
	
		core_energy_raw = read_msr(fd, AMD_MSR_CORE_ENERGY);
		core_energy_delta = core_energy_raw * energy_unit_d;

                //package_raw = read_msr(fd, AMD_MSR_PACKAGE_ENERGY);
                //package_delta = package_raw*energy_unit_d;
	
		double diff = (core_energy_delta - core_energy)*100;
		//double diff = (package_delta-package)*100;
		fprintf(stdout, "%g, %d\n", diff,i);
		fflush(stdout);
	}
	close(core);
	
	return 0;
}

int main(int argc, char **argv) {

	rapl_msr_amd_core(0);
	return 0;
}
