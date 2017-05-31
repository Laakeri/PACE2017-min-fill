CC = g++
CFLAGS = -std=c++11 -O2 -Wall -Wextra -Wshadow -g -Wfatal-errors
OBJFILES = graph.o IO.o MCS.o minfill.o PMC.o preprocess.o timer.o vertexcover.o

all: solver

solver: solver.o $(OBJFILES)
	$(CC) $(CFLAGS) solver.o $(OBJFILES) -o solver

%.o: %.cpp
	$(CC) $(CFLAGS) -MMD -c $< -o $@

-include *.d

clean: 
	rm -f *.o *.d