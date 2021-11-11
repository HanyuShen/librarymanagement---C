all: test-suite

test-suite: test-suite.o catch.o library.o
	g++ -o test-suite test-suite.o catch.o library.o

catch.o: catch.cpp catch.h 
	g++ -c catch.cpp

library.o: library.cpp library.h
	g++ -c library.cpp

test-suite.o: test-suite.cpp library.h catch.h
	g++ -c test-suite.cpp

test:
	./test-suite

clean:
	rm -f test-suite test-suite.o catch.o library.o