CC = gcc
CFLAGS = -Wall -g
TARGET = test

all: $(TARGET)

$(TARGET): testfile.c libFS.c 
	$(CC) $(CFLAGS) -o $(TARGET)

run: $(TARGET) 
	./$(TARGET)