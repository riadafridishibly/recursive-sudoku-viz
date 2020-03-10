OBJS	= main.o
SOURCE	= main.cc
HEADER	= 
OUT		= solver
CC	 	= g++
FLAGS	= -g -c -Wall
LFLAGS	= -lm -lraylib


all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)


main.o: main.cc
	$(CC) $(FLAGS) main.cc 

clean:
	rm -f $(OBJS) $(OUT)
