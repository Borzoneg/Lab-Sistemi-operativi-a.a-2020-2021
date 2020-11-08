CC=gcc
FLAGS=-Wall -g -o
SRC=Main.c
EXE=Main
RESULT=Exampleroot

compile: $(SRC)
	$(CC) $(FLAGS) $(EXE) $(SRC) -lm

clean:
	rm -rf $(EXE)

resultClean:
	rm -r ./$(RESULT)-copy/*
