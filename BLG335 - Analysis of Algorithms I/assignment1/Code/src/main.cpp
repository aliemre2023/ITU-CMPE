#include "tweet.h"
#include <cassert>
#include <algorithm>
#include <iostream>
#include <vector>
#include <chrono>

/*
struct Tweet {
    long long tweetID;
    int retweetCount;
    int favoriteCount;     


    // addditional, for comparation
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
};
*/

bool customTweetComparator(const Tweet& t1, const Tweet& t2, const std::string& sortBy, bool ascending) {
    if (sortBy == "retweetCount") {
        return ascending ? t1.retweetCount < t2.retweetCount : t1.retweetCount > t2.retweetCount;
    } 
    else if (sortBy == "favoriteCount") {
        return ascending ? t1.favoriteCount < t2.favoriteCount : t1.favoriteCount > t2.favoriteCount;
    } 
    else if (sortBy == "tweetID") {
        return ascending ? t1.tweetID < t2.tweetID : t1.tweetID > t2.tweetID;
    } 
    return ascending ? t1.tweetID < t2.tweetID : t1.tweetID > t2.tweetID;
}

void stableSortWithCustomComparator(std::vector<Tweet>& tweets, const std::string& sortBy, bool ascending) {
    std::stable_sort(tweets.begin(), tweets.end(), 
        [&sortBy, ascending](const Tweet& t1, const Tweet& t2) {
            return customTweetComparator(t1, t2, sortBy, ascending);
        });
}

