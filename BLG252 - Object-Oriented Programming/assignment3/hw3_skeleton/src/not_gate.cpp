#include "../include/not_gate.h"


NOTGate::NOTGate(const std::string& id) 
    : Gate(id, 1) {
    output = new Pin("NOT_Output"); 
}

void NOTGate::evaluate(){
    Pin* input = getInputPin(0);
    output->setSignal(!input->getSignal());
}