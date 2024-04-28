#include "../include/unit.hpp"
#include "../include/worker.hpp"

#include <iostream>

using namespace std;


Worker::Worker(string name, float cost_per_day, float base_return_per_day) 
    : Unit(name, cost_per_day, base_return_per_day) {
         m_experience = 0;   
    }

float Worker::getReturnPerDay() {
    float daily_return = Unit::getReturnPerDay() + getExperience() * 2 + this->m_num_head_workers * 3; 
    m_experience++; 
    return daily_return;
}

int Worker::getExperience() const{
    return m_experience;
}

void Worker::increaseHeadWorkerCount() {
    cout << Unit::getName() << " is promoted" << endl;
    m_num_head_workers++;
}

