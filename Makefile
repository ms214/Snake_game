game: game.o snake.o
	g++ -std=c++11 -o snake game.o snake.o -lncursesw -lpthread

game.o : game.cpp
	g++ -c game.cpp

snake.o : snake.cpp snake.h
	g++ -c snake.cpp

clean:
	rm *.o

debug:
	g++ -std=c++11 -g -o dbg game.cpp snake.cpp -lncursesw
