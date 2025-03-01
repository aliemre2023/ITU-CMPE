#include <iostream>
#include <vector>

using namespace std;

bool compare_pair_pair_second(pair<pair<int, int>, int>& a, pair<pair<int, int>, int>& b){
    return a.first.second < b.first.second;
}

bool is_intercept(pair<int,int> first_interval, pair<int,int> second_interval){
    if(first_interval.second <= second_interval.first){
        return false;
    }
    return true;
}

int max_event_weigth(vector<pair<pair<int,int>, int>> schedule){
    // sort by ending times
    sort(schedule.begin(), schedule.end(), compare_pair_pair_second);

    // init dp
    vector<int> dp(schedule.size());
    for(int i = 0; i < schedule.size(); i++){
        dp[i] = schedule[i].second;
    }

    // n^2 approach
    for(int i = 0; i < dp.size(); i++){
        for(int j = 0; j < i; j++){
            if(is_intercept(schedule[j].first, schedule[i].first)){
                break;
            }
            dp[i] = max(dp[i], dp[j]+schedule[i].second);
        }
    }

    // find max in dp
    int max_weight = 0;
    for(int i = 0; i < dp.size(); i++){
        max_weight = max(max_weight, dp[i]);
    }

    return max_weight;
}

int main(){
    vector<pair<pair<int,int>, int>> schedule = {
        {{0,6}, 23}, {{1,4}, 12}, 
        {{3,5}, 20}, {{3,8}, 26}, 
        {{4,7}, 13}, {{5,9}, 20}, 
        {{6,10}, 11}, {{8,11}, 16}
    };

    int max_weight = max_event_weigth(schedule);

    cout << "Max Weight: " << max_weight << "\n";
}