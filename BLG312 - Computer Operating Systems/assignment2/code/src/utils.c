#ifndef UTILS_H
#define UTILS_H

#include <time.h>
#include <stdio.h>
#include <string.h>

char *log_filePath = "data/info.log";

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

long current_timestamp() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (ts.tv_sec * 1000L) + (ts.tv_nsec / 1000000L);
}

char* get_stock_info(int* stock_array, int num_products) {
    static char stock_info[256];
    stock_info[0] = '\0'; // empty string

    if (stock_array == NULL || num_products <= 0) {
        sprintf(stock_info, "ERROR: Invalid stock data (%d)", num_products);
        return stock_info;
    }
    
    for (int i = 0; i < num_products; i++) {
        char temp[32];
        sprintf(temp, "%s%d:%d", i > 0 ? ", " : "", i, stock_array[i]);
        strcat(stock_info, temp);
    }
    
    return stock_info;
}

#endif