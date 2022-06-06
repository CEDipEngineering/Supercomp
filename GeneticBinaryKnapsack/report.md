# Genetic Algorithms

## Implementation in C++

C++ is a great language when weeking performance. The optimizations that can be put in place by the compiler due to characteristics of the language, such as its hard typing of variables, and strictness in memorya allocation and access, really give it a big speed boost compared to interpreted languages such as python, and even against VM languages such as Java or C#.

## Challenges

Most of the challenges were in the form of weird memory initialization quirks, which ended up being solved by initializing vectors and arrays as one would in regular C99.

## The algorithm

Genetic algorithms take inspiration from nature's own mechanisms for selecting fit solutions for a given environment. All the ideas that appear in the algorithm are in some way a mirror of our understanding regarding nature's evolutionary machinations. 

We start with a random population, which we could have built using some rules regarding the nature of the problem (a.k.a exploitation of the problem), however this was not implemented, as it is not necessary for the algorithm to converge on a viable answer.

This population then is evaluated for its fitness (in the knapsack problem's case, the fitness is the total value accumulated inside that individual, or zero if it got above the maximum capacity), and some percentage of the population is killed off for not being fit enough (in my program, 50% is always removed in each generation). Then, we pick 2 of the remaining survivors at random, and breed them (the breed, or crossover, process involves selecting a random amount of one individual's contents, copying that to their child, and getting the rest from the other parent). After that, there is a step of mutations that is applied, which simply runs over every item, in every individual's contents and has a small chance to flip its state. This way, we avoid local maxima, and increase the likelihood of finding the global best solution.

The process described above is repeated for each generation, wherein the best individual is recorded for later analysis.

The best individual in the history of the experiment will be the optimal solution found by the algorithm.

Note that this algorithm is NOT extensive, and does not guarantee the perfect solution.

## Conclusion

This algorithm can converge on an optimal solution very quickly, since it makes good use of randomness to explore the space of possibilities, meanwhile still maintaining a sense of slow and steady approach to its final objective, by making use of knowledge regarding the problem's characteristics (which takes shape by way of the fitness function).

The real advantage of this technique comes from the fact that any problem for which you can define a fitness function, can be solved using this algorithm.

There are many ways to improve this implementation, such as modifying the fitness function to consider other factors, or applying heuristics to the generation of the initial population.