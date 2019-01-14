#name: Yoodong Lee, Won Jun Cho
CC = gcc
WFLAGS = -Wall -Wextra
EXE = 537ps
SCAN_BUILD_DIR = scan-build-out

$(EXE) : main.o process.o procHelp.o
	$(CC) $(WFLAGS) -o $(EXE) main.o process.o procHelp.o
main.o: main.c process.h
	$(CC) $(WFLAGS) -c main.c
process.o: process.c procHelp.h
	$(CC) $(WFLAGS) -c process.c
procHelp.o: procHelp.c procHelp.h	
	$(CC) $(WFLAGS) -c procHelp.c

clean: 
	rm -rf *.o
	rm -rf  537ps
	rm -rf $(SCAN_BUILD_DIR)

scan-build: clean
	scan-build -o $(SCAN_BUILD_DIR) make

scan-view: scan-build
	firefox -new-window $(SCAN_BUILD_DIR)/*/index.html
