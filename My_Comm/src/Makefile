mympi_test:main.o mympi.o
	g++ -g -Wall -pthread main.o mympi.o -o mympi_test 
mympi.o:mympi.cpp mympi.h
	g++ -g -Wall -c mympi.cpp -o mympi.o
main.o:main.cpp mympi.h mympi.cpp
	g++ -g -Wall -c main.cpp -o main.o

clean:
	rm -rf main.o mympi.o mympi_test
