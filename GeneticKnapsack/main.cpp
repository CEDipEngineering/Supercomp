#include <iostream>
#include <vector>
#include <random>

#define POP_SIZE 16

struct Item {
    int id;
    int weight;
    int value;
};

const int CAPACITY = 100;

int main(){

    // Random Number Generators
    std::default_random_engine generator(1337);
    std::uniform_int_distribution<int> weight_distro(1,15);
    std::uniform_int_distribution<int> value_distro(1,100);
    std::uniform_real_distribution<double> coin_distro(0,1);
    

    // Make item samples
    const int item_amount = 40;
    std::cout << "Item List:" << std::endl;
    std::cout << "ID\tWEIGHT\tVALUE" << std::endl;
    std::vector<Item> item_list;
    for(int i=0; i<item_amount; i++){
        item_list.push_back({i, weight_distro(generator), value_distro(generator)});
        std::cout << item_list[i].id << "\t" << item_list[i].weight << "\t" << item_list[i].value << std::endl; 
    }

    std::cout << "===========================================================================================" << std::endl;
    // Create initial population
    bool population[POP_SIZE][item_amount];
    for (int i = 0; i < POP_SIZE; i++){
        for (int j = 0; j < item_amount; j++){
            population[i][j] = coin_distro(generator) > 0.5;
        }
    }
    std::cout << "Initial Population (" << POP_SIZE << " members x " << item_amount << " items): " << std::endl;
    for (int i = 0; i < POP_SIZE; i++){
        for (int j = 0; j < item_amount; j++){
            std::cout << (int) population[i][j] << " ";
        }   
        std::cout << std::endl;
    }
    std::cout << "===========================================================================================" << std::endl;

    // Run Experiment
        // Loop through generations
        // 1. Calculate fitness
        // 2. Select fittest members as parents
        // 3. Crossover parents
        // 4. Apply random mutations to offspring
        // 5. Rebuild population by using parents and mutants (which contain all offspring including non-mutated ones)
        // 6. Go to step 1

    


    // for(int i = 0; i<10; i++){
    //     std::cout << weight_distro(generator) << std::endl;
    //     std::cout << value_distro(generator) << std::endl;
    // }

    return 0;
}