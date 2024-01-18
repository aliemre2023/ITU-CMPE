#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
using namespace std;

// run
// g++ -o main operation_creator.cpp 
// ./main 100000.csv

int main(int argc, char* argv[]){
    

    /*
    string my_data_file, my_operation_file;
    my_data_file = argv[1];

    ifstream file;
    file.open(my_data_file);

    string line;
    string title;
    getline(file, title);
    
    */



    string op;
    cin >> op;
    // wanted row;
    int row;
    cin >> row;
    
    ofstream new_file;
    new_file.open(op + to_string(row) + ".csv");
    //new_file << title << "\n";

    
    string new_row;
    while(row > 0 && op == "DELETE"){
        int id = rand() % 100000;
        new_file << op + ";" + to_string(id) << "\n";
        row--;
    }
    while(row > 0 && op == "ADD"){
        int salary = rand() % 5000;
        int department = rand() % 10;
        new_file << op + ";" + to_string(salary) + ";" + to_string(department)  << "\n";
        row--;
    }
    while(row > 0 && op == "UPDATE"){
        int id = rand() % 100000;
        int salary = rand() % 5000;
        int department = rand() % 10;
        new_file << op + ";" + to_string(id) + ";" + to_string(salary) + ";" + to_string(department)  << "\n";
        row--;
    }
    
    
    


    //file.close();
    new_file.close();

    return 0;
}