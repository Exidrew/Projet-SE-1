CC = gcc
SOURCES = tinyShell.c variables.c
FLAGS = -Wall

ts : $(SOURCES)
	$(CC) $(FLAGS) $^ -o $@

clean :
	rm -f *.o *.exe ts