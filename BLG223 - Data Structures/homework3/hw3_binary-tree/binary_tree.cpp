/* @Author
StudentName: Ali Emre Kaya
StudentID: 150210097
Date: 1 December 2023
*/


// run
// g++ -o myprogram main.cpp 
// ./myprogram dataset2.csv operations_1_2.csv

#include <iostream>
#include <fstream>
#include <sstream>
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
        Node* root;
        Node*& insert(Node*& root, Employee* employee);
        Node* remove(Node* root, int del_id);
        void update(Node* root, Employee* employee);
        void printInorder(Node* root, ofstream& bt_file); // printToFile
        void printer(Node* root); // printToConsole
        int heighter(Node* root);
        BinaryTree() : root(nullptr) {}
};


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

Node* BinaryTree::remove(Node* root, int del_id) {
    Node* curr = root;
    Node* parent = nullptr;

    // Search for the node to be deleted
    while (curr != nullptr && curr->employee->get_id() != del_id) {
        parent = curr;
        if (del_id < curr->employee->get_id())
            curr = curr->left;
        else
            curr = curr->right;
    }

    if (curr == nullptr) {
        cout << "ERROR: An invalid ID to delete" << "\n";
        return root;
    }

    // no child
    if (curr->left == nullptr && curr->right == nullptr) {
        if (parent == nullptr)
            delete root;
        else if (parent->left == curr)
            parent->left = nullptr;
        else
            parent->right = nullptr;
        delete curr->employee;
        delete curr;
    }
    // one child
    else if (curr->left == nullptr || curr->right == nullptr) {
        Node* child = (curr->left != nullptr) ? curr->left : curr->right;
        if (parent == nullptr)
            root = child;
        else if (parent->left == curr)
            parent->left = child;
        else
            parent->right = child;
        delete curr->employee;
        delete curr;
    }
    // two child
    else {
        Node* replace = curr->right;
        Node* replaceParent = curr;

        while (replace->left != nullptr) {
            replaceParent = replace;
            replace = replaceParent->left;
        }

        // Copy the data of the successor to the current node
        curr->employee->set_id(replace->employee->get_id());
        curr->employee->set_salary(replace->employee->get_salary());
        curr->employee->set_department(replace->employee->get_department());

        // Remove the successor node
        if (replaceParent->left == replace)
            replaceParent->left = replace->right;
        else
            replaceParent->right = replace->right;
            
        delete replace->employee;
        delete replace;
    }

    return root;
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



// inorder printing -1-1-1
void BinaryTree::printInorder(Node* root, ofstream& bt_file){
    if (root == nullptr)
        return;

    
    printInorder(root->left, bt_file); 
    bt_file << root->employee->get_id() << ";" << root->employee->get_salary() << ";" << root->employee->get_department() << "\n";
    printInorder(root->right, bt_file);
    
    
}
// preorder
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

            Employee* employee = new Employee(id_int, salary_int, department_int);
            employee_tree.root = employee_tree.insert(employee_tree.root, employee);

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
            //cout << "--ADD" << "\n";
            string salary_str, department_str;
            if(getline(ss, salary_str, ';') && getline(ss, department_str, ';')){
                int salary_int = stoi(salary_str);
                int department_int = stoi(department_str);

                max_id_in_tree += 1;
                Employee* new_employee = new Employee(max_id_in_tree, salary_int, department_int);
    
                employee_tree.root = employee_tree.insert(employee_tree.root, new_employee);
            }
        }
        else if(o == "UPDATE"){
            //cout << "--UPD" << "\n";
            string id_str, salary_str, department_str;
            if (getline(ss, id_str, ';') && getline(ss, salary_str, ';') && getline(ss, department_str, ';')) {
                int id_int = stoi(id_str);
                int salary_int = stoi(salary_str);
                int department_int = stoi(department_str);
                
                Employee* updated_employee = new Employee(id_int, salary_int, department_int);
                Node* temp_root = employee_tree.root;
                employee_tree.update(temp_root, updated_employee);
            }
        }
        else if(o == "DELETE"){
            //cout << "--DEL" << "\n";
            string id_str;
            if (getline(ss, id_str, ';')) {
                int id_int = stoi(id_str);
                Node* temp_root = employee_tree.root;
                temp_root = employee_tree.remove(temp_root, id_int);
            }
        }
        else if(o[0] == 'P'){
            cout << "P \n";
            Node* temp_root = employee_tree.root;
            employee_tree.printer(temp_root);
            //cout << "\n";
        }
        else if(o[0] == 'H'){
            Node* temp_root = employee_tree.root;
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
    employee_tree.printInorder(employee_tree.root, bt_file);
 

    file.close();
    operations.close();
    bt_file.close();

    //clock_t end = clock();
    //cout << (double)(end-start) * 1000 / CLOCKS_PER_SEC << endl;
    
    return 0;
}