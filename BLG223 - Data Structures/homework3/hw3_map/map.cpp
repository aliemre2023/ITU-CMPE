/* @Author
StudentName: Ali Emre Kaya
StudentID: 150210097
Date: 1 December 2023
*/


// run
// g++ -o main main.cpp 
// ./myprogram dataset1.csv operations.csv

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <time.h>

using namespace std;

class Employee {
private:
    int id;
    int salary;
    int department;

public:
    Employee(int id, int salary, int department) : id(id), salary(salary), department(department) {}

    void set_id(int id) {
        this->id = id;
    }
    int get_id() {
        return id;
    }
    void set_salary(int salary) {
        this->salary = salary;
    }
    int get_salary() {
        return salary;
    }
    void set_department(int department) {
        this->department = department;
    }
    int get_department() {
        return department;
    }
};

void addEmployee(map<int, Employee*>& employee_map, Employee* employee) {
    employee_map[employee->get_id()] = employee;
    //employee_map.emplace(employee->get_id(), employee);
}

void updateEmployee(map<int, Employee*>& employee_map, Employee* employee) {
    if(employee_map.find(employee->get_id()) != employee_map.end()){
        employee_map[employee->get_id()]->set_salary(employee->get_salary());
        employee_map[employee->get_id()]->set_department(employee->get_department());
    }
    else{
        cout << "ERROR: An invalid ID to update" << endl;
    }
    
}

void deleteEmployee(map<int, Employee*>& employee_map, int id){
    auto it = employee_map.find(id);
    if(it == employee_map.end()){
        cout << "ERROR: An invalid ID to delete" << "\n";
        return;
    }
    delete it->second;
    employee_map.erase(id);
}

int main(int argc, char* argv[]){
    //clock_t start = clock();

    //console operations
    if(argc != 3){
        return -1;
    }
    string my_data_file, my_operation_file;
    my_data_file = argv[1];
    my_operation_file = argv[2];

    //map solution
    map<int, Employee*> employee_map;

    ifstream file;
    file.open(my_data_file);

    string line;
    string title;
    getline(file, title);

    int max_id_in_map = 0;
    while(getline(file, line)){
        istringstream ss(line);
        string id_str, salary_str, department_str;
        if(getline(ss, id_str, ';') && getline(ss, salary_str, ';') && getline(ss, department_str, ';')){
            int id_int = stoi(id_str);
            int salary_int = stoi(salary_str);
            int department_int = stoi(department_str);

            Employee* employee = new Employee(id_int, salary_int, department_int);
            addEmployee(employee_map, employee);

            if(max_id_in_map < id_int) max_id_in_map = id_int;
        }   
    }

    ifstream operations;
    operations.open(my_operation_file);
    
    string query;
    while(getline(operations, query)){
        istringstream ss(query);
        string o;
        getline(ss, o, ';');
        if(o == "ADD"){
            //cout << "--ADD\n";
            string salary_str, department_str;
            if(getline(ss, salary_str, ';') && getline(ss, department_str, ';')){
                int salary_int = stoi(salary_str);
                int department_int = stoi(department_str);

                max_id_in_map += 1;
                Employee* new_employee = new Employee(max_id_in_map, salary_int, department_int);
                addEmployee(employee_map, new_employee);
            }
        }
        else if(o == "UPDATE"){
            //cout << "--UPD\n";
            string id_str, salary_str, department_str;
            if (getline(ss, id_str, ';') && getline(ss, salary_str, ';') && getline(ss, department_str, ';')) {
                int id_int = stoi(id_str);
                int salary_int = stoi(salary_str);
                int department_int = stoi(department_str);
                
                Employee* updated_employee = new Employee(id_int, salary_int, department_int);
                updateEmployee(employee_map, updated_employee);
            }
        }
        else if(o == "DELETE"){
            //cout << "--DEL\n";
            string id_str;
            if (getline(ss, id_str, ';')) {
                int id_int = stoi(id_str);
                
                deleteEmployee(employee_map, id_int);
            }
        }
        else{
            cout << "Wrong Operation\n";
        }
    }

    // map_solution
    ofstream map_file;
    map_file.open("output.csv");
    map_file << title << "\n";

    for(auto it = employee_map.begin(); it != employee_map.end(); it++){
        map_file << (it->second)->get_id() << ";" << (it->second)->get_salary() << ";" << (it->second)->get_department() << "\n";
        delete it->second;
    } 

    

    file.close();
    operations.close();
    map_file.close();

    //clock_t end = clock();
    //cout << (double)(end-start) * 1000 / CLOCKS_PER_SEC << endl;
    
    return 0;
}