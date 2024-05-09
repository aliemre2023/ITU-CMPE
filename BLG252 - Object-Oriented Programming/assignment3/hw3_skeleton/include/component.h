#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

// Class Component: Abstract base class for all circuit components in a digital logic simulator.
// It provides a common interface and essential properties that all specific components (like gates, wires, etc.) must implement.
class Component {
protected:
    std::string id;

public:
    // Constructor: Initializes the component with a unique identifier.
    // A string that uniquely identifies the component.
    Component(const std::string& id);

    // Pure virtual function evaluate: Forces derived classes to implement their specific evaluation logic,
    // which defines how they process input and produce output in the simulation.
    virtual void evaluate() = 0;

    // Virtual destructor: Ensures that derived class objects are destructed properly, allowing for proper resource management.
    virtual ~Component();
};

#endif
