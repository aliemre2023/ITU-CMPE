#ifndef JOB_H
#define JOB_H

#include <sys/types.h> 

#define MAX_ITEMS 10000
#define MAX_ITEM_LENGTH 30
#define MAX_JOBS 100

typedef struct {
    char name[MAX_ITEM_LENGTH];
    int arrival_time;
    int priority;
    int execution_time;
    int remaining_time;
    pid_t pid;
    int is_running;
    int is_completed;
} Job;

#endif