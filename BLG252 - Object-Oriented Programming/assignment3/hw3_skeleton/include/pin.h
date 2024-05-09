#ifndef PIN_H
#define PIN_H

#include "component.h"

// Class Pin: Inherits from Component to represent an individual pin in a digital logic circuit.
// Pins are basic elements that hold a signal value, typically used as input or output points in components.
class Pin : public Component {
private:
    // Holds the current signal value (true or false) of the pin.
    bool value; 

public:
    // Constructor: Initializes the pin with a unique identifier.
    // A string that uniquely identifies the pin within the circuit.
    Pin(const std::string& id);

    // setSignal: Sets the pin's signal to the specified value.
    void setSignal(bool val);
    
    // getSignal: Returns the current signal value of the pin.
    bool getSignal() const;
    
    // evaluate: Implementation for the abstract evaluate method from Component.
    // For Pin, this method is a placeholder as the pin itself does not perform any evaluation logic.
    void evaluate() override;
};

#endif
