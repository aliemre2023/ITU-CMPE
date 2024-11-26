#include "methods.h"

std::vector<Item> countingSort(std::vector<Item>& items, const std::string& attribute, bool ascending)
{
    int max_age = getMax(items, "age");

    // defauly item for initalization
    Item def_item;
    def_item.age = 0;
    def_item.type = 0;
    def_item.origin = 0;
    def_item.rarityScore = 0.0;
    std::vector<Item> sorted;
    sorted.resize(items.size(), def_item);

    std::vector<int> C;

    // number counts, posssible index
    if(attribute == "age"){
        C.resize(max_age+1, 0);
        for(auto item : items){
            C[item.age]++;
        }
    }
    if(attribute == "type"){
        C.resize(4+1, 0);
        for(auto item : items){
            C[item.type]++;
        }
    }
    if(attribute == "origin"){
        C.resize(6+1, 0);
        for(auto item : items){
            C[item.origin]++;
        }
    }

    // prefix
    for(int i = 1; i < C.size(); i++){
        C[i] += C[i-1];
    }

    // sorting
    for(int i = items.size()-1; i >= 0; i--){
        Item item = items[i];

        int possible_idx;
        if(attribute == "age"){
            possible_idx = C[item.age] - 1;
        }
        if(attribute == "type"){
            possible_idx = C[item.type] - 1;
        }
        if(attribute == "origin"){
            possible_idx = C[item.origin] - 1;
        }
        
        sorted[possible_idx] = item;
        C[item.age]--;
    }

    // descending
    if(!ascending){
        for(int i = 0; i < sorted.size()/2; i++){
            std::swap(sorted[i], sorted[sorted.size()-1-i]);
        }
    }

    return sorted;
}

// Function to heapify a subtree rooted with node i in the array of items
void heapify(std::vector<Item>& items, int n, int i, bool descending)
{
    int parent = i;

    int left = 2*i;
    int right = 2*i+1;

    if(descending){
        if(left < n && items[left].rarityScore < items[parent].rarityScore){
            parent = left;
        }
        if(right < n && items[right].rarityScore < items[parent].rarityScore){
            parent = right;
        }

        // if not placed true place
        if(parent != i){
            std::swap(items[i], items[parent]);
            heapify(items, n, parent, descending);
        }
    }
    else{
        if(left < n && items[left].rarityScore > items[parent].rarityScore){
            parent = left;
        }
        if(right < n && items[right].rarityScore > items[parent].rarityScore){
            parent = right;
        }

        // if not placed true place
        if(parent != i){
            std::swap(items[i], items[parent]);
            heapify(items, n, parent, descending);
        }
    }

}

// Function to perform heap sort on rarityScore scores of items
std::vector<Item> heapSortByRarity(std::vector<Item>& items, bool descending)
{
    Item zeroth;
    zeroth.age = 0;
    zeroth.type = 0;
    zeroth.origin = 0;
    zeroth.rarityScore = 0.0;
    items.insert(items.begin(), zeroth);

    int n = items.size();

    // building loop
    for(int i = n/2; i > 0; i--){
        heapify(items, n, i, descending);
    }

    // throwing loop
    for(int i = n-1; i> 0; i--){
        std::swap(items[1], items[i]);
        heapify(items, i, 1, descending);
    }
    items.erase(items.begin());

    return items;
}
