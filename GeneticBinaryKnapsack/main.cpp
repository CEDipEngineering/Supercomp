#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <fstream>
#include <chrono>

#define POP_SIZE 256
#define HALF_POP POP_SIZE/2
#define CAPACITY 100
#define ITEM_AMOUNT 40
#define CROSSOVER_RATE 0.8f
#define MUTATION_RATE 0.1f 
#define GENERATIONS 200000
#define PRINT_GENERATIONS false

// Set to true to view debug outputs
// Warning this will print every generation's debug info (VERY VERBOSE)
// Recommended to set GENERATIONS to a low number, to facilitate visualization 
#define DEBUG false

struct Item {
    int id;
    int weight;
    int value;
};

struct Individual {
    int fitness;
    bool arr[ITEM_AMOUNT];
};

// Random Number Generators
std::default_random_engine generator(1337);
std::uniform_int_distribution<int> weight_distro(1,15);
std::uniform_int_distribution<int> value_distro(1,CAPACITY);
std::uniform_real_distribution<double> coin_distro(0,1);
std::uniform_int_distribution<int> item_distro(0,ITEM_AMOUNT-1);
std::uniform_int_distribution<int> parent_distro(0,HALF_POP-1);


int individual_fitness(Individual individual, std::vector<Item> item_list){
    int value = 0;
    int weight = 0;
    for (int i = 0; i<ITEM_AMOUNT; i++){
        value += individual.arr[i]*item_list[i].value;
        weight += individual.arr[i]*item_list[i].weight;
    }
    // std::cout << "Value: " << value << " Weight: " << weight << " ";
    if(weight > CAPACITY){
        return 0;
    } 
    return value;
}

Individual crossover(Individual *individual_a, Individual *individual_b){
    double roll = coin_distro(generator);
    // Split the diference between copying each parent
    Individual child;
    if (roll < (1.0-CROSSOVER_RATE)/2.0){
        std::copy(individual_a->arr, individual_a->arr+ITEM_AMOUNT, child.arr); 
    } else if (roll < (1.0-CROSSOVER_RATE)){
        std::copy(individual_b->arr, individual_b->arr+ITEM_AMOUNT, child.arr); 
    } else {
        // Random amount from each parent
        int index = item_distro(generator);
        int i = 0;
        for (i = 0; i<ITEM_AMOUNT; i++){
            if(i < index){
                child.arr[i] = individual_a->arr[i];
            } else {
                child.arr[i] = individual_b->arr[i];
            }
        }
    }
    child.fitness = 0; // Just to avoid garbage
    return child;
}

Individual mutate(Individual individual){
    double roll = 0.0;
    Individual mutant;
    for(int i = 0; i<ITEM_AMOUNT; i++){
        roll = coin_distro(generator);
        if (roll > MUTATION_RATE){
            mutant.arr[i] = individual.arr[i];
        } else {
            mutant.arr[i] = !individual.arr[i]; // Negate position
        }
    }
    return mutant;
}

bool compare_fitness(Individual a, Individual b){return (a.fitness > b.fitness);}

