/* @Author
StudentName: Ali Emre Kaya
StudentID: 150210097
Date: 30 December 2023
*/

// run
// g++ -o myprogram main.cpp 
// ./myprogram 10k_employees.csv operations_for_10k_1.csv 

// delete operations may cause segmentation error?

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <time.h>
using namespace std;

class Employee {
private:
    int id;
    int salary;
    int department;

public:
    Employee(int id, int salary, int department) : id(id), salary(salary), department(department) {}

    int get_id() {
        return id;
    }
    void set_salary(int new_salary) {
        salary = new_salary;
    }
    int get_salary() {
        return salary;
    }
    void set_department(int new_department) {
        department = new_department;
    }
    int get_department() {
        return department;
    }
};

// creating node which takes employee variables
class DoublySkipList_Node {

public:
    Employee* data;
    DoublySkipList_Node* next;
    DoublySkipList_Node* below;
    DoublySkipList_Node(Employee* data, DoublySkipList_Node* next, DoublySkipList_Node* below) : data(data), next(next), below(below) {}

    ~DoublySkipList_Node() {delete data;}
};


class DoublySkipList {
public:
    DoublySkipList_Node* head;
    int height; // max height

    DoublySkipList(int height_in) : height(height_in) {
        Employee* emp_min = new Employee(-1, -1, -1);
        Employee* emp_max = new Employee(INT8_MAX, -1, -1);


        head = new DoublySkipList_Node(emp_min, nullptr, nullptr);
        head->next = new DoublySkipList_Node(emp_max, nullptr, nullptr);

        DoublySkipList_Node* ptr = head;

        //DoublySkipList_Node* temp = nullptr;
        for (int i = 0; i < height_in - 1; i++) {  // Start from 0
            ptr->below = new DoublySkipList_Node(emp_min, nullptr, nullptr);
            ptr->below->next = new DoublySkipList_Node(emp_max, nullptr, nullptr);
            ptr = ptr->below;
        }
        //ptr->below = nullptr;
    }


    //~DoublySkipList();
    void insert(Employee* data);
    void remove(int remove_id);
    void update(Employee* data);
    Employee* search(int search_id);
    void dumpToFile(ofstream& out_file);
    int get_level();
    void deallocater();
    void visualizer(vector<vector<int> >& arr);
};


int DoublySkipList::get_level(){
    int res = rand() % height + 1;
    return res;
}


void DoublySkipList::insert(Employee* emp) {
    int random_height = get_level();
    DoublySkipList_Node* ptr = head;

    // aranan yükseklik
    for(int i = 0; i < height - random_height; i++){
        ptr = ptr->below;
    }

    DoublySkipList_Node* above_temp = nullptr;

    while(ptr){
        while(ptr->next != nullptr && ptr->next->data->get_id() < emp->get_id()){
            ptr = ptr->next;
        }

        DoublySkipList_Node* new_emp = new DoublySkipList_Node(emp, nullptr, nullptr);

        if(above_temp != nullptr){
            above_temp->below = new_emp;
        }
        above_temp = new_emp;

        new_emp->next = ptr->next;
        ptr->next = new_emp;

        ptr = ptr->below;
    }
}


void DoublySkipList::remove(int id) {
    DoublySkipList_Node* ptr = head;

    while(ptr){
        while(ptr->next && ptr->next->data->get_id() < id){
            ptr = ptr->next;
        }

        if(ptr->next->next && ptr->next->data->get_id() == id){
            DoublySkipList_Node* temp = ptr->next;
            ptr->next = temp->next;

            // is it OK?
            temp->data = nullptr;
            temp = nullptr;
            //delete temp->data;
            //delete temp;
        }
        else if(ptr->next->next == nullptr && ptr->next->data->get_id() == id){
            DoublySkipList_Node* temp = ptr->next;
            ptr->next = nullptr;

            // is it OK?
            temp->data = nullptr;
            temp = nullptr;
            //delete temp->data;
            //delete temp;
        }

        ptr = ptr->below;
    }
}


void DoublySkipList::update(Employee* upd_data){
    int id = upd_data->get_id();
    DoublySkipList_Node* ptr = head;
    bool is_exist = false;

    if(ptr->next->data->get_id() == id){
        is_exist = true;
        while(ptr){
            ptr->next->data->set_salary(upd_data->get_salary());
            ptr->next->data->set_department(upd_data->get_department());
            ptr = ptr->below;
        }
    }
    // else
    while(ptr){
        while(ptr->next != nullptr && ptr->next->data->get_id() < id) {
            ptr = ptr->next;
        }

        if(ptr->next->data->get_id() == id){
            is_exist = true;
            ptr->next->data->set_salary(upd_data->get_salary());
            ptr->next->data->set_department(upd_data->get_department());
        }
        ptr = ptr->below;
    }

    if(is_exist == false){
        cout << "ERROR: An invalid ID to update\n";
    }  
}



