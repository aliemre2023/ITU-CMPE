#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h> // sleep
#include <stdlib.h>
#include "requestThreadArgs.h"
#include "delayedRequestArgs.h"
#include "readHelper.c"
#include "threadHelper.c"
#include "utils.c"

#define MAX_THREADS 1000
RequestThreadArgs* active_threads[MAX_THREADS];

pthread_mutex_t stock_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t payment_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t payment_cond = PTHREAD_COND_INITIALIZER;
int current_payments = 0;

void* handle_delayed_request(void* arg);
void* handle_request(void* arg);

void* handle_request(void* arg) {
    int is_unregistered = 0;
    pthread_t tid = pthread_self();

    RequestThreadArgs* args = (RequestThreadArgs*) arg;
    int customer_id = args->customer_id;
    int product_id = args->product_id;
    int quantity = args->quantity;
    int timeout_ms = args->reservation_timeout_ms;
    int max_concurrent_payments = args->max_concurrent_payments;
    int* stock_array = args->stock_array;

    char log_message[256];
    long start_time = current_timestamp();

    pthread_mutex_lock(&stock_mutex);

    if(stock_array[product_id] >= quantity) { // enough stock
        stock_array[product_id] -= quantity;
        char* stock_info = get_stock_info(stock_array, args->num_products);
        args->in_use = 1;

        pthread_mutex_unlock(&stock_mutex);

        snprintf(log_message, sizeof(log_message), "[%lu] [%s] Customer %d reserved product %d (qty: %d)", (unsigned long)tid, stock_info, customer_id, product_id, quantity);
        write_log(log_message);


        // better randomness
        unsigned int seed = (unsigned int)time(NULL) ^ (unsigned int)tid;
        seed ^= (unsigned)tid;
        seed ^= (unsigned)customer_id << 8;
        seed ^= (unsigned)product_id << 16;
        seed ^= (unsigned)quantity << 32;
        srand(seed);
        int will_purchase = rand() % 2; // for some reason always generate same log
        if(will_purchase) {
            int purchased = 0;

            while(current_timestamp() - start_time < timeout_ms) {
                pthread_mutex_lock(&payment_mutex);

                if(current_payments < max_concurrent_payments) {
                    current_payments++;

                    pthread_mutex_unlock(&payment_mutex);
                    
                    is_unregistered = 1;
                    unregister_thread(args);
                    snprintf(log_message, sizeof(log_message), "[%lu] Customer %d attempts to purchase product %d (qty: %d) // succesful", (unsigned long)tid, customer_id, product_id, quantity);
                    write_log(log_message);

                    sleep(5);

                    pthread_mutex_lock(&payment_mutex);
                    current_payments--;
                    pthread_cond_signal(&payment_cond);
                    pthread_mutex_unlock(&payment_mutex);

                    //snprintf(log_message, sizeof(log_message), "[%lu] Customer %d purchased product %d (qty: %d) // succesful", (unsigned long)tid, customer_id, product_id, quantity);
                    //write_log(log_message);

                    purchased = 1;
                    break;
                }
                else {
                    snprintf(log_message, sizeof(log_message), "[%lu] Customer %d attempts to purchase product %d (qty: %d) // wait for payments", (unsigned long)tid, customer_id, product_id, quantity);
                    write_log(log_message);

                    pthread_cond_wait(&payment_cond, &payment_mutex);
                    pthread_mutex_unlock(&payment_mutex);
                }

                pthread_mutex_unlock(&payment_mutex);
                sleep(2);

            }

            if(!purchased) {
                pthread_mutex_lock(&stock_mutex);
                stock_array[product_id] += quantity;
                char* stock_info = get_stock_info(stock_array, args->num_products);
                pthread_mutex_unlock(&stock_mutex);

                snprintf(log_message, sizeof(log_message), "[%lu] [%s] Customer %d timed out before purchase. Product %d (qty: %d) returned to stock.", (unsigned long)tid, stock_info, customer_id, product_id, quantity);
                write_log(log_message);
            }

        }
        else {
            sleep(timeout_ms/1000);

            pthread_mutex_lock(&stock_mutex);
            stock_array[product_id] += quantity;
            char* stock_info = get_stock_info(stock_array, args->num_products);
            pthread_mutex_unlock(&stock_mutex);

            snprintf(log_message, sizeof(log_message), "[%lu] [%s] Customer %d will not buy. Product %d (qty: %d) returned to stock.", (unsigned long)tid, stock_info, customer_id, product_id, quantity);
            write_log(log_message);
        }



    }
    else { // not enough stock
        char* stock_info = get_stock_info(stock_array, args->num_products);
        pthread_mutex_unlock(&stock_mutex);

        pthread_mutex_lock(&stock_mutex);
        int try_after = try_after_time(product_id, quantity, stock_array);
        pthread_mutex_unlock(&stock_mutex);

        if(try_after != -1) {
            if(args->is_retry == 1) {
                snprintf(log_message, sizeof(log_message), "[%lu] [%s] Customer %d could NOT reserve product %d (qty: %d). Retry failed. Try %d second later.", (unsigned long)tid, stock_info, customer_id, product_id, quantity, try_after);
            }
            else {
                snprintf(log_message, sizeof(log_message), "[%lu] [%s] Customer %d could NOT reserve product %d (qty: %d). No stock. Try %d second later.", (unsigned long)tid, stock_info, customer_id, product_id, quantity, try_after);
            }
            
            write_log(log_message);
            

            RequestThreadArgs* delayed_args = malloc(sizeof(RequestThreadArgs));
            delayed_args->customer_id = args->customer_id;
            delayed_args->product_id = args->product_id;
            delayed_args->quantity = args->quantity;
            delayed_args->reservation_timeout_ms = args->reservation_timeout_ms;
            delayed_args->num_products = args->num_products;
            delayed_args->max_concurrent_payments = args->max_concurrent_payments;
            delayed_args->in_use = 0;
            delayed_args->stock_array = args->stock_array; 
            delayed_args->start_time = current_timestamp() + (try_after * 1000);
            delayed_args->is_retry = 1;

            DelayedRequestArgs* delayed = malloc(sizeof(DelayedRequestArgs));
            delayed->args = delayed_args;
            delayed->delay_seconds = try_after;


            pthread_t delayed_tid;
            if(pthread_create(&delayed_tid, NULL, handle_delayed_request, (void*)delayed) != 0) {
                free(delayed_args);
                free(delayed);
                printf("ERROR: Failed to create delayed request thread\n");
            } 
            else {
                pthread_detach(delayed_tid);
            }


        }
        else {
            if(args->is_retry == 1) {
                snprintf(log_message, sizeof(log_message), "[%lu] [%s] Customer %d could NOT reserve product %d (qty: %d). Retry failed. No stock left. ", (unsigned long)tid, stock_info, customer_id, product_id, quantity);
            }
            else {
                snprintf(log_message, sizeof(log_message), "[%lu] [%s] Customer %d could NOT reserve product %d (qty: %d). No stock left.", (unsigned long)tid, stock_info, customer_id, product_id, quantity);
            }
            write_log(log_message);
        }
        // write_log(log_message);

    }


    printf("product_id: %d,,,\n",  args->product_id);
    if(is_unregistered == 0) unregister_thread(args);
    free(arg);
    pthread_exit(NULL);

}

