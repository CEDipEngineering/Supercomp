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

int fitness(Knapsack k, std::vector<Item> items){
    int totalValue = 0;
    for(int i = 0; i<(int)k.arr.size(); i++){
        totalValue += k.arr[i]*items[i].value;
    }
    return totalValue;
}

Knapsack solve(Knapsack k, int capacity, std::vector<Item> items, int index){
    // print_knapsack(k,items);
    std::cout << index << std::endl;
    if(index == (int)items.size()){
        if(items[index].weight + k.weight < capacity){
            k.arr[index] = true;
            k.weight += items[index].weight;
            return k;
        } else {
            return k;
        }
    }
    Knapsack a, b;
    a = solve(k, capacity, items, index+1);
    if(items[index].weight + k.weight < capacity){
        k.arr[index] = true;
        k.weight += items[index].weight;
        b = solve(k, capacity-items[index].weight, items, index+1);
    } else {
        return a;
    }
    if(fitness(a, items)>fitness(b, items)){
        return a;
    }
    return b;
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
    Knapsack best = {0,0,{}};
    best.arr.reserve(n);
    for(int i = 0; i<n; i++){
        best.arr.push_back(false);
    }
    best = solve(best, W, items, 0);

    // Print output
    print_knapsack(best, items);
    
    return 0;
}