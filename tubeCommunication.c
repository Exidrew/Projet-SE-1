#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "headers/variables.h"
#include "headers/variablesLocales.h"
#include "headers/tubeCommunication.h"

#define ERR -1

int creerTubeDeCommunication(int tubeDescriptor[2]) {
    int idPipe = pipe(tubeDescriptor);

    if (idPipe == ERR) {
        perror("Pipe cannot be created");
        free(tubeDescriptor);
        exit(1);
    }

    return idPipe;
}

void ecrireVersSortieStandard(int tubeDescriptor[2]) {
    close(STDOUT_FILENO);
    dup(tubeDescriptor[1]);
    close(tubeDescriptor[1]);
}

void lireEcrireVersStandard(int tubeDescriptor[2]) {
    close(STDIN_FILENO);
    dup(tubeDescriptor[0]);
    close(STDOUT_FILENO);
    dup(tubeDescriptor[1]);
    close(tubeDescriptor[0]);
    close(tubeDescriptor[1]);
}

void lireDepuisEntreeStandard(int tubeDescriptor[2]) {
    close(STDIN_FILENO);
    dup(tubeDescriptor[0]);
    close(tubeDescriptor[0]);
}

void fermerCoteTubes(int nbArgs, ...) {
    va_list ap;
    va_start(ap, nbArgs);

    while (nbArgs > 0) {
        int tubeDescriptor;
        tubeDescriptor = va_arg(ap, int);
        close(tubeDescriptor);
        nbArgs--;
    }
}

void fermerTube(int tubeDescriptor[2]) {
    close(tubeDescriptor[0]);
    close(tubeDescriptor[1]);
}

void ecrireVersTube(int tubeDescriptor[2], char* str) {
    close(tubeDescriptor[0]);
    write(tubeDescriptor[1], str, strlen(str));
    close(tubeDescriptor[1]);
}

void lireDepuisTube(int tubeDescriptor[2], char* str, int maxLength) {
    close(tubeDescriptor[1]);
    read(tubeDescriptor[0], str, maxLength);
    close(tubeDescriptor[0]);
}

void lectureArgumentsDepuisFd(char* arguments, int fileDescriptor, int argc, char* argv[]) {
    if (argc-1 > 0 && !strncmp(argv[argc-1], "l", strlen("l"))) {
        char* in = (char*) calloc(1, sizeof(char));
        int tailleMessage = 0, tailleActuelle = 1;
        char car = '\0';

        while (read(STDIN_FILENO, &car, 1) > 0) {
            if (car == '\n' && in[tailleMessage-1] == '\n') break;
            tailleMessage++;
            if (tailleActuelle < tailleMessage) {
                tailleActuelle *= 2;
                in = realloc(in, tailleActuelle * sizeof(char));
            }
            in[tailleMessage-1] = car;
        }
        arguments = realloc(arguments, (strlen(argv[0]) + strlen(in) + 1) * sizeof(char));
        strcat(arguments, " ");
        strcat(arguments, in);
        
        free(in);
    }
}
