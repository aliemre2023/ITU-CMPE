#include "../include/worker.hpp"
#include "../include/headworker.hpp"
#include "../include/machine.hpp"
#include "../include/factory.hpp"

using namespace std;


Factory::Factory(float capital){
    this->m_capital = capital;
    this->m_is_bankrupt = false;
}

void Factory::passOneDay(){
    float daily_profit = 0;
    for(auto& unit : m_head_workers){
        daily_profit += unit.getReturnPerDay() - unit.getCostPerDay();
    }
    for (auto it = m_workers.begin(); it != m_workers.end();) {
        auto& unit = *it;
        daily_profit += unit.getReturnPerDay() - unit.getCostPerDay();

        if (unit.getExperience() >= 10+1) {
            unit.increaseHeadWorkerCount();
            HeadWorker headWorker(unit);
            
            
            m_head_workers.push_back(headWorker);
            it = m_workers.erase(it);
        } else {
            it++;
        }
    }
    for(auto& unit : m_machines){
        daily_profit += unit.getReturnPerDay() - unit.getCostPerDay();

    }
    m_capital += daily_profit;
}

void Factory::addUnit(const Worker &rhs_worker){
    cout << rhs_worker.getName() << " is hired" << "\n";
    m_workers.push_back(rhs_worker);
}
void Factory::addUnit(const Machine &rhs_machine){
    cout << rhs_machine.getName() << " is bought" << "\n";
    m_machines.push_back(rhs_machine);
}

bool Factory::isBankrupt() const {
    cout << "You went bankrupt!" << "\n";
    return true;
}

float Factory::getCapital() const {
    return m_capital;
}

int Factory::getWorkerCount() const {
    return m_workers.size();
}
int Factory::getMachineCount() const {
    return m_machines.size();
}
int Factory::getHeadWorkerCount() const {
    return m_head_workers.size();
}