CFLAGS=-O3

all: MyBot

base.o: base.cc base.hh array.hh
	g++ $(CFLAGS) -c base.cc -o base.o

MyBot.o: MyBot.cc
	g++ $(CFLAGS) -c MyBot.cc -o MyBot.o

MyBot: MyBot.o base.o
	g++ $(CFLAGS) -o MyBot MyBot.o base.o

clean:
	(rm *.o; rm MyBot;)