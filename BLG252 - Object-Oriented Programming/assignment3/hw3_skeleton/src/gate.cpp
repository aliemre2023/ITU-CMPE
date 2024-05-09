#include "../include/gate.h"


Gate::Gate(const std::string& id, int numberOfInputs) 
    : Component(id) {
    inputs = std::vector<Pin*>(numberOfInputs);
    
    for(int i = 0; i < numberOfInputs; i++){
        inputs[i] = new Pin("Pin");
    }

    output = nullptr;
}

Pin* Gate::getOutputPin(){
    return output;
}

Pin* Gate::getInputPin(int i){
    return inputs[i];
}

Gate::~Gate(){
    for(Pin* input : inputs){
        delete input;
    }
    delete output;
}
