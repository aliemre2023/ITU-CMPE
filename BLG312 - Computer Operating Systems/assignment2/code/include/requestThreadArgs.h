#ifndef REQUEST_THREAD_ARGS_H
#define REQUEST_THREAD_ARGS_H

#include <pthread.h>

typedef struct {
    int customer_id;
    int product_id;
    int quantity;
    int reservation_timeout_ms;
    int* stock_array;
    int num_products;
    int max_concurrent_payments;
    long start_time;
    int in_use; // 1 for active
    int is_retry; // 1 for retry
} RequestThreadArgs;

#endif
