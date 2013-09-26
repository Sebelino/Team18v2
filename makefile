all:
	g++ -g -O2 -static *.cpp -o ./sokoban

cpp11:
	g++ -std=gnu++0x -g -O2 -static *.cpp -o ./sokoban

test:
	./sokoban < maps/test
