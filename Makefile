CC = gcc

INC =
CFLAGS = -g -O0 -Wall -lrt

OBJS = main.o
SRCS = main.c
LIBS = -pthread

TARGET = main

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $@ $(OBJS)

dep :
	gccmakedep $(INC) $(SRCS)

clean :
	rm -rf $(OBJS) $(TARGET) core
