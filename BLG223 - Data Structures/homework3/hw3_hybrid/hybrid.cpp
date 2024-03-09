/* @Author
StudentName: Ali Emre Kaya
StudentID: 150210097
Date: 1 December 2023
*/


// run
// g++ -o myprogram main.cpp 
// ./myprogram dataset1.csv operations.csv

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
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

class Node{
    public:
        Employee* employee;
        Node* left;
        Node* right;
        Node(Employee* emp): employee(emp), left(nullptr), right(nullptr) {}    
};

class BinaryTree{
    public:
        vector<Node*> trees;
        Node*& insertHybrid(Node*& root, Employee* employee);
        Node*& insert(Node*& root, Employee* employee);
        Node* removeHybrid(Node* root, int del_id);
        Node* remove(Node* root, int del_id);
        void updateHybrid(Node* root, Employee* employee);
        void update(Node* root, Employee* employee);
        void printInorder(Node* root, ofstream& bt_file);
        void printer(Node* root);
        int heighter(Node* root);
        BinaryTree() : trees(1, nullptr) {}
};

Node*& BinaryTree::insertHybrid(Node*& root, Employee* new_employee){
    unsigned int tree_index = new_employee->get_id() / 5000;
    if(tree_index >= trees.size()){
        trees.resize(tree_index + 1, nullptr);
    }

    return trees[tree_index] = insert(trees[tree_index], new_employee);
}

Node*& BinaryTree::insert(Node*& root, Employee* new_employee){
    int new_id = new_employee->get_id();
    
    Node* traverser = root;
    Node* new_node = new Node(new_employee);

    if(root == nullptr){
        root = new_node;
    }
    while(traverser){
        if(traverser->employee->get_id() == new_employee->get_id()){
            //cout << "This ID already taken" << "\n";
            break;
        }
        else if(traverser->employee->get_id() > new_id){
            if(traverser->left != nullptr){
                traverser = traverser->left;
            }
            else{
                traverser->left = new_node;
                break;
            }
        }
        else{
            if(traverser->right != nullptr){
                traverser = traverser->right;
            }
            else{
                traverser->right = new_node;
                break;
            }
        }
    }
    return root;
}

Node* BinaryTree::removeHybrid(Node* root, int del_id){
    unsigned int tree_index = del_id / 5000;
    if(tree_index >= trees.size()){
        cout << "ERROR: An invalid ID to delete" << "\n"; 
    }
    else{
        return trees[tree_index] = remove(trees[tree_index], del_id);
    }
    return trees[0];
    
}

Node* BinaryTree::remove(Node* root, int del_id) {
    if(root == nullptr){
        cout << "ERROR: An invalid ID to delete" << "\n";
        return root;
    }
    else if(root != nullptr){
        if(del_id < root->employee->get_id()) 
            root->left = remove(root->left, del_id);
        else if(del_id > root->employee->get_id()) 
            root->right = remove(root->right, del_id);
        else{
            if(root->left == nullptr && root->right == nullptr){
                delete root->employee;
                return nullptr;  
            }
                        
            if(root->left == nullptr || root->right == nullptr){
                delete root->employee;
                return root->left != nullptr ? root->left : root->right; 
            }   

            Node* l_righthold = root->left;                        
            while(l_righthold->right != nullptr){
                l_righthold = l_righthold->right;
            }
            // segmentation faullt veren yer burası
            (root->employee)->set_id((l_righthold->employee)->get_id());
            (root->employee)->set_salary((l_righthold->employee)->get_salary());
            (root->employee)->set_department((l_righthold->employee)->get_department());                           
            root->left = remove(root->left, l_righthold->employee->get_id());
        }
    }
    return root;
}

void BinaryTree::updateHybrid(Node* root, Employee* upd_employee){
    unsigned int upd_id = upd_employee->get_id();
    unsigned int tree_index = upd_id / 5000;
    if(tree_index < trees.size()){
        update(trees[tree_index], upd_employee);
    }
    else{
        cout << "ERROR: An invalid ID to update" << "\n";
    }
}

void BinaryTree::update(Node* root, Employee* upd_employee){
    bool is_exist = false;
    int upd_id = upd_employee->get_id();
    while(root != nullptr){
        if(root->employee->get_id() < upd_id){
            root = root->right;
        }
        else if(root->employee->get_id() > upd_id){
            root = root->left;
        }
        else{
            is_exist = true;
            root->employee = upd_employee;
            break;
        }
    }
    if(!is_exist){
        cout << "ERROR: An invalid ID to update" << "\n";
    }
}



