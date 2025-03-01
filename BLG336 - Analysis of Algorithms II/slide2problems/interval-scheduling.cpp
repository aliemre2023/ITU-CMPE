#include <iostream>
#include <vector>

using namespace std;

bool compare_pair_second(pair<int,int>& a, pair<int,int>& b){
    return a.second < b.second;
}

int max_event_count(vector<pair<int,int>> schedule){
    // sort according to finish times
    sort(schedule.begin(), schedule.end(), compare_pair_second);

    int count = 0;
    int last_end = 0;

    for(int i = 0; i < schedule.size(); i++){
        if(schedule[i].first < last_end){
            continue;
        }

        last_end = schedule[i].second;
        count++;
        cout << "{" << schedule[i].first << "," << schedule[i].second << "}\n";

    }

    return count;
}

int main(){
    vector<pair<int,int>> schedule = {
        {0,6}, {1,4}, {3,5}, {3,8}, {4,7}, {5,9}, {6,10}, {8,11}
    };

    int count = max_event_count(schedule);

    cout << "Count: " << count << "\n";

    return 0;
}