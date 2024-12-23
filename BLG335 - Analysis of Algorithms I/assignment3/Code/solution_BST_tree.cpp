#include <iostream> 
#include <stdlib.h>
#include <iomanip>
#include <string.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stack>
#include <iomanip>
#include <chrono>
#include <random>
//#include <queue>
using namespace std;
using namespace std::chrono;

/////////////////// Player ///////////////////
class publisher
{
public:
	string name;
	float na_sales;
	float eu_sales;
	float others_sales;
};

/////////////////// Node ///////////////////
class Node{
	public:
		publisher key;
		int color; // "Red"=1 or "Black"=0
		Node *parent, *left, *right;
	
		Node(publisher);
		~Node();
		int get_color();
        void set_color(int);
};

/////////////////// BST-Tree ///////////////////
class BST_tree{
	private:
		Node* root;
	public:	
        publisher* best_seller[3];	
		stack<string> tree_deep_stack;

        Node* get_root();

		Node* BST_insert(Node* root, Node* ptr);
		void insertValue(vector<string>);
        void find_best_seller(); 

        BST_tree();
		~BST_tree();
};

void print_best_sellers(int year, publisher* temp_publisher[3]){
    cout.precision(5);
    cout<< "End of the "+to_string(year)+" Year"<<endl;
    cout<< "Best seller in North America: "+temp_publisher[0]->name+" - "<<temp_publisher[0]->na_sales<<" million"<<endl;
    cout<< "Best seller in Europe: "+temp_publisher[1]->name+" - "<<temp_publisher[1]->eu_sales<<" million"<<endl;
    cout<< "Best seller rest of the World: "+temp_publisher[2]->name+" - "<<temp_publisher[2]->others_sales<<" million"<<endl;
}

vector<vector<string>> csv_to_vector(string file_name){
    vector<vector<string>> info;

	ifstream file;
	file.open(file_name);

	string title;
	getline(file, title);

	string line;
    int line_number = 0;
	while(getline(file, line)){
        line_number++;
	
        string Name, Platform, Year_of_Release, Publisher, NA_Sales, EU_Sales, Other_Sales;

        size_t pos = 0;
        size_t delimiter_pos;
        int column_index = 0;

        while ((delimiter_pos = line.find(',', pos)) != string::npos) {
            string field = line.substr(pos, delimiter_pos - pos);
            switch (column_index) {
                case 0: Name = field; break;
                case 1: Platform = field; break;
                case 2: Year_of_Release = field; break;
                case 3: Publisher = field; break;
                case 4: NA_Sales = field; break;
                case 5: EU_Sales = field; break;
                case 6: Other_Sales = field; break;
            }
            pos = delimiter_pos + 1;
            column_index++;
        }
        if (pos < line.size()) {
            string last_field = line.substr(pos);
            Other_Sales = last_field; 
        }

        /*
        cout << "Name: " << Name << ", Platform: " << Platform
             << ", Year: " << Year_of_Release << ", Publisher: " << Publisher
             << ", NA Sales: " << NA_Sales << ", EU Sales: " << EU_Sales
             << ", Other Sales: " << Other_Sales << "\n";
        */   

        if (NA_Sales.empty() || EU_Sales.empty() || Other_Sales.empty()) {
            cerr << "Warning " << line_number << ": Skipping row with missing sales data: " << line << endl;
            continue;
        }

        if (!isdigit(NA_Sales[0]) || !isdigit(EU_Sales[0]) || !isdigit(Other_Sales[0])) {
            cerr << "Warning " << line_number << ": Skipping row with invalid sales data: " << line  << endl;
            continue;
        }

        info.push_back({Name, Platform, Year_of_Release, Publisher, NA_Sales, EU_Sales, Other_Sales});

    }

    return info;


}