void* handle_delayed_request(void* arg) {
    typedef struct {
        RequestThreadArgs* args;
        int delay_seconds;
    } DelayedRequestArgs;
    
    DelayedRequestArgs* delayed = (DelayedRequestArgs*)arg;
    int delay = delayed->delay_seconds;
    
    sleep(delay);
    
    register_thread(delayed->args);
    handle_request(delayed->args);
    
    free(delayed);
    return NULL;
}

int main(){
    clear_log_file();

    FILE *fptr;
    fptr = fopen("data/input.txt", "r");
    if (fptr == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    char line[100];


    int num_customers = 0;
    int num_products = 0;
    int reservation_timeout_ms = 0;
    int max_concurrent_payments = 0;
    int* initial_stock  = NULL;

    int** group = NULL; 
    int group_size = 0;   
    int group_capacity = 100; 
    group = (int**)malloc(group_capacity * sizeof(int*));
    
    int i = 0;
    while(fgets(line, 100, fptr)) {
        if(i < 5){
            if(i == 0) num_customers = seperate_takeAfterEqual(line);
            else if(i == 1) num_products = seperate_takeAfterEqual(line);
            else if(i == 2) reservation_timeout_ms = seperate_takeAfterEqual(line);
            else if(i == 3) max_concurrent_payments = seperate_takeAfterEqual(line);
            else initial_stock = seperateArray_takeAfterEqual(line, num_products);

            i++;
        }
        else if (i == 5){
            char log_message[200];
            strcpy(log_message, "Initial stock: [");
            for(int j = 0; j < num_products; j++){
                char temp[30];
                sprintf(temp, "product %d: %d", j, initial_stock[j]);
                
                strcat(log_message, temp);
                
                if(j < num_products - 1) {
                    strcat(log_message, ", ");
                }
            }
            strcat(log_message, "]");

            write_log(log_message);
            i++;
        }
        else{
            // Wait for buying,
            if(is_empty_line(line)) {
                // a group should start to add to the cart in same time, and start to buy 
                //pthread_t* tids = malloc(group_size * sizeof(pthread_t));

                for (int j = 0; j < group_size; j++) {
                    RequestThreadArgs* args = malloc(sizeof(RequestThreadArgs));
                    args->customer_id = group[j][0];
                    args->product_id = group[j][1];
                    args->quantity = group[j][2];
                    args->reservation_timeout_ms = reservation_timeout_ms;
                    args->stock_array = initial_stock;
                    args->num_products = num_products;
                    args->max_concurrent_payments = max_concurrent_payments;
                    args->in_use = 0;
                    args->start_time = current_timestamp();
                    args->is_retry = 0;
                    printf("c_id:%d - p_id:%d - q:%d - Num: %d STARTTIME: %ld \n", args->customer_id, args->product_id, args->quantity, args->num_products, args->start_time);
                    
                    register_thread(args);
                    pthread_t tid;
                    pthread_create(&tid, NULL, handle_request, (void*)args);
                    pthread_detach(tid);
                }

                /*
                for (int j = 0; j < group_size; j++) {
                    pthread_join(tids[j], NULL);  // wait for them to finish
                }
                */

                sleep(5);

                for (int j = 0; j < group_size; j++) {
                    free(group[j]);
                }
                free(group);
                
                // reallocate
                group_size = 0;
                group = (int**)malloc(group_capacity * sizeof(int*));
            }
            // Take from cart,
            else {
                int* temp_array = seperateRequest(line); // customer, product, count

                printf("Customer: %d, Product: %d, Quantity: %d\n", temp_array[0], temp_array[1], temp_array[2]);


                group[group_size++] = temp_array;
            }
    


            i++;
        }
        printf("%s", line);
    }

    if(group_size > 0) {
        for (int j = 0; j < group_size; j++) {
            RequestThreadArgs* args = malloc(sizeof(RequestThreadArgs));
            args->customer_id = group[j][0];
            args->quantity = group[j][1];
            args->product_id = group[j][2];
            args->reservation_timeout_ms = reservation_timeout_ms;
            args->stock_array = initial_stock;
            args->num_products = num_products;
            args->max_concurrent_payments = max_concurrent_payments;
            args->in_use = 0;
            args->start_time = current_timestamp();
            args->is_retry = 0;
            printf("c_id:%d - p_id:%d - q:%d - Num: %d - STARTTIME: %ld\n", args->customer_id, args->product_id, args->quantity, args->num_products, args->start_time);

            register_thread(args);
            pthread_t tid;
            pthread_create(&tid, NULL, handle_request, (void*)args);
            pthread_detach(tid);
        }

        //sleep(50);

    }
    wait_while_active_thread();

    for (int j = 0; j < group_size; j++) {
        free(group[j]);
    }
    free(group);

    fclose(fptr);
    free(initial_stock);
}