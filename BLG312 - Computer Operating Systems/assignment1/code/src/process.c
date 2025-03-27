#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include "utils.c"
#include "job.h"

// Find the next job to run, according to given algorithm in assignment paper
int find_next_job(Job *jobs, int job_count, int current_job, int current_time) {
    int next_job = -1;
    int highest_priority = 9999;
    int earliest_arrival = 9999;
    int shortest_remaining = 9999;
    int earliest_index = 9999;
    
    // Highest Priority, except previous one
    for (int i = 0; i < job_count; i++) {
        if (i != current_job && jobs[i].arrival_time <= current_time && !jobs[i].is_completed && jobs[i].priority <= highest_priority) {
            // If highest priorties equal, take the earliest arrival
            if(jobs[i].priority == highest_priority){
                if (jobs[i].arrival_time < earliest_arrival) {
                    earliest_arrival = jobs[i].arrival_time;
                    next_job = i;
                } 
                // If arrival same, take whose remaining time less
                else if (jobs[i].arrival_time == earliest_arrival) {
                    if (jobs[i].remaining_time < shortest_remaining) {
                        shortest_remaining = jobs[i].remaining_time;
                        next_job = i;
                    } 
                    // If remaining times same, take smaller index
                    else if (jobs[i].remaining_time == shortest_remaining) {
                        if (i < earliest_index) {
                            earliest_index = i;
                            next_job = i;
                        }
                    }
                }
            }
            else {
                earliest_arrival = jobs[i].arrival_time;
                shortest_remaining = jobs[i].remaining_time;
                highest_priority = jobs[i].priority;
                earliest_index = i;
                next_job = i;

                /*
                For handle earliest index I don't need to do anything, 
                Also because of earliest index stuff, I need to keep indexes ordered
                */
            }     
        }
    }
    
    // If no other job is available, take the previous one
    if (next_job == -1 && current_job != -1 && !jobs[current_job].is_completed) {
        return current_job;
    }

    
    // If all jobs are completed, finish program 
    if (next_job == -1) {
        for (int i = 0; i < job_count; i++) {
            if (!jobs[i].is_completed && jobs[i].arrival_time <= current_time) {
                return i;
            }
        }
    }
    
    return next_job;
}

// Function to check if all jobs are completed
int all_jobs_completed(Job *jobs, int job_count) {
    for (int i = 0; i < job_count; i++) {
        if (!jobs[i].is_completed) {
            return 0;
        }
    }
    return 1;
}

Job jobs[MAX_JOBS];
int job_count = 0;
int time_quantum = 0;
void read_from_data(){
    char *items[MAX_ITEMS];
    char buffer[MAX_ITEM_LENGTH]; // Temporary buffer for line traversing
    char buffer_part[MAX_ITEM_LENGTH]; // Temporary buffer_part for word extraction
    int item_count = 0;

    // Open the file
    FILE* file = fopen("data/input/jobs.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "ERROR: Probably path is incorrect\n");
        return;
    }
    
    // Read each line
    while (item_count < MAX_ITEMS && fgets(buffer, MAX_ITEM_LENGTH, file) != NULL) {
        // Remove the newline character if exist
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
        
        // Reset buffer_part for each new line
        memset(buffer_part, 0, MAX_ITEM_LENGTH);
        int buffer_part_index = 0;
        
        for(unsigned int i = 0; i <= strlen(buffer); i++) { 
            // Process character or end of string
            if(buffer[i] == ' ' || buffer[i] == '\0') {
                /*
                That mean new word exist,
                Store it in to the items array
                */
                if(buffer_part_index > 0) { 
                    buffer_part[buffer_part_index] = '\0';
                    
                    // Allocate memory and copy the word
                    items[item_count] = (char*)malloc(strlen(buffer_part) + 1);
                    if (items[item_count] == NULL) {
                        fprintf(stderr, "Memory allocation failed\n");
                        fclose(file);
                    }
                    
                    strcpy(items[item_count], buffer_part);
                    item_count++;
                    
                    // Reset buffer_part for next word
                    memset(buffer_part, 0, MAX_ITEM_LENGTH);
                    buffer_part_index = 0;
                }
            } 
            else {
                // Add character to current word
                buffer_part[buffer_part_index] = buffer[i];
                buffer_part_index++;
            }
        }
    }
    
    // Close the file
    fclose(file);
    
    // Get the time quantum
    if (strcmp(items[0], "TimeSlice") == 0) {
        time_quantum = atoi(items[1]);
    } else {
        fprintf(stderr, "ERROR: Invalid input format\n");
        return;
    }
    
    // Parse the jobs from the items array
    int item_index = 2; // Start from the third item (after TimeSlice string and its value)
    
    while (item_index < item_count) {
        strcpy(jobs[job_count].name, items[item_index++]);
        jobs[job_count].arrival_time = atoi(items[item_index++]);
        jobs[job_count].priority = atoi(items[item_index++]);
        jobs[job_count].execution_time = atoi(items[item_index++]);
        jobs[job_count].remaining_time = jobs[job_count].execution_time;
        jobs[job_count].pid = 0;
        jobs[job_count].is_running = 0;
        jobs[job_count].is_completed = 0;
        job_count++;
    }

    // Free allocated memory
    for (int i = 0; i < item_count; i++) {
        free(items[i]);
    }
}