BST_tree generate_BST_tree_from_csv(string file_name){
    auto start_time = high_resolution_clock::now();
    
    BST_tree temp_BSTtree;

    // Fill this function.
    vector<vector<string>> info = csv_to_vector(file_name);

    int decade = -1;
    for(int i = 0; i< info.size(); i++){
        string Year_of_Release = info[i][2];


        // logical one
        /*
    
        if(decade == -1) decade = stoi(Year_of_Release) / 10;


        int new_decade = stoi(Year_of_Release) / 10;
        if(decade != new_decade){
            //cout << "DECADE CHANGE" << "\n";
            
            temp_BSTtree.find_best_seller();

            print_best_sellers(stoi(Year_of_Release), temp_BSTtree.best_seller);

            decade = new_decade;
        }
        */

        if(decade == -1) decade = ((stoi(Year_of_Release) / 10) * 10) + 11; // wanted


        int new_decade = stoi(Year_of_Release);
        if(decade == new_decade){
            //cout << "DECADE CHANGE" << "\n";
            
            temp_BSTtree.find_best_seller();

            print_best_sellers(stoi(Year_of_Release)-1, temp_BSTtree.best_seller);

            decade = ((stoi(Year_of_Release) / 10) * 10) + 11;
        }

        //vector<string> publisher_data = {Name, Platform, Year_of_Release, Publisher, NA_Sales, EU_Sales, Other_Sales};
        temp_BSTtree.insertValue(info[i]);
    }

    // im not sure of that
    int last_year = ((stoi(info[info.size()-1][2]) / 10) * 10);
    print_best_sellers(last_year, temp_BSTtree.best_seller);

    
    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end_time - start_time);
    cout << "Total time taken for construction: " << duration.count() << " microseconds" << endl;

    return temp_BSTtree;
}


void search_in_BST(BST_tree tree, string name){
    Node* current = tree.get_root();
    while(current != nullptr){
        if(name == current->key.name){
            return;
        }
        else if(name < current->key.name){
            current = current->left;
        }
        else{
            current = current->right;
        }
    }
    // cant find, which is impossible
    return;
}
void random_50_search(vector<vector<string>> info, BST_tree tree){
    //vector<vector<string>> info = csv_to_vector(fname);
    vector<string> random_50_names;

    /*
    for(int i = 0; i < 50; i++){
        int random_index = rand() % info.size();
        random_50_names.push_back(info[random_index][0]);
    }
    */

    // search with deepest 54 val
    vector<int> random_index_array = {9241, 9241, 9241, 9241, 10439,4625,7154,5636,4928,640,926,10612,2610,7161,4394,2842,7845,7416,2213,16031,10745,1666,13330,7087,4662,7237,11303,4115,5929,14162,6167,16014,5543,7691,9813,9334,11235,6447,10935,521,8117,150,3461,5106,4820,4145,9377,5378,1888,12480,7580,467,4848,13086};
    //vector<int> random_index_array = {10439,4625,7154,5636,4928,640,926,10612,2610,7161,4394,2842,7845,7416,2213,16031,10745,1666,13330,7087,4662,7237,11303,4115,5929,14162,6167,16014,5543,7691,9813,9334,11235,6447,10935,521,8117,150,3461,5106,4820,4145,9377,5378,1888,12480,7580,467,4848,13086};
    for(int i = 0; i < random_index_array.size(); i++){
        random_50_names.push_back(info[random_index_array[i]][0]);
    }
    
    
    
    auto start_time = high_resolution_clock::now();
    for(int i = 0; i < random_50_names.size(); i++){
        search_in_BST(tree, random_50_names[i]);
    }
    auto end_time = high_resolution_clock::now();

    auto duration = duration_cast<nanoseconds>(end_time - start_time);

    cout << "Total time taken for 50 searches: " << duration.count() << " nanoseconds" << endl;
    cout << "Average time taken for 50 searches: " << duration.count() / random_index_array.size() << " nanoseconds" << endl;
}

