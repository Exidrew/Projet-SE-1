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

int main(int argc, char* argv[]) {
    DIR* rep = null;
    DirEnt* directoryEntity = (DirEnt*) calloc(1, sizeof(DirEnt));
    
    if ((rep = opendir("/proc")) == null) fatalsyserror(PS_FAIL_OPENDIR);

    while ((directoryEntity = readdir(rep)) != null) {
        if (estNombre(directoryEntity->d_name)) {
            printf("Le fichier lu : %s\n", directoryEntity->d_name);
        }
    }

    if (closedir(rep) == ERR) fatalsyserror(PS_FAIL_CLOSEDIR);
    free(directoryEntity);

    return 0;
}