CF = -O3 -Wall -pedantic

default: clean main.o Grid.o Pixel.o Image.o
	g++ main.o Grid.o Pixel.o Image.o -o exe
	./exe
	eog maze.bmp

main.o: main.cpp
	g++ $(CF) -c main.cpp -o main.o

Grid.o: Grid.cpp
	g++ $(CF) -c Grid.cpp -o Grid.o

clean:
	rm -f main.o Grid.o exe *.bmp

preview:
	eog maze.bmp