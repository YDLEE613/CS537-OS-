#Won Jun Cho, Yoodong Lee
#wcho8, won || ydlee399, yoodong
CC = gcc
WFLAGS = -Wall -Wextra
EXE = prodcomm
SCAN_BUILD_DIR = scan-build-out

$(EXE) : main.o reader.o munch1.o munch2.o writer.o queue.o
	$(CC) $(WFLAGS) -pthread -o $(EXE) main.o reader.o munch1.o munch2.o writer.o queue.o
main.o: main.c
	$(CC) $(WFLAGS) -c main.c
reader.o: reader.c reader.h
	$(CC) $(WFLAGS) -c reader.c
munch1.o: munch1.c munch1.h	
	$(CC) $(WFLAGS) -c munch1.c
munch2.o: munch2.c munch2.h
	$(CC) $(WFLAGS) -c munch2.c
writer.o: writer.c writer.h
	$(CC) $(WFLAGS) -c writer.c
queue.o: queue.c queue.h
	$(CC) $(WFLAGS) -c queue.c

clean: 
	rm -rf *.o
	rm -rf $(SCAN_BUILD_DIR)

scan-build: clean
	scan-build -o $(SCAN_BUILD_DIR) make

#scan-view: scan-build
#	firefox -new-window $(SCAN_BUILD_DIR)/*/index.html
