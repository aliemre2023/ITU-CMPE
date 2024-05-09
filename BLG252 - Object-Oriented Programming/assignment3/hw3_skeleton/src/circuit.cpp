#include "../include/circuit.h"
#include "../include/gate.h"


void Circuit::addComponent(Component* comp){
    components.push_back(comp);
}

void Circuit::addWire(Wire* wire){
    wires.push_back(wire);
}

void Circuit::simulate(){
    for (Wire* wire : wires) {
        wire->propagateSignal();
        
        for (Component* component : components) {
            component->evaluate();
        } 
    }

    for (Component* component : components) {
        component->evaluate();
    }  
};

Circuit::~Circuit(){
    for (Component* comp : components) {
        delete comp;
    }
    for (Wire* wire : wires) {
        delete wire;
    }
}
    
    