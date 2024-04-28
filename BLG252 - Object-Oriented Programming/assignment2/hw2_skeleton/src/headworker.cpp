#include "../include/unit.hpp"
#include "../include/worker.hpp"
#include "../include/headworker.hpp"

using namespace std;


HeadWorker::HeadWorker(Worker &worker) 
    : Worker(worker.getName(), worker.getCostPerDay(), worker.getReturnPerDay()) {}

float HeadWorker::getReturnPerDay(){
    float daily_return = Worker::getReturnPerDay() + m_experience * 5;
    return daily_return;
}