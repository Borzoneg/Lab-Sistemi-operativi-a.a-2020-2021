CC=gcc
FLAGS=-Wall -g -o
SRC=Main.c
EXE=Main

compile: $(SRC)
	$(CC) $(FLAGS) $(EXE) -pthread $(SRC) -lm

clean:
	rm -rf $(EXE)

cleanresult:
	rm -R ./Results/tmpDir
