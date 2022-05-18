#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/transform_reduce.h>
#include <thrust/functional.h>
#include <thrust/transform.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <chrono>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <random>

using namespace std::chrono;

void reportTime(const char* msg, steady_clock::duration span) {
    auto ms = duration_cast<milliseconds>(span);
    std::cout << msg << " - levou - " <<
    ms.count() << " milisegundos" << std::endl;
}

// CRIE UMA FUNCTOR PARA CALCULAR A SQUARE
struct square
{
    __host__ __device__
    double operator()(const double& x) {
        return x * x;
    }
};

template<typename T>
struct unary_square
{
  __host__ __device__ 
  T operator()(const T &x) const
  {
    return x * x;
  }
};



// IMPLEMENTE O CALCULO DA MAGNITUDE COM THRUST
double magnitude(thrust::device_vector<double> x) {
    double result;
    thrust::device_vector<double> x2(x.size()); // Output
    thrust::transform(x.begin(), x.end(), x2.begin(), square()); // Square
    result = std::pow(thrust::reduce(x2.begin(), x2.end()),0.5); // Sqrt of Sum
    return result;
}

double magnitude2(thrust::device_vector<double> x) {
    double result;
    result = std::sqrt(thrust::transform_reduce(x.begin(), x.end(), unary_square<double>(), 0.0, thrust::plus<double>())); // Square
    return result;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << argv[0] << ": numero invalido de argumentos\n"; 
        std::cerr << "uso: " << argv[0] << "  tamanho do vetor\n"; 
        return 1;
    }
    int n = std::atoi(argv[1]); //numero de elementos
    steady_clock::time_point ts, te;

    // Faça um  vector em thrust 
    thrust::device_vector<double> x(n);
    // inicilize o  vector
    ts = steady_clock::now();
    std::generate(x.begin(),x.end(), std::rand);
    te = steady_clock::now();
    reportTime("Inicializacao", te - ts);
    // Calcule a magnitude do vetor
    ts = steady_clock::now();
    float len = magnitude(x);
    te = steady_clock::now();
    reportTime("Tempo para calculo", te - ts);

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Magnitude : " << len << std::endl;
    
    // Init
    // ts = steady_clock::now();
    // std::generate(x.begin(),x.end(), std::rand);
    // te = steady_clock::now();
    // reportTime("Inicializacao", te - ts);
    // Transform reduce
    ts = steady_clock::now();
    len = magnitude2(x);
    te = steady_clock::now();
    reportTime("Tempo para calculo", te - ts);


    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Magnitude : " << len << std::endl;
}
