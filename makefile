all: MyBot

array.o: array.hh array.cc
	g++ -c array.cc -o array.o

base.o: base.cc base.hh array.hh
	g++ -c base.cc -o base.o

MyBot.o: MyBot.cc
	g++ -c MyBot.cc -o MyBot.o

MyBot: MyBot.o base.o array.o
	g++ -o MyBot MyBot.o base.o array.o

clean:
	(rm *.o; rm MyBot;)