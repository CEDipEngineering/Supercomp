#include<iostream>
#include<vector>
#include<algorithm>

struct item{
    int id;
    double peso;
    double valor;
};


int num_leaf = 0;
int num_copy = 0;

double knapSack2(int W, std::vector<item> items, std::vector<item>& usados, std::vector<item>& melhor){
    static double MAX_VAL = -1.0;
    
    double valor = 0.0;
    double peso = 0.0;
    double sem_i = 0.0, com_i = 0.0;
    // Copia dos items
    std::vector<item> items2 = items;

    // Folha da árvore
    if(items.empty()) {
        num_leaf++;
    }
    if(items.empty() || W == 0)
        return 0;

    int upper_bound = 0;
    for(auto& el : items){
        upper_bound += el.valor;
    }
    for(auto& el : usados){
        upper_bound += el.valor;
    }

    if (upper_bound < MAX_VAL) {
        return 0.0;
    }

    // Inserir item se couber
    if(items[0].peso <= W){
        usados.push_back(items[0]);
        valor = items[0].valor;
        peso = items[0].peso;
        items.erase(items.begin());
        com_i = knapSack2(W - peso, items, usados, melhor);
    }

    // Fingir que não existe
    items2.erase(items2.begin());
    sem_i = knapSack2(W, items2, usados, melhor);


    double valor_atual = 0.0, valor_melhor = 0.0;
    for(auto& el : usados){
        valor_atual += el.valor;
    }
    for(auto& el : melhor){
        valor_melhor += el.valor;
    }
    if(valor_atual > valor_melhor){
        melhor = usados;
        MAX_VAL = valor_atual;
        num_copy++;
    }
    usados.clear();

    return std::max(sem_i, valor + com_i);
}

int main() {

    // Coleta de inputs
    int n = 0;
    int W = 0;
    std::cin >> n >> W;

    // Items
    std::vector<item> mochila;
    std::vector<item> items;
    std::vector<item> usado;
    std::vector<item> melhor;
    items.reserve(n);
    usado.reserve(n);
    double peso, valor;
    for(int i = 0; i < n; i++) {
        std::cin >> peso;
        std::cin >> valor;
        items.push_back({i, peso, valor});
    }
    

    std::cout << "RESULTADO = " << knapSack2(W, items, usado, melhor) << std::endl;
    for(auto& el: melhor) {
            std::cout << el.id << " ";
    }
    std::cout << std::endl;
    std::cout << "NUM_LEAF = " << num_leaf << " NUM_COPY = " << num_copy << std::endl;
    return 0;
}