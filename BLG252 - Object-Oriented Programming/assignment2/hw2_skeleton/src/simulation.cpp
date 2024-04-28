#include "../include/factory.hpp"
#include "../include/simulation.hpp"

#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cmath>

using namespace std;

int day = 0;
int temp_day = day;
int W = 0;
int M = 0;

void Simulation::printWelcomeMessage() const
{
    cout << "Welcome to the Car Factory!" << endl;
    cout << "You have " << m_total_days << " days to make as much money as possible" << endl;
    cout << "You can add workers, machines, and fast forward days" << endl;

    cout << "Available commands:" << endl;
    cout << "    wX: adds X workers" << endl;
    cout << "    mX: adds X machines" << endl;
    cout << "    pX: passes X days" << endl;
    cout << "    q: exit the game properly" << endl;
}

Simulation::Simulation(Factory &factory, int total_days, string workers_list_path, string machines_list_path) {
    m_factory = &factory;
    m_total_days = total_days;

    //workers_list_path = "data/labor_market.txt";
    //machines_list_path = "data/machines_market.txt";

    ifstream workers;
    workers.open(workers_list_path);
    if (!workers.is_open()) {
        cerr << "Failed to open workers list file: " << workers_list_path << endl;
        return; // or handle the error appropriately
    }
    ifstream machines;
    machines.open(machines_list_path);
    if (!machines.is_open()) {
        cerr << "Failed to open machines list file: " << machines_list_path << endl;
        workers.close(); // Close the previously opened workers file
        return; // or handle the error appropriately
    }

    string worker_header;
    getline(workers, worker_header);
    string machine_header;
    getline(machines, machine_header);

    string line;
    while(getline(workers, line)){
        istringstream ss(line);
        string name;
        float daily_cost;
        float daily_return;
        if(ss >> name >> daily_cost >> daily_return){
            //cout << daily_cost << "\n";
            Worker worker = Worker(name, daily_cost, daily_return);
            m_labor_market.push_back(worker);
        }
    }

    string line2;
    while(getline(machines, line2)){
        istringstream ss(line2);
        string name;
        float price;
        float daily_cost;
        float daily_return;
        float fail_prob;
        float repair_cost;
        int repair_time;
        if(ss >> name >> price >> daily_cost >> daily_return >> fail_prob >> repair_cost >> repair_time){
            Machine machine = Machine(name, price, daily_cost, daily_return, fail_prob, repair_cost, repair_time);
            m_machines_market.push_back(machine);
        }
    }
}

void Simulation::run(){
    int day = 0;
    while(day < m_total_days){
        string command;
        cout << "Enter command: ";
        cin >> command;

        if(command.size() == 1){
            break;
        }
        char type = command[0];
        int X = 0;
        int multip = command.size()-2;
        for(int i = 1; i < command.size(); i++){
            int tmp = command[i] - '0';
            X += tmp * pow(10, multip);
            multip -= 1;
        }
        //cout << X << "\n";

        if(type == 'w'){
            W += X;     
            for(int i = 0 ; i < X; i++){
                int random_index = rand() % m_labor_market.size();
                m_factory->addUnit(m_labor_market[random_index]);
                m_labor_market.erase(m_labor_market.begin() + random_index);
            }
        }
        if(type == 'm'){
            M += X;
            for(int i = 0 ; i < X; i++){
                int random_index = rand() % m_machines_market.size();
                m_factory->addUnit(m_machines_market[random_index]);
                m_machines_market.erase(m_machines_market.begin() + random_index);
            }
        }
        if(type == 'p'){
            for(int i = day+1; i < X+day+1; i++){
                m_factory->passOneDay();
                cout << "--- Day: " << i << "\n";
                cout << "[C: " << m_factory->getCapital() << 
                        ", W: " << W - m_factory->getHeadWorkerCount() << 
                        ", M: " << M << 
                        ", HW: " << m_factory->getHeadWorkerCount() <<
                        "]" << "\n";
                temp_day += 1;
                if(m_factory->getCapital() < 0){
                    if(m_factory->isBankrupt()){
                        return;
                    }
                }
                if(temp_day >= m_total_days){
                    break;
                }
            }
            day += X;
        }
        
    }

    cout << "Congrats! You have earned " << m_factory->getCapital() - 100.0f <<  " in " << m_total_days << " days";
            
}
