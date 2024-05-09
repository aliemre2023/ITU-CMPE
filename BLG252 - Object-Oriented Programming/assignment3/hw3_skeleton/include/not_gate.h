#ifndef NOTGATE_H
#define NOTGATE_H

#include "gate.h"

class NOTGate : public Gate {
public:
    NOTGate(const std::string& id);
    void evaluate() override;
};

#endif