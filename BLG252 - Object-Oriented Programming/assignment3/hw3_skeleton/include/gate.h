#ifndef GATE_H
#define GATE_H

#include "component.h"
#include "pin.h"
#include <vector>

// Forward declaration for Pin, used in the composition of Gate.
class Pin;

// Class Gate: Inherits from Component to represent a generic logic gate in a digital circuit.
// This class serves as a base class for specific types of logic gates, handling common attributes and functionalities.
class Gate : public Component {
protected:
    // List of pointers to input pins
    std::vector<Pin*> inputs;
    // Pointer to the output pin
    Pin* output;
public:
    // Constructor: Initializes a gate with a unique identifier and a specified number of input pins.
    // id: A string that uniquely identifies the gate.
    // numberOfInputs: The number of input pins the gate will have.
    Gate(const std::string& id, int numberOfInputs);

    // Virtual destructor: Ensures proper cleanup of input and output pins when a gate object is destroyed.
    virtual ~Gate();

    // getOutputPin: Returns a pointer to the output pin of the gate.
    Pin* getOutputPin();

    // getInputPin: Returns a pointer to a specific input pin of the gate.
    Pin* getInputPin(int i);
};

#endif