Employee* DoublySkipList::search(int id){
    DoublySkipList_Node* ptr = head;
    // alttan da arayabiliriz belki ona çeviririm
    //while(ptr->below != nullptr)

    while(ptr){
        while (ptr->next != nullptr && ptr->next->data->get_id() < id) {
            ptr = ptr->next;
        }
        if(ptr->next->data->get_id() < id){
            ptr = ptr->below;
        }
        else if(ptr->next->data->get_id() == id){
            break;
        }
    }
    return ptr->next->data;
}


void DoublySkipList::dumpToFile(ofstream& out_file){
    DoublySkipList_Node* ptr = head;
    while(ptr != nullptr && ptr->below != nullptr){
        ptr = ptr->below;
    }
    ptr = ptr->next;
    while(ptr != nullptr && ptr->next != nullptr){
        out_file << ptr->data->get_id() << ";" << ptr->data->get_salary() << ";" << ptr->data->get_department() << "\n";
        ptr = ptr->next;
    }
}


void DoublySkipList::deallocater(){
    DoublySkipList_Node* ptr = head;

    while(ptr != nullptr){
        DoublySkipList_Node* temp = ptr;

        while(temp->next != nullptr){
            DoublySkipList_Node* del = temp;
            temp = temp->next;
            

            del->data = nullptr;
            del = nullptr;
            //delete del->data;
            //delete del;

            
        }
        ptr = ptr->below;
    }
    
    // last row
    while(ptr != nullptr){
        DoublySkipList_Node* del = ptr;
        ptr = ptr->next;

        del->data= nullptr;
        del = nullptr;
        //delete del->data;
        //delete del;

        
    }
    //delete ptr;
    //delete head;

    ptr = nullptr;
    head = nullptr;  
}




void DoublySkipList::visualizer(vector<vector<int> >& arr){
    //DoublySkipList_Node* p = head;
    DoublySkipList_Node* row_holder = head;
    while(row_holder){
        DoublySkipList_Node* p = row_holder;

        vector<int> row;

        row.push_back(-1); // initial val;
        while(p->next != nullptr){
            row.push_back(p->next->data->get_id());
            p = p->next;
        }
        arr.push_back(row);
        row_holder = row_holder->below;
    }
}

int main(int argc, char* argv[]){
    //clock_t start = clock();
    srand((unsigned int)time(NULL));

    if(argc != 3) return -1;

    string my_data_file = argv[1];
    string my_operation_file = argv[2];

    ////
    int height = 10;
    DoublySkipList* skiplist = new DoublySkipList(height);
    ////

    ifstream file;
    file.open(my_data_file);

    string title;
    getline(file, title);

    int max_id = 0;
    string line;
    while(getline(file, line)){
        //cout << "---->- BURADA\n";
        istringstream ss(line);
        string id_str, salary_str, department_str;
        if(getline(ss, id_str, ';') && getline(ss, salary_str, ';') && getline(ss, department_str, ';')){
            int id_int = stoi(id_str);
            int salary_int = stoi(salary_str);
            int department_int = stoi(department_str);
            Employee* employee = new Employee(id_int, salary_int, department_int);
            //cout << "CONT_point1\n";
            skiplist->insert(employee);
            //cout << "CONT_point2\n";
        
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
            //cout << "--ADD--" << "\n";
            int id_will_add = max_id + 1;
            max_id += 1;

            string salary_str, department_str;
            if(getline(ss, salary_str, ';') && getline(ss, department_str, ';')){
                int salary_int = stoi(salary_str);
                int department_int = stoi(department_str);
                Employee* employee = new Employee(id_will_add, salary_int, department_int);
                skiplist->insert(employee);
            }
        } 
        else if(op == "UPDATE"){
            //cout << "--UPDATE--" << "\n";
            string id_str, salary_str, department_str;
            if(getline(ss, id_str, ';') && getline(ss, salary_str, ';') && getline(ss, department_str, ';')){
                int id_int = stoi(id_str);
                int salary_int = stoi(salary_str);
                int department_int = stoi(department_str);
                Employee* upd_employee = new Employee(id_int, salary_int, department_int);
                skiplist->update(upd_employee);
            }
        }
        else if(op == "DELETE"){
            //cout << "--DELETE--" << "\n";
            string id_str;
            if(getline(ss, id_str, ';')){
                int id_int = stoi(id_str);
                skiplist->remove(id_int);
            }
        }    
    }

    /* visualize skiplist
    
    vector<vector<int> > arr;
    skiplist->visualizer(arr);

    for(int i = 0; i < arr.size(); i++){
        for(int j = 0; j < arr[i].size(); j++){
            cout << " " << arr[i][j] << " ";
        }
        cout << "\n";
    }
    */




    //PRINTING TIEM
    // 2 skiplist
    ofstream out_file;
    out_file.open("doubly_output.csv");
    out_file << title << "\n";
    skiplist->dumpToFile(out_file);

    // dont forget t odeallocate data in skiplist
    skiplist->deallocater();
    //delete skiplist;
    skiplist = nullptr;
    

    //clock_t end = clock();
    //cout << (double)(end - start) * 1000 / CLOCKS_PER_SEC << "\n";
    return 0;
}
