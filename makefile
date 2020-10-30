all: project2_first.out project2_second.out

project2_first.out: main.o dictionary.o hash24.o
	g++ -std=c++11 $^ -o $@

project2_second.out: <list of .cpp files reuired for project2_second.out>
	g++ -std=c++11 $^ -o $@

clean:
	rm -f *.out