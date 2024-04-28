// DO NOT MODIFY THIS FILE
#ifndef WORKER_HPP
#define WORKER_HPP

#include "unit.hpp"

class Worker : public Unit
{
private:
    static inline int m_num_head_workers = 0;
    //this will be 
    //static inline int m_num_head_workers {}
    

protected:
    //void increaseHeadWorkerCount();
    int m_experience;

public:
    // this will be protected
    void increaseHeadWorkerCount(); 

    Worker(std::string name, float cost_per_day, float base_return_per_day);
    
    float getReturnPerDay();
    int getExperience() const;
};

#endif // WORKER_HPP