bool compare(const vector<string>& a, const vector<string>& b) {
    float na_sales_a = stof(a[4]);
    float na_sales_b = stof(b[4]);

    if (na_sales_a != na_sales_b) {
        return na_sales_a > na_sales_b; 
    }

    float eu_sales_a = stof(a[5]);
    float eu_sales_b = stof(b[5]);

    if (eu_sales_a != eu_sales_b) {
        return eu_sales_a > eu_sales_b; 
    }

    float other_sales_a = stof(a[6]); 
    float other_sales_b = stof(b[6]);

    return other_sales_a > other_sales_b; 
}

bool compare_name(const vector<string>& a, const vector<string>& b) {
    return a[3] < b[3];
}

void save_to_csv(string file_name, vector<vector<string>> info){
    ofstream file(file_name);

    for (auto row : info) {
        for (int i = 0; i < row.size(); ++i) {
            file << row[i];
            if (i != row.size() - 1) {
                file << ","; 
            }
        }
        file << "\n";
    }
    file.close();
}

int getHeight(Node* node){
    if(node == nullptr){
        return 0;
    }

    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);
    return 1 + max(leftHeight, rightHeight);
}



void helper_inorder(Node* node, vector<Node*>& sorted_array) {
    if (node == nullptr) {
        return;
    }

    helper_inorder(node->left, sorted_array);
    sorted_array.push_back(node);
    helper_inorder(node->right, sorted_array);
}

vector<Node*> tree_to_sortedArray_withInorder(BST_tree BSTtree){
    vector<Node*> sorted_array;
    helper_inorder(BSTtree.get_root(), sorted_array);
    return sorted_array;
}

BST_tree generate_BST_tree_from_array(vector<Node*> array){
    auto start_time = high_resolution_clock::now();
    
    BST_tree temp_BSTtree;

    for(int i = 0; i< array.size(); i++){// not very trustful values but names are okay
        vector<string> publisher_data = {array[i]->key.name, "", "", array[i]->key.name, to_string(array[i]->key.na_sales), to_string(array[i]->key.eu_sales), to_string(array[i]->key.others_sales)};
        temp_BSTtree.insertValue(publisher_data);
    }
    
    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end_time - start_time);
    cout << "Total time taken for sorted construction: " << duration.count() << " microseconds" << endl;

    return temp_BSTtree;
}

/*
Node* bfs_findDeepestNode(Node* root) {
    if (root == nullptr) {
        return nullptr;
    }

    queue<Node*> q;
    q.push(root);
    Node* current = nullptr;

    while (!q.empty()) {
        current = q.front();
        q.pop();

        if (current->left) {
            q.push(current->left);
        }
        if (current->right) {
            q.push(current->right);
        }
    }

    return current;
}
*/


////////////////////////////////////////////
//----------------- MAIN -----------------//
////////////////////////////////////////////
int main(int argc, char* argv[]){

	string fname = argv[1];	

    // Fill this function.

    
    // when generating, also find the best sellers
    cout << "==============================================================" << "\n";
    BST_tree BSTtree = generate_BST_tree_from_csv(fname);

    // search efficiently
    cout << "==============================================================" << "\n";
    vector<vector<string>> info = csv_to_vector(fname);
    //cout << info[9241][3] << "\n"; // surely deepest
    random_50_search(info, BSTtree);

    cout << "==============================================================" << "\n";
    cout << "Normal height:" << getHeight(BSTtree.get_root()) << "\n";

    

    // sorted random 50
    cout << "==============================================================" << "\n";
    vector<vector<string>> info2 = csv_to_vector(fname);
    sort(info2.begin(), info2.end(), compare_name);
    save_to_csv("sorted.csv", info2);
    // it's decent calculation is false because, i assume years are sorted in previous one
    cout << "-!-!-!-!-THIS IS WRONG DECADE OPERATION, TRUE ONE IN THE UPSIDE ONE-!-!-!-!-" << "\n";
    BST_tree BSTtree_sorted = generate_BST_tree_from_csv("sorted.csv");
    cout << "-!-!-!-!-THIS IS WRONG DECADE OPERATION, TRUE ONE IN THE UPSIDE ONE-!-!-!-!-" << "\n";
    cout << "==============================================================" << "\n";
    random_50_search(info2, BSTtree_sorted);

    // height
    cout << "==============================================================" << "\n";
    cout << "Sorted height:" << getHeight(BSTtree_sorted.get_root()) << "\n";

    
    cout << "==============================================================" << "\n";
    cout << "Sorted array to Tree" << "\n";
    vector<Node*> sortedarr = tree_to_sortedArray_withInorder(BSTtree);
    BST_tree sortedtre = generate_BST_tree_from_array(sortedarr);
    cout << "\n";
    cout << "Sorted array height:" << getHeight(sortedtre.get_root()) << "\n";

    
    /*
    cout << "==============================================================" << "\n";
    Node* deepestNode = bfs_findDeepestNode(BSTtree.get_root());
    publisher deepestPub = deepestNode->key;
    cout << deepestPub.name << "\n";
    */


	return EXIT_SUCCESS;
}





