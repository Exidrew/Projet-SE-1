#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "headers/variables.h"
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

void ecrireVariableVersTube(int tubeDescriptor[2], var_locale* variables) {
    int size;
    int nbVariables = tailleChaine(variables);
    close(tubeDescriptor[0]);
    write(tubeDescriptor[1], &nbVariables, sizeof(int));
    while(variables != NULL && variables->nom != NULL) {
        size = strlen(variables->nom);
        write(tubeDescriptor[1], &size, sizeof(int));
        write(tubeDescriptor[1], variables->nom, size * sizeof(char) + 1);
        size = strlen(variables->valeur);
        write(tubeDescriptor[1], &size, sizeof(int));
        write(tubeDescriptor[1], variables->valeur, size * sizeof(char) + 1);
        variables = variables->suivant;
    }
    close(tubeDescriptor[1]);
}

void lireVariableDepuisTube(int tubeDescriptor[2]) {
    int size = 1, nbVariables = 0;
    close(tubeDescriptor[1]);
    read(tubeDescriptor[0], &nbVariables, sizeof(int));
    for (int i = 0; i < nbVariables; i++) {
        read(tubeDescriptor[0], &size, sizeof(int));
        char* nom = (char*) calloc(size, sizeof(char) + 1);
        if (nom == NULL) perror("Problème de pointeur pour le nom"), free(nom), exit(3);
        read(tubeDescriptor[0], nom, size * sizeof(char) + 1);
        read(tubeDescriptor[0], &size, sizeof(int));
        char* valeur = (char*) calloc(size, sizeof(char) + 1);
        if (nom == NULL) perror("Problème de pointeur pour la valeur"), free(valeur), exit(4);
        read(tubeDescriptor[0], valeur, size * sizeof(char) + 1);
        listeVariables = ajouter(listeVariables, nom, valeur);
        free(nom);
        free(valeur);
    }
    close(tubeDescriptor[0]);
}
