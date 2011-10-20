all: MyBot

base.o: base.cc base.hh array.hh
	g++ -c base.cc -o base.o

MyBot.o: MyBot.cc
	g++ -c MyBot.cc -o MyBot.o

MyBot: MyBot.o base.o
	g++ -o MyBot MyBot.o base.o

clean:
	(rm *.o; rm MyBot;)