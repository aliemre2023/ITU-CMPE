#ifndef ORGATE_H
#define ORGATE_H

#include "gate.h"


class ORGate : public Gate {
public:
    ORGate(const std::string& id);
    void evaluate() override;
};

#endif