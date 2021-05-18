CC=gcc
#CC=clang
INC=-Iinclude
CFLAGS=-Wall -c -g -std=c11 -fms-extensions $(shell sdl2-config --cflags)
LDFLAGS=$(shell sdl2-config --libs) -lm
# binaries here
BIN_FOLDER=bin
# object files here
OBJ_FOLDER=obj
# all c code here
SRC_FOLDER=src
# search all c code in folder with sources
C_SOURCES=$(shell find $(SRC_FOLDER)/ -name "*.c")
# object files names == sources with .o suffix and placed in OBJ_FOLDER
#OBJS=$(addprefix $(OBJ_FOLDER)/,$(notdir $(patsubst %.c,%.o,$(C_SOURCES))))
OBJS=$(notdir $(patsubst %.c,%.o,$(C_SOURCES)))
#OBJS=color.o    \
	 vectors.o vector2.o vector3.o vector4.o \
	 draw.o     \
	 fps.o      \
	 main.o     \
	 matrix.o   \
	 render.o   \
	 texture.o	\
	 utils.o
EXECUTABLE=$(BIN_FOLDER)/tiny_render

all: $(EXECUTABLE)

color.o: src/color.c
	$(CC) $(INC) $(CFLAGS) src/color.c
vectors.o: src/vectors/vectors.c
	$(CC) $(INC) $(CFLAGS) src/vectors/vectors.c
vector2.o: src/vectors/vector2.c
	$(CC) $(INC) $(CFLAGS) src/vectors/vector2.c
vector3.o: src/vectors/vector3.c
	$(CC) $(INC) $(CFLAGS) src/vectors/vector3.c
vector4.o: src/vectors/vector4.c
	$(CC) $(INC) $(CFLAGS) src/vectors/vector4.c
draw.o: src/draw.c
	$(CC) $(INC) $(CFLAGS) src/draw.c
fps.o: src/fps.c
	$(CC) $(INC) $(CFLAGS) src/fps.c
main.o: src/main.c
	$(CC) $(INC) $(CFLAGS) src/main.c
matrix.o: src/matrix.c
	$(CC) $(INC) $(CFLAGS) src/matrix.c
render.o: src/render.c
	$(CC) $(INC) $(CFLAGS) src/render.c
texture.o: src/texture.c
	$(CC) $(INC) $(CFLAGS) src/texture.c
utils.o: src/utils.c
	$(CC) $(INC) $(CFLAGS) src/utils.c

#$(OBJS): $(C_SOURCES)
#	$(CC) $(INC) $(CFLAGS) $(C_SOURCES)
#	mkdir -p $(OBJ_FOLDER)
#	mv *.o $(OBJ_FOLDER)/
#

$(EXECUTABLE): $(OBJS)
	mkdir -p $(BIN_FOLDER)
	$(CC) $(OBJS) $(LDFLAGS) -o $(EXECUTABLE)

release: $(OBJS)
	mkdir -p $(BIN_FOLDER)
	$(CC) $(OBJS) $(LDFLAGS) -o $(EXECUTABLE) -O3 -s
	
run: $(EXECUTABLE)
	export PATH=$(BIN_FOLDER)
	./$(EXECUTABLE)	

clean:
	rm -rf $(OBJS) $(EXECUTABLE)