int main() 
{
    // Validation
    std::vector<Tweet> arr = readTweetsFromFile("data/sizes/tweets5K.csv");
    std::vector<Tweet> arr_bubbleSort = readTweetsFromFile("data/sizes/tweets5K.csv");
    std::vector<Tweet> arr_insertionSort = readTweetsFromFile("data/sizes/tweets5K.csv");
    std::vector<Tweet> arr_mergeSort = readTweetsFromFile("data/sizes/tweets5K.csv");
    
    std::string by = "retweetCount";
    bool to = true;

    stableSortWithCustomComparator(arr, by, to);
    bubbleSort(arr_bubbleSort, by, to);
    insertionSort(arr_insertionSort, by, to);
    mergeSort(arr_mergeSort, 0, arr_mergeSort.size()-1, by, to);

    std::cout << "\nPRINT:\n";
    printf("0: TweetID: %lld, RetweetCount: %d, FavoriteCount: %d\n",
            arr[1000].tweetID, arr[1000].retweetCount, arr[1000].favoriteCount);
    printf("1: TweetID: %lld, RetweetCount: %d, FavoriteCount: %d\n",
            arr_bubbleSort[1000].tweetID, arr_bubbleSort[1000].retweetCount, arr_bubbleSort[1000].favoriteCount);
    printf("2: TweetID: %lld, RetweetCount: %d, FavoriteCount: %d\n",
            arr_insertionSort[1000].tweetID, arr_insertionSort[1000].retweetCount, arr_insertionSort[1000].favoriteCount);
    printf("3: TweetID: %lld, RetweetCount: %d, FavoriteCount: %d\n",
            arr_mergeSort[1000].tweetID, arr_mergeSort[1000].retweetCount, arr_mergeSort[1000].favoriteCount);

    assert(arr == arr_insertionSort);
    assert(arr == arr_bubbleSort);
    assert(arr == arr_mergeSort);

    
    /*
    // Time Checker, for Sorting Algorithm
    std::vector<std::string> csv_list = {
                "data/permutations/tweets.csv",
                "data/permutations/tweetsNS.csv",
                "data/permutations/tweetsSA.csv",
                "data/permutations/tweetsSD.csv",
                "data/sizes/tweets5K.csv",
                "data/sizes/tweets10K.csv",
                "data/sizes/tweets20K.csv",
                "data/sizes/tweets30K.csv",
                "data/sizes/tweets40K.csv",
                "data/sizes/tweets50K.csv"};

        
    
    for (const auto& csv : csv_list) {
        // Read tweets from the current CSV file
        std::vector<Tweet> arr_bubbleSort = readTweetsFromFile(csv);
        std::vector<Tweet> arr_insertionSort = readTweetsFromFile(csv);
        std::vector<Tweet> arr_mergeSort = readTweetsFromFile(csv);

        // Print the dataset name
        std::cout << "\nDataset: " << csv << "\n";

        // Measure time for Bubble Sort
        auto start_bubble = std::chrono::high_resolution_clock::now();
        bubbleSort(arr_bubbleSort, by, to);
        auto end_bubble = std::chrono::high_resolution_clock::now();
        auto bubble_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_bubble - start_bubble);
        std::cout << "BubbleSort took: " << bubble_duration.count() << " ms\n";

        // Measure time for Insertion Sort
        auto start_insertion = std::chrono::high_resolution_clock::now();
        insertionSort(arr_insertionSort, by, to);
        auto end_insertion = std::chrono::high_resolution_clock::now();
        auto insertion_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_insertion - start_insertion);
        std::cout << "InsertionSort took: " << insertion_duration.count() << " ms\n";

        // Measure time for Merge Sort
        auto start_merge = std::chrono::high_resolution_clock::now();
        mergeSort(arr_mergeSort, 0, arr_mergeSort.size() - 1, by, to);
        auto end_merge = std::chrono::high_resolution_clock::now();
        auto merge_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_merge - start_merge);
        std::cout << "MergeSort took: " << merge_duration.count() << " ms\n";
    }
    */
    
    
    /*
    // Time Checker
    std::vector<std::string> csv_list = {
                "data/sizes/tweets5K.csv",
                "data/sizes/tweets10K.csv",
                "data/sizes/tweets20K.csv",
                "data/sizes/tweets30K.csv",
                "data/sizes/tweets40K.csv",
                "data/sizes/tweets50K.csv"};
    
    for (const auto& csv : csv_list) {
        std::cout << "\nDataset: " << csv << "\n";
        std::cout << "\n";

        // BinarySearch Control
        std::cout << "\binarySearch Control\n";
        std::vector<Tweet> arrf = readTweetsFromFile(csv);

        stableSortWithCustomComparator(arrf, "tweetID", true);

        std::cout << "O(n^2): ";
        for (int i = 0; i < arrf.size(); i++) {
            if (arrf[i].tweetID == 250) {
                std::cout << i << " ";
            }
        }
        std::cout << "\n";

        auto builtin_bs = std::chrono::high_resolution_clock::now();
        std::vector<int> firstColumn;
        for (const auto& row : arrf) {
            firstColumn.push_back(row.tweetID);
        }
        auto it = std::lower_bound(firstColumn.begin(), firstColumn.end(), 1773335);
        std::cout << "builtin bs: " << (it - firstColumn.begin()) << std::endl;
        auto builtin_bs_end = std::chrono::high_resolution_clock::now();
        auto builtin_bs_duration = std::chrono::duration_cast<std::chrono::milliseconds>(builtin_bs_end - builtin_bs);
        std::cout << "builtin bs time: " << builtin_bs_duration.count() << " ms" << std::endl;

        auto start_bs = std::chrono::high_resolution_clock::now();
        int index = binarySearch(arrf, 1773335, "tweetID");
        std::cout << "Index: " << index << "\n";
        auto end_bs = std::chrono::high_resolution_clock::now();
        auto bs_duration = std::chrono::duration<double, std::milli>(end_bs - start_bs);
        std::cout << "BinarySearch took: " << bs_duration.count() << " ms\n";

        // Threshold Control
        std::cout << "\nThreshold Control\n";
        std::vector<Tweet> arrg = readTweetsFromFile(csv);
        stableSortWithCustomComparator(arrg, "favoriteCount", true);

        std::cout << "O(n^2): ";
        for (int i = 0; i < arrg.size(); i++) {
            if (arrg[i].favoriteCount == 250) {
                std::cout << arrg.size() - i;
                break;
            }
        }
        std::cout << "\n";

        std::vector<int> firstColumnCount;
        for (const auto& row : arrg) {
            firstColumnCount.push_back(row.favoriteCount);
        }
        auto itCount = std::lower_bound(firstColumnCount.begin(), firstColumnCount.end(), 250);
        std::cout << "builtin bs: " << firstColumnCount.size() - (itCount - firstColumnCount.begin()) << std::endl;

        auto start_cat = std::chrono::high_resolution_clock::now();
        int count = countAboveThreshold(arrg, "favoriteCount", 250);
        std::cout << "Count: " << count << "\n";
        auto end_cat = std::chrono::high_resolution_clock::now();
        auto cat_duration = std::chrono::duration<double, std::milli>(end_cat - start_cat);
        std::cout << "CountAboveThreshold took: " << cat_duration.count() << " ms\n";
    }*/
    
    
    

    return 0;
}