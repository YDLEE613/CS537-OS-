#name: Yoodong Lee, Won Jun Cho
CC = gcc
WFLAGS = -Wall -Wextra
EXE = 537ps

$(EXE) : main.o process.o procHelp.o
	$(CC) $(WFLAGS) -o $(EXE) main.o process.o procHelp.o
main.o: main.c process.h
process.o: process.c procHelp.h
procHelp.o: procHelp.c procHelp.h	

clean: 
	rm edit *.o
	rm edit 537ps
