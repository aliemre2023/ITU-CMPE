#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

int find_equalitySign(char string[]){
    int i = 0;
    while(string[i] != '='){
        i++;
    }

    return i;
}

int seperate_takeAfterEqual(char string[]){
    int i = find_equalitySign(string);
    i++;

    int result = 0;
    while(is_digit(string[i])){
        result *= 10;
        result +=  (string[i] - '0');
        i++;
    }

    return result;
}

int* seperateArray_takeAfterEqual(char string[], int count){
    int i = find_equalitySign(string);
    i++;

    int* result = (int*)malloc((count) * sizeof(int));

    int current = 0;
    int temp_num = 0;
    while(string[i] != '\0') {
        if(is_digit(string[i])) {
            temp_num = temp_num * 10 + (string[i] - '0');
        }
        else if(string[i] == ',') {
            result[current++] = temp_num;
            temp_num = 0;
        }
        i++;
    }
    
    if(current < count) {
        result[current] = temp_num;
    }
    
    return result;
}

int* seperateRequest(char string[]){
    int* result = (int*)malloc((3) * sizeof(int));

    int current = 0;
    int i = 0;
    int temp_num = 0;
    while(string[i] != '\0') {
        if(is_digit(string[i])){
            temp_num = temp_num * 10 + (string[i] - '0');
        }
        else if(string[i] == ','){
            result[current++] = temp_num;
            temp_num = 0;
        }
        i++;
    }
    if(current < 3) {
        result[current] = temp_num;
    }

    return result;
}

int is_empty_line(char *line) {
    while (*line) {
        if (*line != ' ' && *line != '\t' && *line != '\n' && *line != '\r') {
            return 0;
        }
        line++;
    }
    return 1;
}