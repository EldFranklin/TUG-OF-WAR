CROSS_COMPILE=arm-none-linux-gnueabihf-gcc
SRC=src/
OBJ=obj/
BIN=bin/

OBJS = main.o functions.o
all: folder app

app: $(OBJS)
	${CROSS_COMPILE} $(OBJ)main.o $(OBJ)functions.o  -static -o $(BIN)app

main.o: $(SRC)main.c
	${CROSS_COMPILE} -c $(SRC)main.c -o $(OBJ)main.o -Wall

functions.o: $(SRC)functions.c
	${CROSS_COMPILE} -c $< -o $(OBJ)functions.o -Wall

folder: 
	mkdir -p bin obj

clean:
	rm obj/ bin/ -rf obj/