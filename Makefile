CC=gcc
CFLAGS=-Iinclude -Wall -g $(shell sdl2-config --cflags) 
LDFLAGS=$(shell sdl2-config --libs) -lm
C_SOURCES=$(shell find src/ -name "*.c")
OBJS=$(notdir $(patsubst %.c,%.o,$(C_SOURCES)))
EXECUTABLE=tiny_render

all: $(EXECUTABLE)

$(OBJS): $(C_SOURCES)
	$(CC) $(CFLAGS) -c $(C_SOURCES)

$(EXECUTABLE): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(EXECUTABLE)

clean:
	rm -rf $(OBJS) $(EXECUTABLE)
