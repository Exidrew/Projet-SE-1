CC = gcc
SOURCES = error.c tinyShell.c gestionChaine.c variablesLocales.c variables.c tubeCommunication.c cd.c
PSSOURCES = myps.c procdata.c error.c
FLAGS = -Wall
DEBUG_FLAGS = -Wall -g

make all $(SOURCES) $(PSSOURCES)

build-debug : $(SOURCES)
	make clear
	$(CC) $(DEBUG_FLAGS) $^ -o build-debug

ts : $(SOURCES)
	make clear
	$(CC) $(FLAGS) $^ -o $@

myps : $(PSSOURCES)
	make clear
	$(CC) $(FLAGS) $^ -o $@

myps-build-debug : $(PSSOURCES)
	make clear
	$(CC) $(DEBUG_FLAGS) $^ -o mypsdebug

debugmyps :
	valgrind --tool=memcheck --leak-check=full --leak-resolution=high --show-reachable=yes --track-origins=yes -s ./mypsdebug

debug :
	valgrind --tool=memcheck --leak-check=full --leak-resolution=high --show-reachable=yes --track-origins=yes -s ./build-debug

gc : gestionChaine.c
	make clear
	$(CC) $(DEBUG_FLAGS) $^ -o gc

clear :
	rm build-debug myps mypsdebug ts 2>/dev/null || true
