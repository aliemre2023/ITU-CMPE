#include "or_gate.h"


ORGate::ORGate(const std::string& id) 
    : Gate(id, 2){
    output = new Pin("OR_Output");
}

void ORGate::evaluate() {
    bool result = false;
    
    Pin* input1 = getInputPin(0);
    Pin* input2 = getInputPin(1);
    
    if (input1->getSignal() || input2->getSignal()) {
        result = true;
    }

    output->setSignal(result);
}