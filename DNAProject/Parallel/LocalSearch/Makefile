main_seq: main_seq.cpp func.hpp
	g++ -Og -Wall -fopenmp main_seq.cpp -o main_seq

main_par: main_par.cpp func.hpp
	g++ -Og -Wall -fopenmp main_par.cpp -o main_par

run: main_par main_seq
	./main_par < in.txt
	./main_seq < in.txt