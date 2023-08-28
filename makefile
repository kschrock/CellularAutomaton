CC=g++
CFLAGS=-I.
DEPS = GraphicsClient.h CellularAutomaton.h
OBJ = CellularAutomaton.cpp 
CLEANC=rm -rf *.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

gol: casimulator.cpp
	g++ -o gol casimulator.cpp

clean:
	rm -f gol 
