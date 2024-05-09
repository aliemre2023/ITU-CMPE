#include "../include/circuit.h"
#include "../include/pin.h"
#include "../include/and_gate.h"
#include "../include/or_gate.h"
#include "../include/not_gate.h"

#include <iostream>

int main() {
    Circuit circuit;

    // Create input pins as components
    Pin * A = new Pin("A");
    Pin * B = new Pin("B");
    Pin * C = new Pin("C");
    Pin * D = new Pin("D");

    // Set initial signals
    A->setSignal(false);
    B->setSignal(false);
    C->setSignal(false);
    D->setSignal(false);

    // Create gates
    ANDGate * and1 = new ANDGate("AND1");
    ANDGate * and2 = new ANDGate("AND2");
    NOTGate * not1 = new NOTGate("NOT1");
    ORGate  * or1  = new ORGate("OR1");

    // Add components in the proper sequence
    circuit.addComponent(A);
    circuit.addComponent(B);
    circuit.addComponent(C);
    circuit.addComponent(D);
    circuit.addComponent(not1);
    circuit.addComponent(and1);
    circuit.addComponent(and2);
    circuit.addComponent(or1);

    // Create and add wires respecting the order of dependency
    circuit.addWire(new Wire(A, and1->getInputPin(0)));
    circuit.addWire(new Wire(B, and1->getInputPin(1)));
    circuit.addWire(new Wire(C, and2->getInputPin(0)));
    circuit.addWire(new Wire(D, not1->getInputPin(0)));
    circuit.addWire(new Wire(not1->getOutputPin(), and2->getInputPin(1)));
    circuit.addWire(new Wire(and1->getOutputPin(), or1->getInputPin(0)));
    circuit.addWire(new Wire(and2->getOutputPin(), or1->getInputPin(1)));

    // Run the simulation
    // Simulate all possible combinations of inputs
    for (int i = 0; i < 16; ++i) {
        std::cout << "------------------" << i+1 << "------------------" << "\n";
        // Set inputs based on the bits of i
        A->setSignal(i & 1);
        B->setSignal(i & 2);
        C->setSignal(i & 4);
        D->setSignal(i & 8);

        // Simulate the circuit
        circuit.simulate();

        // Print out the result for this combination
        std::cout << "Inputs: A=" << (i & 1) << ", B=" << ((i & 2) >> 1) << ", C=" << ((i & 4) >> 2) << ", D=" << ((i & 8) >> 3)
                  << " | Output: " << or1->getOutputPin()->getSignal() << std::endl;
    }

    return 0;
}
