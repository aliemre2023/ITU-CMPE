#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fs.h"
#include "utils.h"

#define MAX_ENTRIES 100


int main(int argc, char *argv[]){
    char log_message[256];

    if(argc < 2){
        snprintf(log_message, sizeof(log_message), "Usage: %s <command> [arguments]", argv[0]);
        write_log(log_message);
        fprintf(stderr, "%s\n", log_message);
        return 1;
    }

    const char *cmd = argv[1];

    if(strcmp(cmd, "mkfs") == 0){
        mkfs("disk.img");
    }
    else if(strcmp(cmd, "mkdir_fs") == 0){      
        if(mkdir_fs(argv[2]) == 0){
            snprintf(log_message, sizeof(log_message), "Successfully created directory: %s", argv[2]);
            write_log(log_message);
        } else {
            snprintf(log_message, sizeof(log_message), "Failed to create directory: %s", argv[2]);
            write_log(log_message);
        }
    }
    else if(strcmp(cmd, "create_fs") == 0){      
        if(create_fs(argv[2]) == 0){
            snprintf(log_message, sizeof(log_message), "Successfully created file: %s", argv[2]);
            write_log(log_message);
        } else {
            snprintf(log_message, sizeof(log_message), "Failed to create file: %s", argv[2]);
            write_log(log_message);
        }
    }
    else if(strcmp(cmd, "ls_fs") == 0){
        
        DirectoryEntry *entries = malloc(sizeof(DirectoryEntry) * MAX_ENTRIES);
        if(!entries){
            snprintf(log_message, sizeof(log_message), "Error: Memory allocation failed");
            write_log(log_message);
            fprintf(stderr, "%s\n", log_message);
            return 1;
        }
        
        int count = ls_fs(argv[2], entries, MAX_ENTRIES);
        
        if(count >= 0){
            snprintf(log_message, sizeof(log_message), "Listed directory %s: found %d entries", argv[2], count);
            write_log(log_message);
        }
        else{
            snprintf(log_message, sizeof(log_message), "Error: Failed to list directory %s", argv[2]);
            write_log(log_message);
            fprintf(stderr, "%s\n", log_message);
        }
        
        free(entries);
    }
    else if(strcmp(cmd, "write_fs") == 0){
        
        int bytes_written = write_fs(argv[2], argv[3]);
        
        if(bytes_written >= 0){
            snprintf(log_message, sizeof(log_message), "Successfully wrote %d bytes to %s", bytes_written, argv[2]);
            write_log(log_message);
        } else {
            snprintf(log_message, sizeof(log_message), "Error: Failed to write to file %s", argv[2]);
            write_log(log_message);
            fprintf(stderr, "%s\n", log_message);
            return 1;
        }
    }
    else if(strcmp(cmd, "read_fs") == 0){
        
        char buffer[4096] = {0};
        
        int bytes_read = read_fs(argv[2], buffer, sizeof(buffer)-1);
        
        if(bytes_read >= 0){
            snprintf(log_message, sizeof(log_message), "Successfully read %d bytes from %s", bytes_read, argv[2]);
            write_log(log_message);
            // Print the file contents
            printf("%s\n", buffer);
        }
        else{
            snprintf(log_message, sizeof(log_message), "Error: Failed to read file %s", argv[2]);
            write_log(log_message);
            fprintf(stderr, "%s\n", log_message);
            return 1;
        }
    }
    else if(strcmp(cmd, "delete_fs") == 0){
        int result = delete_fs(argv[2]);
        
        if(result == 0){
            snprintf(log_message, sizeof(log_message), "Successfully deleted file: %s", argv[2]);
            write_log(log_message);
        } else {
            snprintf(log_message, sizeof(log_message), "Failed to delete file: %s", argv[2]);
            write_log(log_message);
            fprintf(stderr, "%s\n", log_message);
            return 1;
        }
    }
    else if(strcmp(cmd, "rmdir_fs") == 0){
        int result = rmdir_fs(argv[2]);
        
        if(result == 0){
            snprintf(log_message, sizeof(log_message), "Successfully removed directory: %s", argv[2]);
            write_log(log_message);
        } else {
            snprintf(log_message, sizeof(log_message), "Failed to remove directory: %s", argv[2]);
            write_log(log_message);
            fprintf(stderr, "%s\n", log_message);
            return 1;
        }
    }
    else{
        snprintf(log_message, sizeof(log_message), "Error: Unknown command '%s'", cmd);
        write_log(log_message);
        fprintf(stderr, "%s\n", log_message);
        fprintf(stderr, "Available commands: mkfs, mkdir_fs, create_fs, write_fs, read_fs, ls_fs, rmdir_fs, delete_fs\n");
        return 1;
    }

    return 0;
}