CC = gcc
SOURCES = error.c tinyShell.c gestionChaine.c variablesLocales.c variables.c tubeCommunication.c cd.c
FLAGS = -Wall
DEBUG_FLAGS = -Wall -g

build-debug : $(SOURCES)
	$(CC) $(DEBUG_FLAGS) $^ -o build-debug

ts : $(SOURCES)
	$(CC) $(FLAGS) $^ -o $@

clean :
	rm -f *.o *.exe ts

debug :
	valgrind --tool=memcheck --leak-check=full --leak-resolution=high --show-reachable=yes --track-origins=yes -s ./build-debug

gc : gestionChaine.c
	$(CC) $(DEBUG_FLAGS) $^ -o gc