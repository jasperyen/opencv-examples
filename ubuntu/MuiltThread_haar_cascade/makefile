all : run.o

CC = g++
CFLAGS = -std=c++11 -pthread `pkg-config --cflags --libs opencv`

run.o : main.o CaptureThread.o CascadeThread.o
	$(CC) -o $@ $^ $(CFLAGS)

main.o : main.cpp
	$(CC) -c $^ $(CFLAGS)

CaptureThread.o : CaptureThread.cpp CaptureThread.h
	$(CC) -c $^ $(CFLAGS)

CascadeThread.o : CascadeThread.cpp CascadeThread.h
	$(CC) -c $^ $(CFLAGS)

clean :
	rm -rf *.o *.gch
