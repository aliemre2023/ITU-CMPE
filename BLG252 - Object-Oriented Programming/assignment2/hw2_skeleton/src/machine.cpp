#include "../include/unit.hpp"
#include "../include/machine.hpp"

#include <cstdlib> // std::srand

using namespace std;

Machine::Machine(string name, float price, float cost_per_day, float base_return_per_day, float failure_probability, int repair_time, float repair_cost) 
    : Unit(name, cost_per_day, base_return_per_day) {
    m_price = price;
    m_failure_probability = failure_probability;
    m_repair_time = repair_time;
    m_repair_cost = repair_cost;
    m_days_until_repair = 0;
}

float Machine::getReturnPerDay(){
    if (m_days_until_repair > 0) {
        m_days_until_repair--;
        return 0;
    }
    else {
        double random_number = static_cast<double>(rand()) / RAND_MAX;
        if (random_number < m_failure_probability ) {
            m_days_until_repair = m_repair_time;
            return -m_repair_cost;
        } else {
            return getReturnPerDay();
        }
    }
}

float Machine::getPrice() const{
    return m_price;
}