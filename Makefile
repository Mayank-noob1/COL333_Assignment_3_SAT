num = 1
N = 1000
K1 = 200
K2 = 100


part1: 
	./compile.sh
	./run1.sh data/input$(num)
	- minisat data/input$(num).satinput data/input$(num).satoutput > temp/minisatlog_input$(num).satlog
	./run2.sh data/input$(num)

part2:
	./compile.sh
	./run3.sh data/input$(num)

rand1:
	python3 problemGenerator1.py $(N) $(K1) $(K2) data/input0.graph
	make part1 num=0
	python3 checker.py data/input0.graph data/input0.mapping 1

rand2:
	python3 problemGenerator2.py $(N) data/input0.graph
	make part2 num=0
	python3 checker.py data/input0.graph data/input0.mapping 2