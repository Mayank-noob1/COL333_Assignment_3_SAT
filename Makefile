num = 1

part1: 
	./compile.sh
	./run1.sh data/input$(num)
	- minisat data/input$(num).satinput data/input$(num).satoutput >> satlog/input$(num).satlog
	./run2.sh data/input$(num)

part2:
	./compile.sh
	./run3.sh data/input$(num)