/* @Author
StudentName: Ali Emre Kaya
StudentID: 150210097
Date: 29 October 2023
*/

// run
// g++ -o myprogram linkedlist1.cpp 
// ./myprogram hw2.csv operations.txt 

#include <iostream>
#include <fstream>
#include <sstream>
//#include <time.h>
using namespace std;

class Employee{
private:
    int id;
    int salary;
    int department;

public:
    Employee(int id, int salary, int department) : id(id), salary(salary), department(department) {}

    int get_id(){
        return id;
    }
    void set_salary(int new_salary){
        salary = new_salary;
    }
    int get_salary(){
        return salary;
    }
    void set_department(int new_department){
        department = new_department;
    }
    int get_department(){
        return department;
    }
};

// creating node which takes employee variables
class Node {
    public:
        Employee* employee;
        Node* next;

        //constructor, initalizer
        //that node will represent Employee,
        Node(Employee* emp) : employee(emp), next(nullptr) {}
};

/*
I add new data to head of the linkedlist,
so there is a reversed version of the employees,
I will reverse it to take the ascending order version,
and write it into the new csv file.
*/
Node* reverse_linkedlist(Node* head){
    Node* bef_temp = nullptr;

    while(head->next != nullptr){
        Node* head_temp = head;
        head = head->next;
        head_temp->next = bef_temp;
        bef_temp = head_temp;
    }
    head->next = bef_temp;
    return head;
}

void addEmployee(Node* &employees_head, int id, int salary, int department){
    // creating new employee
    Employee* new_employee = new Employee(id, salary, department);
    // assign the employee to the node
    Node* new_node = new Node(new_employee);
    // append it head of the linked list, and copy it important node ptr (employees_head)
    new_node->next = employees_head;
    employees_head = new_node; 
}

void updateEmployee(Node* employees_head, int id, int salary, int department){
    if(employees_head == nullptr){
        cout << "ERROR: There is no Employee" << "\n";
    }
    while(employees_head != nullptr){
        if((employees_head->employee)->get_id() == id){
            (employees_head->employee)->set_salary(salary);
            (employees_head->employee)->set_department(department);
            return; 
        }
        employees_head = employees_head->next;
    }
    cout << "ERROR: An invalid ID to update" << endl;
}

void deleteEmployee(Node* &employees_head, int id){
    Node* prev_node = nullptr;
    Node* curr_node = employees_head;

    while(curr_node != nullptr){
        if(curr_node->employee->get_id() == id){
            // if the id which is wanted to delete is first node
            if(prev_node == nullptr){
                employees_head = curr_node->next;
                // deleting employee before deleting the node;
                delete curr_node->employee;
                delete curr_node;
            } 
            // else
            else{
                prev_node->next = curr_node->next;
                delete curr_node->employee;
                delete curr_node;
            }
            return;
        }
        prev_node = curr_node;
        curr_node = curr_node->next;
    }
    cout << "ERROR: An invalid ID to delete" << endl;
}

int main(int argc, char* argv[]){
    //clock_t start = clock();

    if(argc != 3) return -1;

    string my_data_file = argv[1];
    string my_operation_file = argv[2];

    // will used only for add;
    // that will always stay in the head, because add operator always turn back to the head,
    // so I will able to use it for saving values into the new csv
    Node* employees = nullptr;

    ifstream file;
    file.open(my_data_file);

    string title;
    getline(file, title);

    int max_id = 0;
    string line;
    while(getline(file, line)){
        istringstream ss(line);
        string id_str, salary_str, department_str;
        if(getline(ss, id_str, ';') && getline(ss, salary_str, ';') && getline(ss, department_str, ';')){
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
            if(getline(ss, salary_str, ';') && getline(ss, department_str, ';')){
                int salary_int = stoi(salary_str);
                int department_int = stoi(department_str);
                addEmployee(employees, id_will_add, salary_int, department_int);
            }
        } 
        else if(op == "UPDATE"){
            // I can't able to use employee(the main head) in there,
            //because UPDATE and DELETE functions should visit all the node
            //(ADD is only use the head), that will lead to lose the head,
            //I will escape it with using psuedo head
            Node* employees_head = employees; 
            string id_str, salary_str, department_str;
            if(getline(ss, id_str, ';') && getline(ss, salary_str, ';') && getline(ss, department_str, ';')){
                int id_int = stoi(id_str);
                int salary_int = stoi(salary_str);
                int department_int = stoi(department_str);
                updateEmployee(employees_head, id_int, salary_int, department_int);
            }
        }
        else if(op == "DELETE"){
            Node* employees_head = employees; 
            string id_str;
            if(getline(ss, id_str, ';')){
                int id_int = stoi(id_str);
                deleteEmployee(employees_head, id_int);
            }
        }    
    }

    // linked_list_solution
    ofstream linked_list_file;
    linked_list_file.open("linked_list_solution.csv");
    linked_list_file << title << "\n";

    // employees still represent the head
    
    Node* writer = reverse_linkedlist(employees);
    while(writer != nullptr){
        int id = (writer->employee)->get_id();
        int sal = (writer->employee)->get_salary();
        int dep = (writer->employee)->get_department();
        linked_list_file << id << ";" << sal << ";" << dep << "\n";
    
        writer = writer->next;
    }

    // now I can delete all the linked-list with using employees
    while(employees != nullptr){
        // temp for don't lose the head;
        Node* temp = employees;
        employees = employees->next;
        delete temp->employee;
        delete temp;
    }

    //clock_t end = clock();
    //cout << (double)(end - start) * 1000 / CLOCKS_PER_SEC << "\n";
    return 0;
}
