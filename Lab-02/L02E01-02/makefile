CC=gcc
FLAGS=-Wall -g -o
SRC=Main.c
EXE=Main

compile: $(SRC)
	$(CC) $(FLAGS) $(EXE) $(SRC) -lm

install:
	mkdir bin
	$(CC) $(FLAGS) $(EXE) $(SRC) -lm
	mv $(EXE) bin

clean:
	rm -rf $(EXE)

distclean:
	rm -r bin
