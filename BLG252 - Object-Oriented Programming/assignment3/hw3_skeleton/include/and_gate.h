#ifndef ANDGATE_H
#define ANDGATE_H


#include "gate.h"


class ANDGate : public Gate {
public:
    ANDGate(const std::string& id);
    void evaluate() override;
};

#endif