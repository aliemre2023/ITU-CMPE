#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <vector>
#include "component.h"
#include "wire.h"

// Class Circuit: Represents a digital circuit composed of various components and wires.
class Circuit {
private:
    // Collection of pointers to components within the circuit.
    std::vector<Component*> components;
    // Collection of pointers to wires connecting components within the circuit.
    std::vector<Wire*> wires;

public:
    // Destructor: Cleans up all components and wires in the circuit to prevent memory leaks.
    ~Circuit();

    // addComponent: Adds a new component to the circuit.
    void addComponent(Component* comp);

    // addWire: Adds a new wire to the circuit that connects components.
    void addWire(Wire* wire);


    // simulate: Initiates the simulation of the circuit.
    // This function runs the logical operations of the circuit by calling evaluate on each component
    // in an order that respects the logical dependencies and connection via wires.
    // Hint/Attention: Think simple and before each component evaluation iterate over all wires and propagate signal
    void simulate();
};

#endif