#include "methods.h"

#include <vector>
#include <algorithm>
#include <iostream>
#include <chrono>

struct CompareRarityScore1 {
    bool operator()(const Item& a, const Item& b) {
        return a.rarityScore < b.rarityScore; // Max-heap based on rarityScore
    }
};
struct CompareRarityScore2 {
    bool operator()(const Item& a, const Item& b) {
        return a.age < b.age; // Max-heap based on age
    }
};

int main() 
{
    std::cout << "--MAIN--" << "\n";
    
    // countingsort test
    // Validaty
    /* 
    std::vector<Item> items = readItemsFromFile("data/test/items_rarity_calculated.txt");
    std::vector<Item> items2 = readItemsFromFile("data/test/items_rarity_calculated.txt");

    items = countingSort(items, "empty_attribute", true);
    std::sort(items2.begin(), items2.end(), CompareRarityScore2());

    for (int i = 0; i < items.size(); i++) {
        if(items2[i].age != items[i].age)
            std::cout << items2[i].age << " - " << items[i].age << "\n";
    }
    */
    // Analyze
    /*
    std::vector<Item> items_l = readItemsFromFile("data/items_l.csv");
    std::vector<Item> items_m = readItemsFromFile("data/items_m.csv");
    std::vector<Item> items_s = readItemsFromFile("data/items_s.csv");

    auto start_l = std::chrono::high_resolution_clock::now();
    countingSort(items_l, "age", true);
    auto stop_l = std::chrono::high_resolution_clock::now();
    auto duration_l = std::chrono::duration_cast<std::chrono::microseconds>(stop_l - start_l);
    printf("Large countingSort: %ld\n", duration_l.count());

    auto start_m = std::chrono::high_resolution_clock::now();
    countingSort(items_m, "age", true);
    auto stop_m = std::chrono::high_resolution_clock::now();
    auto duration_m = std::chrono::duration_cast<std::chrono::microseconds>(stop_m - start_m);
    printf("Medium countingSort: %ld\n", duration_m.count());
    
    auto start_s = std::chrono::high_resolution_clock::now();
    countingSort(items_s, "age", true);
    auto stop_s = std::chrono::high_resolution_clock::now();
    auto duration_s = std::chrono::duration_cast<std::chrono::microseconds>(stop_s - start_s);
    printf("Small countingSort: %ld\n", duration_s.count());
    */

    


    // Rarity Calculation 
    /* Analyze
    std::vector<Item> items_l = readItemsFromFile("data/items_l.csv");
    std::vector<Item> items_m = readItemsFromFile("data/items_m.csv");
    std::vector<Item> items_s = readItemsFromFile("data/items_s.csv");

    auto start_l = std::chrono::high_resolution_clock::now();
    calculateRarityScores(items_l, 500);
    auto stop_l = std::chrono::high_resolution_clock::now();
    auto duration_l = std::chrono::duration_cast<std::chrono::microseconds>(stop_l - start_l);
    printf("Large rarityCalc: %ld\n", duration_l.count());

    auto start_m = std::chrono::high_resolution_clock::now();
    calculateRarityScores(items_m, 500);
    auto stop_m = std::chrono::high_resolution_clock::now();
    auto duration_m = std::chrono::duration_cast<std::chrono::microseconds>(stop_m - start_m);
    printf("Medium rarityCalc: %ld\n", duration_m.count());
    
    auto start_s = std::chrono::high_resolution_clock::now();
    calculateRarityScores(items_s, 500);
    auto stop_s = std::chrono::high_resolution_clock::now();
    auto duration_s = std::chrono::duration_cast<std::chrono::microseconds>(stop_s - start_s);
    printf("Small rarityCalc: %ld\n", duration_s.count());
    */
    
    // heapsort test
    /* Valide
    std::vector<Item> items = readItemsFromFile("data/test/items_rarity_calculated.txt");
    std::vector<Item> items2 = readItemsFromFile("data/test/items_rarity_calculated.txt");

    heapSortByRarity(items, false);
    std::sort(items2.begin(), items2.end(), CompareRarityScore1());

    for (int i = 0; i < items.size(); i++) {
        if(items2[i].rarityScore != items[i].rarityScore)
            std::cout << items2[i].rarityScore << " - " << items[i].rarityScore << "\n";
    }
    */
   
    // Analyze
    /*    
    std::vector<Item> items_x = readItemsFromFile("data/test/items_rarity_calculated.txt");
    std::vector<Item> items_y = readItemsFromFile("data/test/items_rarity_sorted.txt");
    std::vector<Item> items_t = readItemsFromFile("data/test/items_rarity_calculated.txt");
    std::vector<Item> items_k = readItemsFromFile("data/test/items_rarity_sorted.txt");

    auto start_lx = std::chrono::high_resolution_clock::now();
    countingSort(items_t, "age", true);
    auto stop_lx = std::chrono::high_resolution_clock::now();
    auto duration_lx = std::chrono::duration_cast<std::chrono::microseconds>(stop_lx - start_lx);
    printf("ns countingSort: %ld\n", duration_lx.count());

    auto start_l = std::chrono::high_resolution_clock::now();
    heapSortByRarity(items_x, false);
    auto stop_l = std::chrono::high_resolution_clock::now();
    auto duration_l = std::chrono::duration_cast<std::chrono::microseconds>(stop_l - start_l);
    printf("ns heapS: %ld\n", duration_l.count());

    auto start_lk = std::chrono::high_resolution_clock::now();
    countingSort(items_k, "age", true);
    auto stop_lk = std::chrono::high_resolution_clock::now();
    auto duration_lk = std::chrono::duration_cast<std::chrono::microseconds>(stop_lk - start_lk);
    printf("ns countingSort: %ld\n", duration_lk.count());

    auto start_m = std::chrono::high_resolution_clock::now();
    heapSortByRarity(items_y, false);
    auto stop_m = std::chrono::high_resolution_clock::now();
    auto duration_m = std::chrono::duration_cast<std::chrono::microseconds>(stop_m - start_m);
    printf("s heapS: %ld\n", duration_m.count());
    */

    return 0;
}