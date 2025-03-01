#include <iostream>
#include <vector>
#include <stack>

using namespace std;

int woman_choice(vector<pair<vector<int>, int>> womans, int woman_idx, int man_idx){
    for(int i = 0; i < womans[woman_idx].first.size(); i++){
        if(womans[woman_idx].first[i] == man_idx){
            return i;
        }
    }
    // if man is not in the woman's list
    return INT_MAX;
}

void gale_shapley(vector<pair<vector<int>, int>>& mans, vector<pair<vector<int>, int>>& womans){
    stack<pair<int, int>> q; // man_idx, try_count
    for(int i = 0; i < mans.size(); i++){
        q.push({i, 0});
    }

    while(!q.empty()){
        pair<int, int> man_info = q.top();
        q.pop();
        int man = man_info.first;

        int man_choice = mans[man].first[man_info.second];

        if(womans[man_choice].second == -1){
            cout << "Wanted relation: " << man << " -> " << man_choice << "\n";

            mans[man].second = man_choice;
            womans[man_choice].second = man;
        }
        else if(womans[man_choice].second > woman_choice(womans, man_choice, man)){
            cout << "Better relation: " << man << " -> " << man_choice << "\n";

            q.push({womans[man_choice].second, 0}); // reassign divorced man

            mans[man].second = man_choice;
            womans[man_choice].second = man;
        }
        else{
            cout << "Unwanted man: " << man << " -> " << man_choice << "\n";

            q.push({man, ++man_info.second}); // reassign unwanted man
            continue;
        }
    }
}

void print(vector<pair<vector<int>, int>> mans){
    cout << "Relations:" << "\n";
    for(int i = 0; i < mans.size(); i++){
        cout << i << " -> " << mans[i].second << "\n";
    }
}

int main(){
    // wanted order, fionce
    vector<pair<vector<int>, int>> mans = {
        {{2, 1, 0}, -1},
        {{2, 0, 1}, -1},
        {{0, 1, 2}, -1}
    };
    vector<pair<vector<int>, int>> womans = {
        {{1, 0, 2}, -1},
        {{0, 1, 2}, -1},
        {{0, 1, 2}, -1}
    };

    // Propose-and-reject algorithm. [Gale-Shapley 1962]
    gale_shapley(mans, womans);
    
    print(mans);

    return 0;
}