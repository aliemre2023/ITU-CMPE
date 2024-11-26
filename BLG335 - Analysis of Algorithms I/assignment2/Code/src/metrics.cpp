#include "methods.h"

int getMax(std::vector<Item>& items, const std::string& attribute)
{
    Item maxItem = items[0];
    int n = items.size();
    
    for(auto item : items){
      if(item.age > maxItem.age){
          maxItem = item;
      }
    }
    
    return maxItem.age;
}

// min = age - ageWindow
// max = age + ageWindow
// rarityScore = (1 - probability) * (1 + item.age/ageMax)

void calculateRarityScores(std::vector<Item>& items, int ageWindow) {
  int max_age = getMax(items, "age");

  for(int i = 0; i < items.size(); i++){
    Item item =  items[i];

    int totalCount = 0;
    int similarCount = 0;

    int mini_age = item.age - ageWindow; //std::max(0, item.age - ageWindow);
    int maxi_age = item.age + ageWindow; //std::min(max_age, item.age + ageWindow);

    int mini_idx = i-1;
    int maxi_idx = i;

    // in test.h, before rarity calculation there is countSort algorithm, then I able to make this optimization (2*ageWindow) instead of (n)
    while(mini_idx >= 0 && abs(item.age - items[mini_idx].age) <= ageWindow){
      Item candidate_item = items[mini_idx];
      if(candidate_item.age >= mini_age && candidate_item.age <= maxi_age){
        totalCount++;
        if(item.origin == candidate_item.origin && item.type == candidate_item.type){
          similarCount++;
        }
      }
      mini_idx--;
    }
    while(maxi_idx < items.size() && abs(item.age - items[maxi_idx].age) <= ageWindow){
      Item candidate_item = items[maxi_idx];
      if(candidate_item.age >= mini_age && candidate_item.age <= maxi_age){
        totalCount++;
        if(item.origin == candidate_item.origin && item.type == candidate_item.type){
          similarCount++;
        }
      }
      maxi_idx++;
    }

    /*
    for(int j = 0; j < items.size(); j++){
    //for(int j = std::max(0 ,i-ageWindow); j < std::min(int(items.size()), i+ageWindow+1); j++){
      Item candidate_item = items[j];
      if(candidate_item.age >= mini_age && candidate_item.age <= maxi_age){
        totalCount++;
        if(item.origin == candidate_item.origin && item.type == candidate_item.type){
          similarCount++;
        }
      }
    }
    */

    double probability = 0;
    if(totalCount > 0){
      probability = similarCount / (double)totalCount;
    }

    double rarity = (1 - probability) * (1 + (item.age / (double)max_age));
    items[i].rarityScore = rarity;
  }
}