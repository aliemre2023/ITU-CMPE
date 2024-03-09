/* @Author
StudentName: Ali Emre Kaya
StudentID: 150210097
Date: 28 December 2023
*/

// run
// g++ -o ./src/main ./src/3_quadruply_skiplist.cpp 
// ./myprogram 100000.csv DELETE10000.csv 


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
class QuadruplySkipList_Node {

public:
    Employee* data;
    QuadruplySkipList_Node* next;
    QuadruplySkipList_Node* below;
    QuadruplySkipList_Node* prev;
    QuadruplySkipList_Node* above;

    QuadruplySkipList_Node(Employee* data, QuadruplySkipList_Node* next, QuadruplySkipList_Node* below, QuadruplySkipList_Node* prev, QuadruplySkipList_Node* above) : data(data), next(next), below(below), prev(prev), above(above) {}

    //~QuadruplySkipList_Node() {delete data;}
};

class QuadruplySkipList {
public:
    QuadruplySkipList_Node* head;
    int height; // max height

    QuadruplySkipList(int height_in) : height(height_in) {
        Employee* emp_min = new Employee(-1, -1, -1);
        Employee* emp_max = new Employee(INT8_MAX, -1, -1);


        head = new QuadruplySkipList_Node(emp_min, nullptr, nullptr, nullptr, nullptr);
        head->next = new QuadruplySkipList_Node(emp_max, nullptr, nullptr, nullptr, nullptr);
        head->next->prev = head;

        QuadruplySkipList_Node* ptr = head;

        //QuadruplySkipList_Node* temp = nullptr;
        for (int i = 0; i < height_in - 1; i++) {
            ptr->below = new QuadruplySkipList_Node(emp_min, nullptr, nullptr, nullptr, nullptr);
            ptr->below->next = new QuadruplySkipList_Node(emp_max, nullptr, nullptr, nullptr, nullptr);

            ptr = ptr->below;
        }
        //ptr->below = nullptr;
    }


    //~QuadruplySkipList();
    void insert(Employee* data);
    void remove(int remove_id);
    void update(Employee* data);
    Employee* search(int search_id);
    void dumpToFile(ofstream& out_file);
    void deallocater();
    void visualizer(vector<vector<int> >& arr);
    int get_level();
};

int QuadruplySkipList::get_level(){
    int res = rand() % height + 1;
    return res;
}

void QuadruplySkipList::insert(Employee* emp) {
    int random_height = get_level();
    QuadruplySkipList_Node* ptr = head;

    // aranan yükseklik
    for (int i = 0; i < height - random_height; i++) {
        ptr = ptr->below;
    }

    QuadruplySkipList_Node* above_temp = nullptr;

    while(ptr){
        while (ptr->next != nullptr && ptr->next->data->get_id() < emp->get_id()) {
            ptr = ptr->next;
        }

        QuadruplySkipList_Node* new_emp = new QuadruplySkipList_Node(emp, nullptr, nullptr, nullptr, nullptr);

        if (above_temp != nullptr) {
            above_temp->below = new_emp;
            new_emp->above = above_temp;
        }
        above_temp = new_emp;
        new_emp->prev = ptr;
        if(ptr->next){
            ptr->next->prev = new_emp;
        }
        new_emp->next = ptr->next;
        ptr->next = new_emp;
        ptr = ptr->below;
    }
}

void QuadruplySkipList::remove(int id) {
    QuadruplySkipList_Node* ptr = head;
    if(ptr == nullptr){
        cout << "ERROR: There is no Employee\n";
        return;
    }

    while(ptr->below) {
        ptr = ptr->below;
    }

    while (ptr->next && ptr->next->data->get_id() != id) {
        ptr = ptr->next;
    }
    if(ptr->next == nullptr){
        cout << "ERROR: An invalid ID to delete\n";
        return;
    }

    if (ptr->next && ptr->next->data->get_id() == id) {
        QuadruplySkipList_Node* temp = ptr->next; //temp will deleted
        ptr->next = temp->next; // next arranged

        if(temp->next) {
            temp->next->prev = ptr; // prev arranged
        }
        // else, there is no need to do *(prev <- nullptr)

        while(temp->above){
            temp = temp->above;
            temp->prev->next = temp->next;

            if(temp->next){
                temp->next->prev = temp->prev;
            }
        }

        //delete temp->data;
        //delete temp;
        temp->data = nullptr;
        temp = nullptr;
    }
}


void QuadruplySkipList::update(Employee* upd_data){
    int id = upd_data->get_id();
    QuadruplySkipList_Node* ptr = head;
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


Employee* QuadruplySkipList::search(int id){
    QuadruplySkipList_Node* ptr = head;

    while (ptr->next != nullptr && ptr->next->data->get_id() < id) {
        ptr = ptr->next;
    }
    return ptr->next->data;
}

void QuadruplySkipList::dumpToFile(ofstream& out_file){
    QuadruplySkipList_Node* ptr = head;
    while(ptr != nullptr && ptr->below != nullptr){
        ptr = ptr->below;
    }
    ptr = ptr->next;
    while(ptr != nullptr && ptr->next != nullptr){
        out_file << ptr->data->get_id() << ";" << ptr->data->get_salary() << ";" << ptr->data->get_department() << "\n";
        ptr = ptr->next;
    }
}


void QuadruplySkipList::deallocater(){
    QuadruplySkipList_Node* ptr = head;

    while(ptr != nullptr){
        QuadruplySkipList_Node* temp = ptr;

        while(temp->next != nullptr){
            QuadruplySkipList_Node* del = temp;
            temp = temp->next;
            // is it because of being on stack. probably
            //delete del->data;
            //delete del;

            del->data = nullptr;
            del = nullptr;
        }
        ptr = ptr->below;
    }
    
    // last row
    while(ptr != nullptr){
        QuadruplySkipList_Node* del = ptr;
        ptr = ptr->next;
        //delete del->data;
        //delete del;

        del->data= nullptr;
        del = nullptr;
    }
    //delete ptr;
    //delete head;

    ptr = nullptr;
    head = nullptr;  
}


void QuadruplySkipList::visualizer(vector<vector<int> >& arr){
    //QuadruplySkipList_Node* p = head;
    QuadruplySkipList_Node* row_holder = head;
    while(row_holder){
        QuadruplySkipList_Node* p = row_holder;

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
    clock_t start = clock();
    srand((unsigned int)time(NULL));

    if(argc != 3) return -1;

    string my_data_file = argv[1];
    string my_operation_file = argv[2];

    ////
    int height = 10;
    QuadruplySkipList* skiplist = new QuadruplySkipList(height);
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
            //cout << "point1\n";
            skiplist->insert(employee);
            //cout << "point2\n";
        
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

    /* // visualize skiplist
    
    vector<vector<int> > arr;
    skiplist->visualizer(arr);

    for(int i = 0; i < arr.size(); i++){
        for(int j = 0; j < arr[i].size(); j++){
            cout << " " << arr[i][j] << " ";
        }
        cout << "\n";
    }
    */




    //cout << "PRINTING TIME\n";
    // 2 skiplist
    ofstream out_file;
    out_file.open("quadruply_output.csv");
    out_file << title << "\n";
    skiplist->dumpToFile(out_file);

    skiplist->deallocater();
    // dont forget t odeallocate data in skiplist
    //delete skiplist;
    skiplist = nullptr;
    

    clock_t end = clock();
    cout << (double)(end - start) * 1000 / CLOCKS_PER_SEC << "\n";
    return 0;
}
