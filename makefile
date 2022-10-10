default:
	g++ -O3 *.cpp -o game
native:
	g++ -O3 -march=native *.cpp -o game