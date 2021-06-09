game: game.o snake.o gate.o
	g++ -std=c++11 -o snake game.o gate.o snake.o -lncursesw -lpthread

game.o : game.cpp
	g++ -c game.cpp

gate.o : gate.cpp gate.h
	g++ -c gate.cpp

snake.o : snake.cpp snake.h
	g++ -c snake.cpp

clean:
	rm *.o

debug:
	g++ -std=c++11 -g -o dbg game.cpp snake.cpp gate.cpp -lncursesw
