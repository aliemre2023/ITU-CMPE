#include "../include/pin.h"


Pin::Pin(const std::string& id) 
    : Component(id) {
    value = 0;
}

void Pin::setSignal(bool val){
    value = val; 
}

bool Pin::getSignal() const{
    return this->value;
}

void Pin::evaluate(){
    
}