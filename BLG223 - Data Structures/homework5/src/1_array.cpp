/* @Author
StudentName: Ali Emre Kaya
StudentID: 150210097
Date: 24 October 2023
*/


// run
// g++ -o ./src/main ./src/1_array.cpp 
// ./src/main ./dataset/1000.csv ./dataset/DELETE1000.csv


#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio> 
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

void addEmployee(Employee**& employees, int& size, int id, int salary, int department) {
    if (id >= size) {
        int new_size = id + 1;   //too enormous slow, but true;
        //int new_size = (id + 1) * 2;
        // 2. newsize = after n_s + 1 it's already bigger than N_s
        Employee** new_array = new Employee*[new_size];

        for (int i = 0; i < size; i++) {
            new_array[i] = employees[i];
        }

        delete[] employees;
        employees = new_array; // copy
        size = new_size;  
    }

    employees[id] = new Employee(id, salary, department);
}

void updateEmployee(Employee** employees, int size, int id, int salary, int department) {
    if(0 < id && id < size && employees[id] != nullptr){
        employees[id]->set_id(id);
        employees[id]->set_salary(salary);
        employees[id]->set_department(department);
    } 
    else{
        cout << "ERROR: An invalid ID to update" << endl;
    }
}

void deleteEmployee(Employee**& employees, int& size, int id) {
    if(size == 0){
        cout << "ERROR: There is no Employee" << "\n";
    }
    if(id < size){
        if(employees[id] != nullptr){
            delete employees[id];
            employees[id] = nullptr;
        } 
        else{
            cout << "ERROR: An invalid ID to delete" << "\n";
        }
    } 
    else{
        cout << "ERROR: An invalid ID to delete" << "\n";
    }
}

int main(int argc, char* argv[]){
    clock_t start = clock();



    if(argc != 3){
        return -1;
    }
    string my_data_file, my_operation_file;
    my_data_file = argv[1];
    my_operation_file = argv[2];

    int size = 500000; // initial size always will be 500.000
    Employee** employees = new Employee*[size];

    ifstream file;
    //string path_to_csv = "/Users/aliemre2023/Desktop/hw1/array-solution/";
    file.open(my_data_file);
    //file.open("/Users/aliemre2023/Desktop/hw1/array-solution/hw1.csv");

    string line;
    string title;
    getline(file, title); // Employee_ID;Salary;Department

    int da_max_id = 0;
    // only one time read
    while(getline(file, line)){
        istringstream ss(line);
        string id_str, salary_str, department_str;
        if (getline(ss, id_str, ';') && getline(ss, salary_str, ';') && getline(ss, department_str, ';')) {
            int id_int = stoi(id_str);
            int salary_int = stoi(salary_str);
            int department_int = stoi(department_str);
            addEmployee(employees, size, id_int, salary_int, department_int);
            //employees[id] = new Employee(id, salary, department);

            // find da max id
            da_max_id = max(da_max_id, id_int);


            
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
            int id_will_add = da_max_id + 1; // for new_size = size + 1;, true
            da_max_id += 1;

            string salary_str, department_str;
            if (getline(ss, salary_str, ';') && getline(ss, department_str, ';')) {
                int salary_int = stoi(salary_str);
                int department_int = stoi(department_str);
                addEmployee(employees, size, id_will_add, salary_int, department_int);
            }
        } 
        else if(o == "UPDATE"){
            string id_str, salary_str, department_str;
            if (getline(ss, id_str, ';') && getline(ss, salary_str, ';') && getline(ss, department_str, ';')) {
                int id_int = stoi(id_str);
                int salary_int = stoi(salary_str);
                int department_int = stoi(department_str);
                updateEmployee(employees, size, id_int, salary_int, department_int);
            }
        } 
        else if(o == "DELETE"){
            string id_str;
            if (getline(ss, id_str, ';')) {
                int id_int = stoi(id_str);
                deleteEmployee(employees, size, id_int);
            }
        } 
        else{
            continue;
        }
    }

    // visualize
    /*
    cout << size << endl;
    for(int i = 0; i < size; i++){
        if(employees[i]){
            int id = employees[i]->get_id();
            int sal = employees[i]->get_salary();
            int dep = employees[i]->get_department();
            cout << "id:" << id << " || salary:" << sal << " || department:" << dep << endl;
        }
    }
    */

    // array_solution
    ofstream array_file;
    array_file.open("output/output.csv");
    array_file << title << "\n";

    for(int i = 0; i < size; i++){
        if(employees[i]){
            int id = employees[i]->get_id();
            int sal = employees[i]->get_salary();
            int dep = employees[i]->get_department();
            array_file << id << ";" << sal << ";" << dep << "\n";
        }
        
    }
    
    

    // Deallocate memory
    delete[] employees;

    file.close();
    operations.close();

    


    clock_t end = clock();
    cout << (double)(end-start) * 1000 / CLOCKS_PER_SEC << endl;


    return 0;
}
