all : run.o

CC = g++
CFLAGS = -std=c++11 -pthread `pkg-config --cflags --libs opencv`
PICAMFLAGS = -L/opt/vc/lib -I/usr/local/include/ -lraspicam -lraspicam_cv -lmmal -lmmal_core -lmmal_util

run.o : main.o CaptureThread.o
	$(CC) -o $@ $^ $(CFLAGS) $(PICAMFLAGS)

main.o : main.cpp
	$(CC) -c $^ $(CFLAGS) $(PICAMFLAGS)

CaptureThread.o : CaptureThread.cpp CaptureThread.h
	$(CC) -c $^ $(CFLAGS) $(PICAMFLAGS)

clean :
	rm -rf *.o *.gch
