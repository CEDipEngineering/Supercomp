#include <iostream>
#include<vector>
#include<algorithm>
#include<random>

struct Item {
    int id;
    int weight;
    int value;
};

struct Knapsack {
    int id;
    int weight;
    std::vector<bool> arr;
};

void print_knapsack(Knapsack k){
    std::cout << "Knapsack " << k.id << std::endl;
    std::cout << "Weight: " << k.weight << std::endl;
    for(int i = 0; i<(int)k.arr.size(); i++){
        std::cout << k.arr[i] << " ";
    }
    std::cout << std::endl;
}

void print_knapsack(Knapsack k, std::vector<Item> items){
    std::cout << "Knapsack " << k.id << std::endl;
    std::cout << "Weight: " << k.weight << std::endl;
    int totalValue = 0;
    for(int i = 0; i<(int)k.arr.size(); i++){
        std::cout << k.arr[i] << " ";
        totalValue += k.arr[i]*items[i].value;
    }
    std::cout << std::endl << "Total value: " << totalValue << std::endl;
}

int main(){
    // Read number of items and capacity of knapsacks
    int n = 0;
    int W = 0;
    std::cin >> n >> W;

    // Read all items
    std::vector<Item> items;
    items.reserve(n);
    int w,v;
    for(int i = 0; i<n; i++){
        std::cin >> w >> v;
        items.push_back({i, w, v});
    }

    // Algorithm
        Knapsack bag_light, bag_valuable;
        bag_light = {0,0,{}};
        bag_valuable = {1,0,{}};
        bag_light.arr.reserve(n);
        bag_valuable.arr.reserve(n);
        
        // Most expensive
        std::sort(items.begin(), items.end(), [](Item a, Item b){return a.value > b.value;});
        for (auto& el : items){
            if(el.weight + bag_valuable.weight <= W){
                bag_valuable.weight += el.weight;
                bag_valuable.arr.push_back(true);
            } else {
                bag_valuable.arr.push_back(false);
            }
        }

        // Lightest
        std::sort(items.begin(), items.end(), [](Item a, Item b){return a.weight < b.weight;});
        for (auto& el : items){
            if(el.weight + bag_light.weight <= W){
                bag_light.weight += el.weight;
                bag_light.arr.push_back(true);
            } else {
                bag_light.arr.push_back(false);
            }
        }


    // Print output
    std::cout << "Light Backpack" << std::endl;
    print_knapsack(bag_light, items);
    std::cout << "Valuable Backpack" << std::endl;
    print_knapsack(bag_valuable, items);
    
    return 0;
}