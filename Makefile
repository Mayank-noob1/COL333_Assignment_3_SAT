num = 1
type = 0

run: Clique.cpp
	g++ -o bin/main.exe Clique.cpp
	bin/main.exe $(type) input/input$(num).txt clause/clause$(num).txt
	minisat clause/clause$(num).txt output/output$(num).txt