int main() {
    clear_log_file();
    read_from_data();

    ///// MAIN SCHEDULER LOOP /////
    char log_message[256];
    int current_time = 0;
    int current_job = -1;
    int time_slice_remaining = time_quantum;
    int all_completed = 0;

    while (!all_completed) {
        // Check for newly arrived jobs, first execvp
        for (int i = 0; i < job_count; i++) {
            if (jobs[i].arrival_time == current_time && jobs[i].pid == 0) {
                // Fork a new process for this job
                sprintf(log_message, "[PRC.c] - FORK, %s", jobs[i].name);
                write_log(log_message);
                
                jobs[i].pid = fork();
                
                if (jobs[i].pid == 0) {
                    // Child process
                    char job_name[MAX_ITEM_LENGTH];
                    char arrive_time[10];
                    char priority_level[10];
                    char run_time[10];
                    
                    strcpy(job_name, jobs[i].name);
                    sprintf(arrive_time, "%d", jobs[i].arrival_time);
                    sprintf(priority_level, "%d", jobs[i].priority);
                    sprintf(run_time, "%d", jobs[i].execution_time);
                    
                    sprintf(log_message, "[PRC.c] - EXECUTE, %s (PID: %d) EXECVP", job_name, getpid());
                    write_log(log_message);
                                
                    char *args[] = {"bin/job", job_name, arrive_time, priority_level, run_time, NULL};
                    execvp("bin/job", args);
                    
                    // If execvp returns, there was an error
                    perror("execvp failed");
                    exit(1);
                } else if (jobs[i].pid < 0) {
                    // Fork failed
                    perror("fork failed");
                    exit(1);
                } else {
                    // Parent process
                    // Pause the new process immediately
                    kill(jobs[i].pid, SIGSTOP);
                }
            }
        }  
     

        // If the current job  finished its time slice, or we don't have a current job
        if (time_slice_remaining <= 0 || current_job == -1) {
            
            // Decrease remaining time for the current job
            if (current_job != -1) {
        
                // Check if the job has completed
                if (jobs[current_job].remaining_time <= 0) {
                    sprintf(log_message, "[PRC.c] - COMPLETED, %s (PID: %d) SIGKILL", jobs[current_job].name, jobs[current_job].pid);
                    write_log(log_message);
                    
                    jobs[current_job].is_completed = 1;
                    jobs[current_job].is_running = 0;
                    
                    //usleep(1000); // for last log
                    kill(jobs[current_job].pid, SIGKILL);

                    // Reset the time slice for the next job
                    time_slice_remaining = 0;
                    current_job = -1;
                }
                        
            }
            
            // If there's a current job, pause it
            if (current_job != -1 && jobs[current_job].is_running && !jobs[current_job].is_completed) {
                
                sprintf(log_message, "[PRC.c] - RAN-PAUSED, %s, %d SIGSTOP", jobs[current_job].name, time_quantum);
                write_log(log_message);
                
                kill(jobs[current_job].pid, SIGSTOP);
                jobs[current_job].is_running = 0;
            }
            
            // Find the next job to run
            int next_job = find_next_job(jobs, job_count, current_job, current_time);
            
            // If there's no job to run, increment time and continue
            if (next_job == -1) {
                all_completed = all_jobs_completed(jobs, job_count);
                
                sleep(1);
                current_time++;
                continue;
            }
            
            current_job = next_job;
            time_slice_remaining = time_quantum;
            
            // If the job is not running, start it
            if (!jobs[current_job].is_running) {
                sprintf(log_message, "[PRC.c] - RESUME, %s (PID: %d) SIGCONT", jobs[current_job].name, jobs[current_job].pid);
                write_log(log_message);
                
                kill(jobs[current_job].pid, SIGCONT);
                jobs[current_job].is_running = 1;
            }
        }

        
        
        // Check if all jobs are completed
        all_completed = all_jobs_completed(jobs, job_count);

        jobs[current_job].remaining_time--;
        time_slice_remaining--;
        
        // Sleep for 1 second
        usleep(950000);// //usleep(900000); // 0.1 is system slow process
        current_time++;
        printf("%d\n", current_time);

        // Decrease remaining time for the current job
        if (current_job != -1) {
        
            // Check if the job has completed
            if (jobs[current_job].remaining_time <= 0) {
                sprintf(log_message, "[PRC.c] - COMPLETED, %s (PID: %d) SIGKILL", jobs[current_job].name, jobs[current_job].pid);
                write_log(log_message);
                
                jobs[current_job].is_completed = 1;
                jobs[current_job].is_running = 0;
                
                //usleep(1000); // for last log
                kill(jobs[current_job].pid, SIGKILL);

                // Reset the time slice for the next job
                time_slice_remaining = 0;
                current_job = -1;
            }
                    
        }
    }

    sprintf(log_message, "[PRC.c] - All tasks done");
    write_log(log_message);

    
    return 0;
}