CC = gcc
SOURCES = tinyShell.c
FLAGS = -Wall

ts : $(SOURCES)
	$(CC) $(FLAGS) $(SOURCES) -o $@