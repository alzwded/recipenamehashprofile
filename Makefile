test.out: main.o pearson.o
	g++ -fopenmp -o test.out main.o pearson.o -lgomp

stress.out: stress.o pearson.o
	g++ -fopenmp -o stress.out stress.o pearson.o -lgomp -v

main.o: main.cpp
	g++ -fopenmp -c -std=c++11 -o main.o main.cpp

pearson.o: pearson.c
	gcc -fopenmp -c -O4 -o pearson.o pearson.c

stress.o: stress.cpp
	g++ -fopenmp -c -O4 -std=c++11 -o stress.o stress.cpp -v

clean:
	rm -f pearson.o main.o test.out stress.out stress.o
