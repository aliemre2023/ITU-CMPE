#include <time.h>
#include <stdio.h>
#include "utils.h"

const char* log_filePath = "tests/run_log.txt";

void get_timestamp(char *timestamp, int size) {
    time_t now;
    struct tm *timeinfo;
    
    time(&now);
    timeinfo = localtime(&now);
    
    strftime(timestamp, size, "[%Y-%m-%d %H:%M:%S]", timeinfo);
}

void write_log(const char *message) {
    FILE *log_file = fopen(log_filePath, "a");
    
    if (log_file == NULL) {
        printf("path: %s\n", log_filePath);
        perror("Error opening log file");
        return;
    }
    
    char timestamp[26];
    get_timestamp(timestamp, sizeof(timestamp));
    
    fprintf(log_file, "%s [INFO] %s\n", timestamp, message);
    fclose(log_file);
}

void clear_log_file(){
    FILE *log_file = fopen(log_filePath, "w");
    if(log_file != NULL){
        fclose(log_file);
        return;
    }
}