// in printing -1-1-1
void BinaryTree::printInorder(Node* root, ofstream& bt_file){
    if (root == nullptr)
        return;

    
    printInorder(root->left, bt_file); 
    bt_file << root->employee->get_id() << ";" << root->employee->get_salary() << ";" << root->employee->get_department() << "\n";
    printInorder(root->right, bt_file);
    //bt_file << root->employee->get_id() << ";" << root->employee->get_salary() << ";" << root->employee->get_department() << "\n";
    
    
}
void BinaryTree::printer(Node* root){
    if (root == nullptr)
        return;
        
    cout << root->employee->get_id() << ";" << root->employee->get_salary() << ";" << root->employee->get_department() << "\n";  
    printer(root->left);
    printer(root->right);
    
}
int BinaryTree::heighter(Node* root){
    if(root == nullptr){
        return 0;
    }
    else{
        int left_height = heighter(root->left);
        int right_height = heighter(root->right);

        return max(left_height, right_height) + 1;
    }
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

    //BT solution
    BinaryTree employee_tree;

    ifstream file;
    file.open(my_data_file);

    string line;
    string title;
    getline(file, title);

    int max_id_in_tree = 0;
    while(getline(file, line)){
        istringstream ss(line);
        string id_str, salary_str, department_str;
        if(getline(ss, id_str, ';') && getline(ss, salary_str, ';') && getline(ss, department_str, ';')){
            int id_int = stoi(id_str);
            int salary_int = stoi(salary_str);
            int department_int = stoi(department_str);
            
            int tr = id_int / 5000;
            Employee* employee = new Employee(id_int, salary_int, department_int);
            employee_tree.trees[tr] = employee_tree.insertHybrid(employee_tree.trees[tr], employee);

            if(max_id_in_tree < id_int) max_id_in_tree = id_int;
        }   
    }

    ifstream operations;
    operations.open(my_operation_file);
    
    string query;
    while(getline(operations, query)){
        istringstream ss(query);
        string o;
        getline(ss, o, ';');
        //cout << o << "\n";
        if(o == "ADD"){
            //cout << "---ADD" << "\n";
            string salary_str, department_str;
            if(getline(ss, salary_str, ';') && getline(ss, department_str, ';')){
                int salary_int = stoi(salary_str);
                int department_int = stoi(department_str);

                max_id_in_tree += 1;
                int tr = max_id_in_tree / 5000;
                Employee* new_employee = new Employee(max_id_in_tree, salary_int, department_int);
    
                employee_tree.trees[tr] = employee_tree.insertHybrid(employee_tree.trees[tr], new_employee);
            }
        }
        else if(o == "UPDATE"){
            //cout << "---UPD" << "\n";
            string id_str, salary_str, department_str;
            if (getline(ss, id_str, ';') && getline(ss, salary_str, ';') && getline(ss, department_str, ';')) {
                int id_int = stoi(id_str);
                int salary_int = stoi(salary_str);
                int department_int = stoi(department_str);
                int tr = id_int / 5000;
                Employee* updated_employee = new Employee(id_int, salary_int, department_int);
                Node* temp_root = employee_tree.trees[tr];
                employee_tree.updateHybrid(temp_root, updated_employee);
            }
        }
        else if(o == "DELETE"){
            //cout << "---DEL" << "\n";
            string id_str;
            if (getline(ss, id_str, ';')) {
                int id_int = stoi(id_str);
                int tr = id_int / 5000;
                Node* temp_root = employee_tree.trees[tr];
                temp_root = employee_tree.removeHybrid(temp_root, id_int);
            }
        }
        else if(o[0] == 'P'){
            cout << "P \n";
            int tr = max_id_in_tree / 5000;
            Node* temp_root = employee_tree.trees[tr];
            employee_tree.printer(temp_root);
            //cout << "\n";
        }
        else if(o[0] == 'H'){
            int tr = max_id_in_tree / 5000;
            Node* temp_root = employee_tree.trees[tr];
            int height = employee_tree.heighter(temp_root);
            cout << "H " << height << "\n";
        }
        
        else{
            //cout << "Wrong Operation\n";
        }
    }


    // bt_solution
    ofstream bt_file;
    bt_file.open("output.csv");
    bt_file << title << "\n";
    //employee_tree.printPostorder(employee_tree.trees[1], bt_file);
    
    for(int i = 0; i < (max_id_in_tree / 5000) + 1; i++){
        employee_tree.printInorder(employee_tree.trees[i], bt_file);
    }
    
    
 

    file.close();
    operations.close();
    bt_file.close();

    //clock_t end = clock();
    //cout << (double)(end-start) * 1000 / CLOCKS_PER_SEC << endl;
    
    return 0;
} 