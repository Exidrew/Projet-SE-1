CC = gcc
SOURCES = error.c tinyShell.c gestionChaine.c variablesLocales.c variables.c tubeCommunication.c cd.c
PSSOURCES = myps.c procdata.c error.c redirections.c tubeCommunication.c
LSSOURCES = myls.c error.c
SSH_SOURCES = myssh.c error.c client.c server.c gestionChaine.c
DAEMON_SOURCES = $(SOURCES) daemon.c server.c
FLAGS = -Wall -pedantic
DEBUG_FLAGS = -Wall -pendantic -g

myssh :
	make clear
	$(CC) $(FLAGS) $(DAEMON_SOURCES) -DSERVER -pthread -o startServer
	$(CC) $(FLAGS) $(SSH_SOURCES) -o myssh
	$(CC) $(FLAGS) $(PSSOURCES) -o myps
	$(CC) $(FLAGS) $(LSSOURCES) -o myls

server :
	$(CC) $(FLAGS) $(DAEMON_SOURCES) -pthread -o startServer

myssh-debug : $(SSH_SOURCES)
	make clear
	$(CC) $(DEBUG_FLAGS) $^ -o $@

debugmyssh :
	valgrind --tool=memcheck --leak-check=full --leak-resolution=high --show-reachable=yes --track-origins=yes -s ./myssh-debug toto@10.10.10.1

make ts : $(SOURCES) $(PSSOURCES)
	make clear
	$(CC) $(FLAGS) $(SOURCES) -o ts
	$(CC) $(FLAGS) $(PSSOURCES) -o myps
	$(CC) $(FLAGS) $(LSSOURCES) -o myls

	
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
