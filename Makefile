snake: snake.o
	g++ -std=c++11 -o snake snake.o -lncursesw

snake.o : snake.cpp
	g++ -c snake.cpp
