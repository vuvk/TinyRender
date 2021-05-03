CC=gcc
CFLAGS=-Iinclude -Wall -g $(shell sdl2-config --cflags)
LDFLAGS=$(shell sdl2-config --libs) -lm
# binaries here
BIN_FOLDER=bin
# object files here
OBJ_FOLDER=obj
# all c code here
SRC_FOLDER=src
# search all c code in folder with sources
C_SOURCES=$(shell find $(SRC_FOLDER)/ -name "*.c")
# object files names == sources with .* suffix and placed in OBJ_FOLDER
OBJS=$(addprefix $(OBJ_FOLDER)/,$(notdir $(patsubst %.c,%.o,$(C_SOURCES))))
EXECUTABLE=$(BIN_FOLDER)/tiny_render

all: $(EXECUTABLE)

$(OBJS): $(C_SOURCES)
	$(CC) $(CFLAGS) -c $(C_SOURCES)
	mkdir -p $(OBJ_FOLDER)
	mv *.o $(OBJ_FOLDER)/

$(EXECUTABLE): $(OBJS)
	mkdir -p $(BIN_FOLDER)
	$(CC) $(OBJS) $(LDFLAGS) -o $(EXECUTABLE)

release: $(OBJS)
	mkdir -p $(BIN_FOLDER)
	$(CC) $(OBJS) $(LDFLAGS) -o $(EXECUTABLE) -O3 -s
	
clean:
	rm -rf $(OBJ_FOLDER) $(EXECUTABLE)
