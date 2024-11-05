#include "tweet.h"

int select(Tweet tweet, std::string sortBy){
    int general = 0;
    if(sortBy == "tweetID"){
        general = (tweet.tweetID);
    }
    if(sortBy == "retweetCount"){
        general = (tweet.retweetCount);
    }
    if(sortBy == "favoriteCount"){
        general = (tweet.favoriteCount);
    }
    return general;
}

int binarySearch(const std::vector<Tweet>& tweets, long long key, const std::string& sortBy) 
{
    bool ascending = (select(tweets[0], sortBy) < select(tweets[tweets.size() - 1], sortBy));
    
    int left = 0, right = tweets.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        long long num = select(tweets[mid], sortBy); 

        if (num == key) {
            return mid; 
        }

        else if(ascending){
            if (num < key){
                left = mid + 1;
            } 
            else{
                right = mid - 1;
            }
        } 
        else {  
            if(num > key){
                left = mid + 1;
            } 
            else{
                right = mid - 1;
            }
        }
    }

    // If key not found, return the position where it would be inserted
    return ascending ? left : right;
}


int countAboveThreshold(const std::vector<Tweet>& tweets, const std::string& metric, int threshold) 
{
    // is those vlaeus given sorted already, i can find result in O(n) time
    // (or I can sort them but !!! vector is given const format so I cant sort it, also if I sort them here complexity will be O(n*log(n)))
    // mergeSort(tweets, 0, tweets.size()-1, metric, true); 
    /*
    bool ascending = (select(tweets[0], metric) < select(tweets[tweets.size()-1], metric));

    int idx = binarySearch(tweets, threshold, metric);
    int res = 0;

    if(ascending){
        
        while(idx < tweets.size() && select(tweets[idx], metric) == select(tweets[idx+1], metric)){
            idx++; // threshold val does not counted
        }
        
        return tweets.size() - (idx+1);
    }
    else{
        while(idx >= 0 && select(tweets[idx], metric) == select(tweets[idx-1], metric)){
            idx--;
        }
        return idx;
    }
    */
    // but I dont know anything about values, then I implement O(n) solution
    int res = 0;
    if(metric == "tweetId"){
        for(int i = 0; i < tweets.size(); i++){
            if(tweets[i].tweetID > threshold){
                res++;
            }
        }
    }
    else if(metric == "favoriteCount"){
        for(int i = 0; i < tweets.size(); i++){
            if(tweets[i].favoriteCount > threshold){
                res++;
            }
        }
    }
    else{
        for(int i = 0; i < tweets.size(); i++){
            if(tweets[i].retweetCount > threshold){
                res++;
            }
        }
    }

    return res;
}