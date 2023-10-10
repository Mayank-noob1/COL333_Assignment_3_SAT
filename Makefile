num = 1

run1: Clique.cpp
	g++ -o bin/main.exe Clique.cpp
	bin/main.exe 0 input/input$(num).txt clause/clause$(num).txt
	minisat clause/clause$(num).txt output/output$(num).txt
