CC = gcc
SOURCES = error.c tinyShell.c gestionChaine.c variablesLocales.c variables.c tubeCommunication.c cd.c
PSSOURCES = myps.c procdata.c error.c redirections.c tubeCommunication.c
SSH_SOURCES = myssh.c error.c
FLAGS = -Wall
DEBUG_FLAGS = -Wall -g

myssh : $(SSH_SOURCES)
	make clear
	$(CC) $(FLAGS) $^ -o $@

myssh-debug : $(SSH_SOURCES)
	make clear
	$(CC) $(DEBUG_FLAGS) $^ -o $@

debugmyssh :
	valgrind --tool=memcheck --leak-check=full --leak-resolution=high --show-reachable=yes --track-origins=yes -s ./myssh-debug

make ts : $(SOURCES) $(PSSOURCES)
	make clear
	$(CC) $(FLAGS) $(SOURCES) -o ts
	$(CC) $(FLAGS) $(PSSOURCES) -o myps
	
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
	rm build-debug myps myps-debug ts ts-debug myssh myssh-debug 2> /dev/null || true
