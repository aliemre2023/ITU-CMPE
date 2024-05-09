#include "../include/and_gate.h"


ANDGate::ANDGate(const std::string& id) 
    : Gate(id, 2) {
    output = new Pin("AND_Output"); 
    }

void ANDGate::evaluate() {
    bool result = false;
    
    Pin* input1 = getInputPin(0);
    Pin* input2 = getInputPin(1);

    if (input1->getSignal() && input2->getSignal()) { 
        result = true;
    }
    
    output->setSignal(result);
}
