// DO NOT MODIFY THIS FILE
#ifndef FACTORY_HPP
#define FACTORY_HPP

#include "worker.hpp"
#include "headworker.hpp"
#include "machine.hpp"

#include <iostream>
#include <vector>

class Factory
{
private:
    float m_capital;
    bool m_is_bankrupt;

    std::vector<Worker> m_workers;
    std::vector<HeadWorker> m_head_workers;
    std::vector<Machine> m_machines;

public:
    Factory(float capital);

    void passOneDay();
    
    void addUnit(const Worker &rhs_worker);
    void addUnit(const Machine &rhs_machine); 
    
    bool isBankrupt() const;
    float getCapital() const;
    
    int getWorkerCount() const;
    int getMachineCount() const;
    int getHeadWorkerCount() const;
};

#endif // FACTORY_HPP