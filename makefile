all:
	g++ -g -O2 -static *.cpp -std=gnu++0x -o ./sokoban

tester:
	g++ -g -O2 -static *.cpp -o ./sokoban
	./sokoban test

cpp11:
	g++ -std=gnu++0x -g -O2 -static *.cpp -o ./sokoban

test:
	./sokoban < maps/test

testii:
	./sokoban < maps/test2
