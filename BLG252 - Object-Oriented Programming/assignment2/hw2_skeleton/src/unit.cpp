#include "../include/unit.hpp"

using namespace std;


Unit::Unit(string name, float cost_per_day, float base_return_per_day){
    this->m_name = name;
    this->m_cost_per_day = cost_per_day;
    this->m_base_return_per_day = base_return_per_day;
}

string Unit::getName() const{
    return m_name;
}

float Unit::getCostPerDay() const{
    return this->m_cost_per_day;
}

float Unit::getReturnPerDay() const{
    return this->m_base_return_per_day;
}