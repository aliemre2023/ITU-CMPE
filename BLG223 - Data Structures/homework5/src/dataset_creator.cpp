#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// run
// g++ -o main ./dataset_creator.cpp 
// ./main 100000.csv

int main(int argc, char* argv[]){
    //clock_t start = clock();

    if(argc != 2){
        return -1;
    }

    string my_file = argv[1];

    ifstream file;
    file.open(my_file);

    string line;
    string title;
    getline(file, title);
    
    // wanted row;
    int row;
    cin >> row;
    
    ofstream new_file;
    new_file.open(to_string(row) + ".csv");
    new_file << title << "\n";

    
    string new_row;
    while(getline(file, new_row) && row > 0){
        new_file << new_row << "\n";
        row--;
    }
    
  
    new_file.close();
    file.close();
    
    return 0;
}