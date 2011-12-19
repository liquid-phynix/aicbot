CC=g++
#CXXFLAGS=-std=c++0x -O3 -c
CXXFLAGS=-std=c++0x -O3 -Wall -Wextra -c
LDFLAGS=-O3
EXE=MyBot
SRC=wandering.cpp base.cpp ants.cpp dij.cpp helpers.cpp loc.cpp MyBot.cpp pos.cpp hungarian.cpp draw.cpp perturn.cpp food.cpp battle.cpp cluster.cpp
HEADERS=ants.hh array.hh base.hh dij.hh hash.hh helpers.hh hungarian.hh loc.hh mask.hh pos.hh queue.hh
OBJ=$(SRC:.cpp=.o)

#CXXFLAGS+=-g -std=c++0x -Wall -Wextra -c

all: $(SRC) $(EXE)

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $@
.cpp.o: $(HEADERS)
	$(CC) $(CXXFLAGS) $< -o $@
clean: 
	-rm -f ${EXECUTABLE} ${OBJECTS}
	-rm *.ppm
	-rm sub.zip
rem:
	-rm *.ppm
sub:
	zip sub.zip *.cpp *.hh


