CC=gcc
CFLAGS= -Wall $(shell sdl2-config --cflags)
LIBS=$(shell sdl2-config --libs) -lSDL2_image -lm

# Target executable name
TARGET=client

# Source files
SRCS=client.c Entity.c

# Object files
OBJS=$(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link the target with object files and libraries
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

# Compile source files into object files
%.o: %.c
	$(CC) -c $< $(CFLAGS)

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Debug target, prints variables
debug:
	@echo "SRCS = $(SRCS)"
	@echo "OBJS = $(OBJS)"
	@echo "CFLAGS = $(CFLAGS)"
	@echo "LIBS = $(LIBS)"

