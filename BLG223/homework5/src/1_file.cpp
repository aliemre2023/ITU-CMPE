/* @Author
StudentName: Ali Emre Kaya
StudentID: 150210097
Date: 24 October 2023
*/


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>


// run
// g++ -o ./src/main ./src/1_file.cpp 
// ./src/main ./dataset/1000.csv ./dataset/UPDATE1000.csv


using namespace std;

int main(int argc, char* argv[]){
    clock_t start = clock();

    if(argc != 3){
        return -1;
    }
    string my_data_file, my_operation_file;
    my_data_file = argv[1];
    my_operation_file = argv[2];
    
    
    ifstream operations;
    operations.open(my_operation_file);
    string query;

    while(getline(operations, query)){
        
        istringstream query_line(query);
        string op; // ADD, UPDATE, DELETE

    
        getline(query_line, op, ';');
        if(op == "ADD"){
            //cout << "in ADD" << endl;

            //fstream file;
            //file.open("/Users/aliemre2023/Desktop/hw1/file-solution/hw1.csv", ios::app | ios::in | ios::out);
            ifstream file;
            file.open(my_data_file);
            string title_line; // unnecessary row
            getline(file, title_line);

            fstream file_edit;
            file_edit.open(my_data_file, ios::app);

            
            
            // I found max id in there
            int max_id = 0;
            string file_line;
            while(getline(file ,file_line)){
                string id_str;
                istringstream file_line_to_use(file_line);
                getline(file_line_to_use, id_str, ';');
                int id_int;
                id_int = stoi(id_str);
                
                max_id = max(max_id, id_int);
            }

            // should I use string library
            int new_employee_id = max_id + 1; 
            string new_id_str = to_string(new_employee_id);  
            string salary_str;
            getline(query_line, salary_str, ';');
            string department_str;
            getline(query_line, department_str, ';');

            file_edit << new_employee_id << ";" << salary_str << ";" << department_str << "\n";

            file.close();
            
        }
        else if(op == "UPDATE"){
            //cout << "in UPDATE" << endl;

            //fstream file;
            //file.open("/Users/aliemre2023/Desktop/hw1/file-solution/hw1.csv", ios::app | ios::in | ios::out);
            ifstream file;
            file.open(my_data_file);
            string title_line; // unnecessary row
            getline(file, title_line);

            //temporary file to store updated data
            ofstream file_edit; 
            file_edit.open("temp.csv", ios::out);

            // copy the header
            file_edit << title_line << "\n";

            // upadeted's data
            string updated_id;
            getline(query_line, updated_id, ';');
            string salary_str;
            getline(query_line, salary_str, ';');
            string department_str;
            getline(query_line, department_str, ';');

            bool is_id_valid = true;

            string file_line;
            // getline(file, file_line); buralarda biy yerde sorunlar çıkabilir
            while(getline(file, file_line)){
                istringstream file_line_to_use(file_line);
                string current_id;
                getline(file_line_to_use, current_id, ';');
                string salary_old;
                getline(file_line_to_use, salary_old, ';');
                string department_old;
                getline(file_line_to_use, department_old, ';');

                if(current_id == updated_id){
                    is_id_valid = false;
                    file_edit << updated_id << ";" << salary_str << ";" << department_str << "\n";
                }
                else{
                    file_edit << current_id << ";" << salary_old << ";" << department_old << "\n";
                }
            }
            if(is_id_valid){
                cout << "ERROR: An invalid ID to update" << "\n";
            }

 
            file.close();
            file_edit.close();   

            remove(my_data_file.c_str());
            rename("temp.csv", my_data_file.c_str());
        }
        else if(op == "DELETE"){
            //cout << "in DELETE" << endl;

            ifstream file;
            //file.open("/Users/aliemre2023/Desktop/hw1/file-solution/hw1.csv", ios::in,  ios::out);
            file.open(my_data_file);
            string title_line; // unnecessary row
            getline(file, title_line);

            ofstream file_edit;
            file_edit.open("temp.csv");
            file_edit << title_line << "\n";

            
            string deleted_id;
            getline(query_line, deleted_id, ';');
            int deleted_id_int = stoi(deleted_id);

            bool is_id_valid = true;
            bool is_file_empty = true;
            
            string file_line;
            while(getline(file, file_line)){
                is_file_empty = false;
                
                istringstream file_line_to_use(file_line);
                string current_id;
                getline(file_line_to_use, current_id, ';');
                string salary_old;
                getline(file_line_to_use, salary_old, ';');
                string department_old;
                getline(file_line_to_use, department_old, ';');


                int current_id_int = stoi(current_id);
                if(current_id_int != deleted_id_int){
                    file_edit << current_id << ";" << salary_old << ";" << department_old << "\n";
                }
                else{
                    // pass the line
                    is_id_valid = false;
                    continue;
                }
            }
            if(is_file_empty){
                cout << "ERROR: There is no Employee" << "\n";
            }
            if(is_id_valid){
                cout << "ERROR: An invalid ID to delete" << "\n";
            }

            file.close();
            file_edit.close();

            remove(my_data_file.c_str());
            rename("temp.csv", my_data_file.c_str());            
        }
    }
    operations.close();

    clock_t end = clock();
    cout << (double)(end-start) * 1000 / CLOCKS_PER_SEC << endl;

    
    return 0;
}