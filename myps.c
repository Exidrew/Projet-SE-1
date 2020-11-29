#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>

#include "headers/error.h"
#include "headers/myps.h"
#include "headers/procdata.h"

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

char* getStatusPath(char* pid) {
    int procLen = strlen(DIR_PROC);
    int pidLen = strlen(pid);
    int statusLen = strlen(DIR_STATUS);
    // +4 car on rajoute deux "/" et strcar rajoute "\0"
    // Ca corrige une erreur valgrind
    int totalLen = procLen + pidLen + statusLen + 4;
    char* path = (char*) calloc(totalLen, sizeof(char));

    path = strcat(path, DIR_PROC);
    path = strcat(path, "/");
    path = strcat(path, pid);
    path = strcat(path, "/");
    path = strcat(path, DIR_STATUS);

    return path;
}

char* searchInFile(char* contain, int fileDescriptor) {
    char* line = (char*) calloc(2, sizeof(char));
    int lenLine, index = 0, i, j = 0, totalLen = 0;
    char car;
    char* theData;
    
    while (strncmp(line, contain, strlen(contain)) != 0) {
        lenLine = 0;
        lseek(fileDescriptor, totalLen, SEEK_SET);
        car = '\0';
        while (car != '\n' && read(fileDescriptor, &car, 1) != EOF) {
            line[lenLine] = car;
            line = (char*) realloc(line, (lenLine + 2) * sizeof(char));
            lenLine++;
            if (line == null) fatalsyserror(MEM_FAILED_ALLOCATION);
        }
        line[lenLine-1] = '\0';
        totalLen += lenLine;
    }

    // Suppression des espaces
    while (!isspace(line[index])) index++;
    index++;

    // Copie de la commande (pour retirer le prefixe)
    // Important pour free cmdline correctement
    theData = (char*) calloc(lenLine, sizeof(char));
    for(i=index; line[i]; i++,j++) {
        theData[j] = line[i];
    }

    lseek(fileDescriptor, 0, SEEK_SET);
    free(line);
    return theData;
}

void getDetailsProcessus(DirEnt* directory, ProcData* data) {
    int fileDescriptor;
    char* cmdline, *statut;
    char* path = getStatusPath(directory->d_name);

    if ((fileDescriptor = open(path, O_RDONLY)) == ERR) fatalsyserror(FILE_FAILED_OPEN);

    cmdline = searchInFile("Name:", fileDescriptor);
    statut = searchInFile("State:", fileDescriptor);

    if ((close(fileDescriptor)) == ERR) fatalsyserror(FILE_FAILED_CLOSE);

    setProcDatas(data, directory->d_name, cmdline, statut);

    free(cmdline), free(path), free(statut);
}

int main(int argc, char* argv[]) {
    DIR* rep = null;
    DirEnt* directoryEntity;
    ProcData** listProcData = (ProcData**) calloc(1, sizeof(ProcData*));
    int nbProcData = 0;

    if ((rep = opendir(DIR_PROC)) == null) fatalsyserror(PS_FAIL_OPENDIR);

    while ((directoryEntity = readdir(rep)) != null) {
        if (estNombre(directoryEntity->d_name)) {
            listProcData = (ProcData**) realloc(listProcData, (nbProcData + 2) * sizeof(ProcData*));
            listProcData[nbProcData] = (ProcData*) calloc(1, sizeof(ProcData));
            getDetailsProcessus(directoryEntity, listProcData[nbProcData]);
            nbProcData++;
        }
    }
    if (closedir(rep) == ERR) fatalsyserror(PS_FAIL_CLOSEDIR);

    afficherTousLesProcessus(listProcData, nbProcData);

    freeListProcData(listProcData, nbProcData);
    return 0;
}