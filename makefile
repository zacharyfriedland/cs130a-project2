all: project2_first.out project2_second.out

project2_first.out: project2_first.o dictionary.o hash24.o
	g++ -std=c++11 $^ -o $@

project2_second.out: project2_second.o dictionary.o hash24.o
	g++ -std=c++11 $^ -o $@

clean:
	rm -f *.out *.o