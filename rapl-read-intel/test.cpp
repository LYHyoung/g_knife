#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <signal.h>
#include "Rapl.h"
int ms_pause = 1;


int main(int argc, char* argv[]){
      Rapl *rapl = new Rapl();
    while(1){ 
            usleep(ms_pause*1000);
            rapl->sample();
	    fprintf(stdout, "%lf, %lf\n",rapl->pp0_current_power(),rapl->total_time());
            fflush(stdout);
    }
    _exit(0);
}
