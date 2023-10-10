num = 1
n = 7

run1: Clique.cpp
	g++ -o bin/main.exe Clique.cpp
	bin/main.exe 0 input/input$(num).txt clause/clause$(num).txt
	- minisat clause/clause$(num).txt sat_output/sat_output$(num).txt
	bin/main.exe 1 sat_output/sat_output$(num).txt output1/output$(num).txt $(n)

run2: Clique.cpp
	g++ -o bin/main.exe Clique.cpp
	bin/main.exe 2 input/input$(num).txt output2/output$(num).txt