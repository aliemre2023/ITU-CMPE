#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include "utils.c"

int running = 1;
int total_runtime = 0;
int current_runtime = 0;
char job_name[30];

int main(int argc, char *argv[]){
    if(argc != 5){
        printf("Error! %s <job_name> <arrive_time> <priority_level> <run_time>\n", argv[0]);
        return 1;
    }

    char log_message[256];

    strcpy(job_name, argv[1]);
    total_runtime = atoi(argv[4]);

    // Job execution loop
    do {
        current_runtime++;

        snprintf(log_message, sizeof(log_message), "[JOB.c] - Job %s running: %d/%d seconds completed", job_name, current_runtime, total_runtime);
        write_log(log_message);

        sleep(1);
    } while(current_runtime < total_runtime);

    return 0;
}