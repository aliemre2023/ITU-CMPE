#ifndef WIRE_H
#define WIRE_H

#include "pin.h"

// Class Wire: Represents a connection between two pins in a digital circuit.
// It essentially acts as a conduit for signals, transferring state from a source pin to a destination pin.
class Wire {
private:
    // Pointer to the source pin from which the signal originates.
    Pin* source;
    // Pointer to the destination pin to which the signal is sent.
    Pin* destination;

public:
    // Constructor: Initializes a wire with a source and a destination pin.
    Wire(Pin* src, Pin* dest);
    
    // propagateSignal: Transfers the signal from the source pin to the destination pin.
    // This function effectively updates the state of the destination pin to match that of the source pin.
    void propagateSignal();
};

#endif