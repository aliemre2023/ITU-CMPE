#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

#define MAX_ENTRIES 100
#define MAX_LINE_LENGTH 256

void run_command(const char *cmd) {
    FILE *fp = popen(cmd, "r");
    if (fp == NULL) {
        printf("Failed to run command: %s\n", cmd);
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }

    pclose(fp);
}

int main(int argc, char *argv[]) {
    clear_log_file();
    
    if (argc != 3) {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    FILE* input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        printf("Error: Could not open input file %s\n", argv[1]);
        return 1;
    }
    FILE* output_file = fopen(argv[2], "w");
    if (output_file == NULL) {
        printf("Error: Could not open output file %s\n", argv[2]);
        fclose(input_file);
        return 1;
    }

    FILE* old_stdout = stdout;
    stdout = output_file;
    
    char line[MAX_LINE_LENGTH];
    char cmd_buffer[MAX_LINE_LENGTH + 20];

    // line by line
    while (fgets(line, MAX_LINE_LENGTH, input_file) != NULL) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        if (line[0] == '\0') {
            continue;
        }
        
        //printf("Executing: %s\n", line);
        
        snprintf(cmd_buffer, sizeof(cmd_buffer), "bin/main %s", line);
        run_command(cmd_buffer);
        
        //printf("\n");
    }
    
    stdout = old_stdout;
    fclose(input_file);
    fclose(output_file);
    
    //printf("Commands executed from %s, output written to %s\n", argv[1], argv[2]);
    return 0;
}