/////////////////// Node ///////////////////

Node::Node(publisher key){
	this->key = key;
	this->parent = NULL;
	this->left = NULL;
	this->right = NULL;
}


Node* BST_tree::get_root(){

    // Fill this function.

    return this->root;
}

//int ii = -1;
Node* BST_tree::BST_insert(Node* root, Node* ptr){

    // Fill this function.
    

    if(root == nullptr){
        return ptr;
    }

    if(ptr->key.name < root->key.name){
        if(root->left == nullptr){
            root->left = ptr;
            ptr->parent = root;
            //cout << "değil ::: " << ii <<"\n";
            //ii++;
            
        }
        else{
            BST_insert(root->left, ptr);
        }
    }
    if(ptr->key.name > root->key.name){
        if(root->right == nullptr){
            root->right = ptr;
            ptr->parent = root;
            //cout << "burasiiii "  << ii << "\n";
            //ii++;
            
        }
        else{
            BST_insert(root->right, ptr);
        }
    }
    if(ptr->key.name == root->key.name){
        root->key.eu_sales += ptr->key.eu_sales;
        root->key.na_sales += ptr->key.na_sales;
        root->key.others_sales += ptr->key.others_sales;
        //ii++;
    }


	return root;
}


void BST_tree::insertValue(vector<string> n) {

    // Fill this function.

    publisher publisher;
    publisher.name = n[3];
    publisher.na_sales = stof(n[4]);
    publisher.eu_sales = stof(n[5]);
    publisher.others_sales = stof(n[6]);

    Node* newNode = new Node(publisher);
    this->root = BST_insert(this->root, newNode);
}

void BST_tree::find_best_seller(){

    // Fill this function.
    if (!root) return;

    stack<Node*> s;
    s.push(root);

    publisher* best_na = nullptr;
    publisher* best_eu = nullptr;
    publisher* best_others = nullptr;

    //  pre-order traversal 
    while (!s.empty()) {
        
        Node* current = s.top();
        s.pop();

        // best of each market
        if (!best_na || current->key.na_sales > best_na->na_sales){
            best_na = &current->key;
        }
        if (!best_eu || current->key.eu_sales > best_eu->eu_sales){
            best_eu = &current->key;
        }
        if (!best_others || current->key.others_sales > best_others->others_sales){
            best_others = &current->key;
        }

        // left processed firstly
        if (current->right) {
            s.push(current->right);
        }
        if (current->left) {
            s.push(current->left);
        }
    }

    best_seller[0] = best_na;
    best_seller[1] = best_eu;
    best_seller[2] = best_others;
}


BST_tree::BST_tree(){
	this->root = NULL;
    this->best_seller[0] = NULL;
    this->best_seller[1] = NULL;
    this->best_seller[2] = NULL;
}

BST_tree::~BST_tree(){
}
