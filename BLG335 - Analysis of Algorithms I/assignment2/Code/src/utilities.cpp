#include "item.h"

std::vector<Item> readItemsFromFile(const std::string& filename) 
{
    std::vector<Item> items;

    std::fstream f;
    f.open(filename);

    std::string title;
    getline(f, title);

    std::string line;
    while(getline(f,line)){
        std::stringstream s(line);
        std::string itemAge_str, itemType_str, itemOrigin_str, itemRarity_str;

        if (getline(s, itemAge_str, ',') && getline(s, itemType_str, ',') && getline(s, itemOrigin_str, ',') && getline(s, itemRarity_str, ',')){
            int age = stoi(itemAge_str);
            int type = stoi(itemType_str);
            int origin = stoi(itemOrigin_str);
            double rarity = stod(itemRarity_str);

            Item item;
            item.age = age;
            item.type = type;
            item.origin = origin;
            item.rarityScore = rarity;
            items.push_back(item);
        }
    }

    return items;
}

void writeItemsToFile(const std::string& filename, const std::vector<Item>& items) 
{
    std::fstream f;
    f.open(filename);

    f << "age" << "," << "type" << "," << "origin" << "rarity" << "\n"; 

    for(auto item : items){
        f << item.age << "," << item.type << "," << item.origin << "," << item.rarityScore <<  "\n";
    }
}
