CC = gcc
SOURCES = tinyShell.c gestionChaine.c variables.c liste_chaine.c tubeCommunication.c
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