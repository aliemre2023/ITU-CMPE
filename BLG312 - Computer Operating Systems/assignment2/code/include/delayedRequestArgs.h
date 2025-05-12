#ifndef DELAYED_REQUEST_ARGS_H
#define DELAYED_REQUEST_ARGS_H

#include "requestThreadArgs.h"

typedef struct {
    RequestThreadArgs* args;
    int delay_seconds;
} DelayedRequestArgs;

#endif