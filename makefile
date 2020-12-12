CC = gcc
SOURCES = error.c tinyShell.c gestionChaine.c variablesLocales.c variables.c tubeCommunication.c cd.c
PSSOURCES = myps.c procdata.c error.c
FLAGS = -Wall
DEBUG_FLAGS = -Wall -g

make ts : $(SOURCES) $(PSSOURCES)
	make clear
	$(CC) $(FLAGS) $(SOURCES) -o $@
	$(CC) $(FLAGS) $(PSSOURCES) -o $@
	
ts-debug : $(SOURCES) $(PSSOURCES)
	make clear
	$(CC) $(DEBUG_FLAGS) $(SOURCES) -o ts-debug
	$(CC) $(DEBUG_FLAGS) $(PSSOURCES) -o myps-debug

myps : $(PSSOURCES)
	make clear
	$(CC) $(FLAGS) $^ -o $@

myps-debug : $(PSSOURCES)
	make clear
	$(CC) $(DEBUG_FLAGS) $^ -o $@

debugmyps :
	valgrind --tool=memcheck --leak-check=full --leak-resolution=high --show-reachable=yes --track-origins=yes -s ./myps-debug

debugts :
	valgrind --tool=memcheck --leak-check=full --leak-resolution=high --show-reachable=yes --track-origins=yes -s ./ts-debug

clear :
	rm build-debug myps mypsdebug ts 2> /dev/null || true
