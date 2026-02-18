#Define the C complier to use 
CC=gcc

#Define any compile-time flags
CFLAGS=-Wall -Wextra

#Define the target executable name
TARGET=myshell

#'all' is the default target
all: $(TARGET)

#the target will be rebuilt then when 'make' is run
$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)