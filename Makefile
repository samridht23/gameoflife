CXX = g++
CXXFLAGS = -Wall -Wextra -Wpedantic -std=c++11 -lSDL2
SRC = main.cpp
OBJ=$(SRC:.cpp=.o)
EXE = gameoflife

all:
	$(CXX) $(SRC) $(CXXFLAGS) -o $(EXE)

clean:
	rm -f $(OBJ) $(EXE)