int main(){
    auto begin = std::chrono::high_resolution_clock::now();
    // Make item samples
    if(DEBUG){
        std::cout << "Item List:" << std::endl;
        std::cout << "ID\tWEIGHT\tVALUE" << std::endl;
    }
    std::vector<Item> item_list;
    for(int i=0; i<ITEM_AMOUNT; i++){
        item_list.push_back({i, weight_distro(generator), value_distro(generator)});
        if(DEBUG){
            std::cout << item_list[i].id << "\t" << item_list[i].weight << "\t" << item_list[i].value << std::endl; 
        }
    }

    std::ofstream item_file ("items.txt");
    item_file << ITEM_AMOUNT << " " << CAPACITY << std::endl;
    for(auto& el: item_list){
        item_file << el.weight << " " << el.value << std::endl;
    }
    item_file << std::endl;
    item_file.close();

    if(DEBUG){
        std::cout << "===========================================================================================" << std::endl;
    }
    
    std::vector<Individual> population;
    std::vector<Individual> parents;
    std::vector<Individual> best_history;
    population.reserve(POP_SIZE);
    parents.reserve(POP_SIZE);
    best_history.reserve(GENERATIONS);
    Individual t;
    int index1, index2;
    Individual child;
    Individual mutant;

    // Create initial population
    for (int i = 0; i < POP_SIZE; i++){
        population.push_back({0, {}});
        for (int j = 0; j < ITEM_AMOUNT; j++){
            population[i].arr[j] = coin_distro(generator) > 0.65; // Change to set chance of grabbing random items at start
        }
        population[i].fitness = 0;
    }
    // Initialize history
    for (int i = 0; i < GENERATIONS; i++){
        // best_history.push_back({0,{}});
        for (int j = 0; j < ITEM_AMOUNT; j++){
            best_history[i].arr[j] = false;
        }
        best_history[i].fitness = 0;
    }
    
    
    if(DEBUG){
        std::cout << "Initial Population (" << POP_SIZE << " members x " << ITEM_AMOUNT << " items): " << std::endl;
        for (int i = 0; i < POP_SIZE; i++){
            for (int j = 0; j < ITEM_AMOUNT; j++){
                std::cout << (int) population[i].arr[j] << " ";
            }   
            std::cout << std::endl;
        }
        std::cout << "===========================================================================================" << std::endl;
        std::cout << "FITNESS" << std::endl;
    }

    for(int gen = 0; gen<GENERATIONS; gen++){
        // Calculate fitness for whole population
        for (int i = 0; i < POP_SIZE; i++){
            population[i].fitness = individual_fitness(population[i], item_list);
        }
        std::sort(population.begin(), population.end(), compare_fitness);
        
        best_history.push_back(population[0]); 
        if(DEBUG){
            for (int i = 0; i < POP_SIZE; i++){
                for (int j = 0; j < ITEM_AMOUNT; j++){
                    std::cout << (int) population[i].arr[j] << " ";
                }   
                std::cout << "Fitness: " << population[i].fitness << std::endl;
            }

            std::cout << "===========================================================================================" << std::endl;
            std::cout << "PARENTS" << std::endl;
        }   
        
        // Separate best half of population as parents
        t.fitness = 0;
        for (int i = 0; i<HALF_POP; i++){
            std::copy(population[i].arr, population[i].arr + ITEM_AMOUNT, t.arr);
            t.fitness = population[i].fitness; // Just for debugging, not necessary
            parents.push_back(t);
        }

        if(DEBUG){
            for (int i = 0; i < HALF_POP; i++){
                for (int j = 0; j < ITEM_AMOUNT; j++){
                    std::cout << (int) parents[i].arr[j] << " ";
                }   
                std::cout << "Fitness: " << parents[i].fitness << std::endl;
            }

            std::cout << "===========================================================================================" << std::endl;
            std::cout << "CROSSOVER AND MUTATION" << std::endl;
        }

        // Crossover random parents to create other half of population
        for (int i=0; i<HALF_POP; i++){
            index1 = parent_distro(generator);
            index2 = parent_distro(generator);
            child = crossover(&(parents[index1]), &(parents[index2]));
            parents.push_back(child);
        }

        if(DEBUG){
            for (int i = HALF_POP; i < POP_SIZE; i++){
                for (int j = 0; j < ITEM_AMOUNT; j++){
                    std::cout << (int) parents[i].arr[j] << " ";
                }   
                std::cout << "Fitness: " << parents[i].fitness << std::endl;
            }

            std::cout << "MUTATIONS" << std::endl;
        }

        // Apply random mutations to children
        for (int i = HALF_POP; i<POP_SIZE; i++){
            mutant = mutate(parents[i]);
            for(int j = 0; j<ITEM_AMOUNT; j++){
                parents[i].arr[j] = mutant.arr[j];
            }
        }

        if(DEBUG){
            for (int i = HALF_POP; i < POP_SIZE; i++){
                for (int j = 0; j < ITEM_AMOUNT; j++){
                    std::cout << (int) parents[i].arr[j] << " ";
                }   
                std::cout << "Fitness: " << parents[i].fitness << std::endl;
            }
        }
        population.clear();
        // Set population from parents and children
        for (int i = 0; i < POP_SIZE; i++){
            population.push_back({0,{}});
            for (int j = 0; j < ITEM_AMOUNT; j++){
                population[i].arr[j] = parents[i].arr[j];
            }   
        }

        if(DEBUG){
            std::cout << "Population at end of generation" << std::endl;
            for (int i = 0; i < POP_SIZE; i++){
                for (int j = 0; j < ITEM_AMOUNT; j++){
                    std::cout << (int) population[i].arr[j] << " ";
                }   
                std::cout << "Fitness: " << population[i].fitness << std::endl;
            }
            std::cout << "=================================" << std::endl << "END OF GENERATION" << std::endl << "=================================" << std::endl;
        }
    }
    if(PRINT_GENERATIONS){
        std::cout << "Best Individuals for generations: " << std::endl;
        for(int gen = 0; gen<GENERATIONS; gen++){
            if(gen%10==0){
                std::cout << "Generation " << gen+1 << ":\t\t";
                for(int i = 0; i<ITEM_AMOUNT; i++){
                    std::cout << best_history[gen].arr[i] << " ";
                }
                std::cout << "Fitness: " << best_history[gen].fitness << std::endl;
            } else if (gen == GENERATIONS-1){
                std::cout << "==================================" << std::endl;
                std::cout << "Final Generation: " << std::endl;
                for(int i = 0; i<ITEM_AMOUNT; i++){
                    std::cout << best_history[gen].arr[i] << " ";
                }
                std::cout << "Fitness: " << best_history[gen].fitness << std::endl;
                std::cout << "==================================" << std::endl;
            }
        }
    }
    
    std::sort(best_history.begin(), best_history.end(), compare_fitness);
    std::cout << "Best knapsack in history: " << std::endl;
    std::cout << "Fitness: " << best_history[0].fitness << std::endl;
    for(int i = 0; i<ITEM_AMOUNT; i++){
        std::cout << (int) best_history[0].arr[i] << " ";
    }
    std::cout << std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    printf("Time measured: %.6f seconds.\n", elapsed.count() * 1e-9);
    return 0;
}