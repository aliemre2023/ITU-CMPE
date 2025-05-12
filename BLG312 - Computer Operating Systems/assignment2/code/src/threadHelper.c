#ifndef THREAD_HELPER_H
#define THREAD_HELPER_H

#include "requestThreadArgs.h"
#include <pthread.h>
#include <stdlib.h>
#include "utils.c"

#define MAX_THREADS 1000
RequestThreadArgs* active_threads[MAX_THREADS];
pthread_mutex_t threads_mutex = PTHREAD_MUTEX_INITIALIZER;
int active_thread_count = 0;

void print_active_threads() {
    pthread_mutex_lock(&threads_mutex);
    
    printf("=== Active Threads: %d ===\n", active_thread_count);
    for (int i = 0; i < active_thread_count; i++) {
        RequestThreadArgs* thread = active_threads[i];
        printf("Thread %d: Customer ID: %d, Product ID: %d, Quantity: %d, In use: %d, Elapsed time: %ld ms\n",
               i,
               thread->customer_id,
               thread->product_id,
               thread->quantity,
               thread->in_use,
               current_timestamp() - thread->start_time);
    }
    if (active_thread_count == 0) {
        printf("No active threads.\n");
    }
    printf("========================\n");
    
    pthread_mutex_unlock(&threads_mutex);
}

void register_thread(RequestThreadArgs* args) {
    pthread_mutex_lock(&threads_mutex);
    if (active_thread_count < MAX_THREADS) {
        active_threads[active_thread_count++] = args;
    }
    pthread_mutex_unlock(&threads_mutex);
}

void unregister_thread(RequestThreadArgs* args) {
    pthread_mutex_lock(&threads_mutex);
    for (int i = 0; i < active_thread_count; i++) {
        if (active_threads[i] == args) { // shifting
            for (int j = i; j < active_thread_count - 1; j++) {
                active_threads[j] = active_threads[j + 1];
            }
            active_thread_count--;
            break;
        }
    }
    pthread_mutex_unlock(&threads_mutex);
}

int compare_elapsed_time(const void* a, const void* b) {
    RequestThreadArgs* arg1 = *(RequestThreadArgs**)a;
    RequestThreadArgs* arg2 = *(RequestThreadArgs**)b;
    
    long elapsed1 = current_timestamp() - arg1->start_time;
    long elapsed2 = current_timestamp() - arg2->start_time;
    
    return (elapsed2 - elapsed1);
}

int try_after_time(int product_id, int quantity, int* stock) {
    print_active_threads();

    pthread_mutex_lock(&threads_mutex);

    RequestThreadArgs* wanted_products[MAX_THREADS];
    int count = 0;
    for(int i = 0; i < active_thread_count; i++){
        if(active_threads[i]->in_use == 1 && active_threads[i]->product_id == product_id){
            wanted_products[count] = active_threads[i];
            count++;
        }
    }

    qsort(wanted_products, count, sizeof(RequestThreadArgs*), compare_elapsed_time); 

    int available_quantity = stock[product_id];
    for(int i = 0; i < count; i++) {
        printf("AAAA: %d %d\n", available_quantity, wanted_products[i]->quantity);
        available_quantity += wanted_products[i]->quantity;
        printf("BBBB: %d\n", available_quantity);
        
        if(available_quantity >= quantity) {
            printf("CCCC: %ld %d %ld\n", wanted_products[i]->start_time, wanted_products[i]->reservation_timeout_ms, current_timestamp());
            long time_to_wait = (wanted_products[i]->start_time + wanted_products[i]->reservation_timeout_ms) - current_timestamp();
            
            pthread_mutex_unlock(&threads_mutex);
            return (time_to_wait > 0) ? (int)((time_to_wait+1000)/1000) : 0;
        }
    }

    pthread_mutex_unlock(&threads_mutex);
    return -1;
}

void wait_while_active_thread() {
    while(active_thread_count > 0){
        printf("Waiting for %d active threads.\n", active_thread_count);
        sleep(1);
    }
    printf("All threads finished: %d\n", active_thread_count);
} 



#endif