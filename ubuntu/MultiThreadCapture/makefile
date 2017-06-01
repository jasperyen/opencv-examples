all : run.o

CC = g++
CFLAGS = -std=c++11 -pthread `pkg-config --cflags --libs opencv`

run.o : main.o CaptureThread.o PiCaptureThread.o
	$(CC) -o $@ $^ $(CFLAGS)

main.o : main.cpp
	$(CC) -c $^ $(CFLAGS)

CaptureThread.o : CaptureThread.cpp CaptureThread.h
	$(CC) -c $^ $(CFLAGS)

PiCaptureThread.o : PiCaptureThread.cpp PiCaptureThread.h
	$(CC) -c $^ $(CFLAGS)

clean :
	rm -rf *.o *.gch
