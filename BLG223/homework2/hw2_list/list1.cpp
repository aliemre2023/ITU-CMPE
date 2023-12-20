/* @Author
StudentName: Ali Emre Kaya
StudentID: 150210097
Date: 24 October 2023
*/


// run
// g++ -o myprogram list1.cpp 
// ./myprogram hw2.csv operations.txt  

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio> 
#include <list>
//#include <time.h>

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

void addEmployee(list<Employee*>& employees, int id, int salary, int department){
    Employee* employee = new Employee(id, salary, department);
    employees.push_back(employee);
}

void updateEmployee(list<Employee*>& employees, int id, int salary, int department){
    bool is_valid = false;
    for(auto it = employees.begin(); it != employees.end(); it++){
        if((*it)->get_id() == id){
            is_valid = true;
            (*it)->set_salary(salary);
            (*it)->set_department(department);
        }
    }
    if(!is_valid){
        cout << "ERROR: An invalid ID to update" << "\n";
    }
}

void deleteEmployee(list<Employee*>& employees, int id) {
    if(employees.size() == 0){
        cout << "ERROR: There is no Employee" << "\n";
    }
    bool is_valid = false;
    for(auto it = employees.begin(); it != employees.end(); it++){
        if((*it)->get_id() == id){
            is_valid = true;
            employees.erase(it);
        }
    }
    if(!is_valid){
        cout << "ERROR: An invalid ID to delete" << "\n";
    }
}


int main(int argc, char* argv[]){
    if(argc != 3) return -1;
    string my_data_file, my_operation_file;
    my_data_file = argv[1];
    my_operation_file = argv[2];

    list<Employee*> employees;

    ifstream file;
    file.open(my_data_file);
    string title;
    getline(file, title);

    int max_id = 0;
    string line;
    while(getline(file, line)){
        istringstream ss(line);
        string id_str, salary_str, department_str;
        if (getline(ss, id_str, ';') && getline(ss, salary_str, ';') && getline(ss, department_str, ';')) {
            int id_int = stoi(id_str);
            int salary_int = stoi(salary_str);
            int department_int = stoi(department_str);
            addEmployee(employees, id_int, salary_int, department_int);

            max_id = max(max_id, id_int);
        }
    }

    ifstream operations;
    operations.open(my_operation_file);

    string query;
    while(getline(operations, query)){
        istringstream ss(query);

        string op;
        getline(ss, op, ';');
        if(op == "ADD"){
            int id_will_add = max_id + 1;
            max_id += 1;

            string salary_str, department_str;
            if (getline(ss, salary_str, ';') && getline(ss, department_str, ';')) {
                int salary_int = stoi(salary_str);
                int department_int = stoi(department_str);
                addEmployee(employees, id_will_add, salary_int, department_int);
            }
        }
        else if(op == "UPDATE"){
            string id_str, salary_str, department_str;
            if (getline(ss, id_str, ';') && getline(ss, salary_str, ';') && getline(ss, department_str, ';')) {
                int id_int = stoi(id_str);
                int salary_int = stoi(salary_str);
                int department_int = stoi(department_str);
                updateEmployee(employees, id_int, salary_int, department_int);
            }
        }
        else if(op == "DELETE"){
            string id_str;
            if (getline(ss, id_str, ';')) {
                int id_int = stoi(id_str);
                deleteEmployee(employees, id_int);
            }
        } 
        else{
            continue;
        }
    }

    file.close();
    operations.close();

    // list_solution
    ofstream list_file;
    list_file.open("list_solution.csv");
    list_file << title << "\n";

    for(auto it = employees.begin(); it != employees.end(); it++){
        int id = (*it)->get_id();
        int sal = (*it)->get_salary();
        int dep = (*it)->get_department();
        list_file << id << ";" << sal << ";" << dep << "\n";
        
        
    }

    for (Employee* emp : employees) {
        delete emp;
    }
    employees.clear();
}