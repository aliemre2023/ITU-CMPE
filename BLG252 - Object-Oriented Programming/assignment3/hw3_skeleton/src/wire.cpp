#include "../include/wire.h"


Wire::Wire(Pin* src, Pin* dest){
    source = src;
    destination = dest;
}

void Wire::propagateSignal(){
    bool signal = source->getSignal();
    destination->setSignal(signal);
}