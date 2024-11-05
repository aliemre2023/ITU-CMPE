#ifndef TWEET_H
#define TWEET_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream> 
#include <sstream>
#include <ctime> 


struct Tweet {
    long long tweetID;
    int retweetCount;
    int favoriteCount;     

    
    // addditional, for comparation
    /*
    bool operator==(const Tweet& other) const {
        return tweetID == other.tweetID && 
            retweetCount == other.retweetCount && 
            favoriteCount == other.favoriteCount;
    }

    bool operator<(const Tweet& other) const {
        if (tweetID != other.tweetID) {
            return tweetID < other.tweetID;
        }
        if (retweetCount != other.retweetCount) {
            return retweetCount < other.retweetCount;
        }
        return favoriteCount < other.favoriteCount;
    }
    */
};

// Function prototype for counting tweets above threshold
int countAboveThreshold(const std::vector<Tweet>& tweets, const std::string& metric, int threshold);
int binarySearch(const std::vector<Tweet>& tweets, long long key, const std::string& sortBy); 

// Function prototypes for sorting
void bubbleSort(std::vector<Tweet>& tweets, const std::string& sortBy, bool ascending);
void insertionSort(std::vector<Tweet>& tweets, const std::string& sortBy, bool ascending);
void mergeSort(std::vector<Tweet>& tweets, int left, int right, const std::string& sortBy, bool ascending);
void merge(std::vector<Tweet>& tweets, int left, int mid, int right, const std::string& sortBy, bool ascending);

// Function prototypes for utilities
std::vector<Tweet> readTweetsFromFile(const std::string& filename);
void writeTweetsToFile(const std::string& filename, const std::vector<Tweet>& tweets);

#endif
