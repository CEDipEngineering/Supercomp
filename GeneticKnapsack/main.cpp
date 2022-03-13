#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

#define POP_SIZE 16
#define HALF_POP POP_SIZE/2
#define CAPACITY 100
#define ITEM_AMOUNT 40
#define CROSSOVER_RATE 0.8f
#define MUTATION_RATE 0.1f 
#define GENERATIONS 1000

#define DEBUG true

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
std::uniform_int_distribution<int> value_distro(1,100);
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

    if(DEBUG){
        std::cout << "===========================================================================================" << std::endl;
    }
    
    // Create initial population
    std::vector<Individual> population;
    population.reserve(POP_SIZE);
    for (int i = 0; i < POP_SIZE; i++){
        population.push_back({0, {}});
        for (int j = 0; j < ITEM_AMOUNT; j++){
            population[i].arr[j] = coin_distro(generator) > 0.65; // Change to set chance of grabbing random items at start
        }
        population[i].fitness = 0;
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

    for (int i = 0; i < POP_SIZE; i++){
        population[i].fitness = individual_fitness(population[i], item_list);
    }
    std::sort(population.begin(), population.end(), compare_fitness);
    
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

    std::vector<Individual> parents;
    parents.reserve(POP_SIZE);
    Individual t;
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

    int index1, index2;
    Individual child;
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

    for (int i = HALF_POP; i<POP_SIZE; i++){
        parents[i] = mutate(parents[i]);
    }

    if(DEBUG){
        for (int i = HALF_POP; i < POP_SIZE; i++){
            for (int j = 0; j < ITEM_AMOUNT; j++){
                std::cout << (int) parents[i].arr[j] << " ";
            }   
            std::cout << "Fitness: " << parents[i].fitness << std::endl;
        }
    }

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