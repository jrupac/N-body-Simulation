CC=g++
TARGET=elastic
INPUT_SRCS=main.cpp
INPUT_OBJS=$(INPUT_SRCS:.cpp=.o)

CFLAGS=-o $(TARGET)
DEBUG=-g
LIBS=-lglut -lGLU

all: $(INPUT_OBJS)
	$(CC) $(CFLAGS) $(LIBS) $^

debug: $(INPUT_OBJS)
	$(CC) $(DEBUG) $(CFLAGS) $(LIBS) $^

clean:
	rm -f $(TARGET) $(INPUT_OBJS)

