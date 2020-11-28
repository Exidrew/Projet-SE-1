#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>

#include "headers/error.h"
#include "headers/myps.h"

#define true 1
#define false 0

int estNombre(char* chaine) {
    int i;
    for (i=0; i < strlen(chaine); i++) {
        if (!isdigit(chaine[i])) return false;
    }
    return true;
}

void afficherArgumentMyPs(int argc, char* argv[]) {
    int i;
    printf("Myps lancé\n");
    printf("Avec les arguments :\n");
    for (i=0; i < argc; i++) printf("- %s\n", argv[i]);
}

char* getDetailsProcessus(DirEnt* directory, char* message) {
    int lenName = strlen(directory->d_name);
    message = (char*) realloc(message, (strlen(message) + lenName + 2) * sizeof(char));
    if (message == NULL) fatalsyserror(10);

    message = strcat(message, directory->d_name);
    message = strcat(message, "\n");
    return message;
}

int main(int argc, char* argv[]) {
    DIR* rep = null;
    DirEnt* directoryEntity;
    char* details;
    if ((details = (char*) calloc(10, sizeof(char))) == NULL) fatalsyserror(10);
    
    if ((rep = opendir(DIR_PROC)) == null) fatalsyserror(PS_FAIL_OPENDIR);

    while ((directoryEntity = readdir(rep)) != null) {
        if (estNombre(directoryEntity->d_name)) {
            details = getDetailsProcessus(directoryEntity, details);
        }
    }
    printf("%s", details);

    free(details);
    if (closedir(rep) == ERR) fatalsyserror(PS_FAIL_CLOSEDIR);

    return 0;
}