#include "tweet.h"

void bubbleSort(std::vector<Tweet>& tweets, const std::string& sortBy, bool ascending) 
{
    int n = tweets.size();
    bool swap = false;

    for(int i = 0; i < n; i++){
        swap = false;
        for(int j = 0; j < n - 1 - i; j++){
            
            int general = 0;
            if(sortBy == "tweetID"){
                if(tweets[j].tweetID > tweets[j+1].tweetID){
                    general = 1;
                }
            }
            if(sortBy == "retweetCount"){
                if(tweets[j].retweetCount > tweets[j+1].retweetCount){
                    general = 1;
                }
            }
            if(sortBy == "favoriteCount"){
                if(tweets[j].favoriteCount > tweets[j+1].favoriteCount){
                    general = 1;
                }
            }
            //

            if(ascending){
                if(general){
                    auto temp = tweets[j];
                    tweets[j] = tweets[j+1];
                    tweets[j+1] = temp; 
                    swap = true;
                }
            }
            else{
                if(!general){
                    auto temp = tweets[j+1];
                    tweets[j+1] = tweets[j];
                    tweets[j] = temp; 
                    swap = true;
                }
            }
        }
        if(!swap){
            break;
        }
    }
}

void insertionSort(std::vector<Tweet>& tweets, const std::string& sortBy, bool ascending) 
{
    int n = tweets.size();

    for(int i = 1; i < n; i++){
        int j = i-1;
        auto temp = tweets[i];

        bool general;
        while(0 <= j){
            if (sortBy == "tweetID") {
                general = (temp.tweetID < tweets[j].tweetID);
            } 
            else if (sortBy == "retweetCount") {
                general = (temp.retweetCount < tweets[j].retweetCount);
            } 
            else if (sortBy == "favoriteCount") {
                general = (temp.favoriteCount < tweets[j].favoriteCount);
            }

            if(ascending){
                if(!general){
                    break;
                }
            }
            else{
                if(general){
                    break;
                }
            }

            tweets[j+1] = tweets[j];
            j--;   
        }
        tweets[j+1] = temp;
        
    }
}

void merge(std::vector<Tweet>& tweets, int left, int mid, int right, const std::string& sortBy, bool ascending) 
{

    std::vector<Tweet> temp(right - left + 1);
    for (int i = left; i <= right; ++i) {
        temp[i - left] = tweets[i];
    }

    int l = left, r = mid + 1, t = 0;

    while (l <= mid && r <= right) {
        bool general = false;
        if (sortBy == "tweetID") {
            general = (temp[l - left].tweetID <= temp[r - left].tweetID);
        } 
        else if (sortBy == "retweetCount") {
            general = (temp[l - left].retweetCount <= temp[r - left].retweetCount);
        } 
        else if (sortBy == "favoriteCount") {
            general = (temp[l - left].favoriteCount <= temp[r - left].favoriteCount);
        }

        if (ascending) {
            if(general) {
                tweets[left + t++] = temp[l++ - left]; 
            } 
            else {
                tweets[left + t++] = temp[r++ - left];  
            }
        } 
        else{
            if(!general) {
                tweets[left + t++] = temp[l++ - left];  
            } 
            else {
                tweets[left + t++] = temp[r++ - left];
            }
        }
    }

    while (l <= mid) {
        tweets[left + t++] = temp[l++ - left];
    }

    while (r <= right) {
        tweets[left + t++] = temp[r++ - left];
    }
    
}

void mergeSort(std::vector<Tweet>& tweets, int left, int right, const std::string& sortBy, bool ascending) 
{
    if(left == right) return;

    int mid = left + (right-left)/2;

    mergeSort(tweets, left, mid, sortBy, ascending);
    mergeSort(tweets, mid+1, right, sortBy, ascending);
    merge(tweets, left, mid, right,sortBy, ascending);
}