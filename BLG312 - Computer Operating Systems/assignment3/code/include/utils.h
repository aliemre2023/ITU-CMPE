#ifndef UTILS_H
#define UTILS_H

// Function declarations
void get_timestamp();
void write_log(const char* message);
void clear_log_file();

extern const char* log_filePath;

#endif // UTILS_H