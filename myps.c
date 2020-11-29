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

void setProcDatas(ProcData* data, char* pid, char* cmdline) {
    int lenPID, lenCmdLine;

    lenPID = strlen(pid);
    data->pid = (char*) calloc((lenPID + 1), sizeof(char));
    if (data->pid == null) fatalsyserror(MEM_FAILED_ALLOCATION);
    strcpy(data->pid, pid);

    lenCmdLine = strlen(cmdline);
    data->cmdline = (char*) calloc((lenCmdLine + 1), sizeof(char));
    if (data->cmdline == null) fatalsyserror(MEM_FAILED_ALLOCATION);
    strcpy(data->cmdline, cmdline);
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

    free(line);
    return theData;
}



void getDetailsProcessus(DirEnt* directory, ProcData* data) {
    int fileDescriptor;
    char* cmdline;
    char* path = getStatusPath(directory->d_name);

    if ((fileDescriptor = open(path, O_RDONLY)) == ERR) fatalsyserror(FILE_FAILED_OPEN);

    cmdline = searchInFile("Name:", fileDescriptor);

    if ((close(fileDescriptor)) == ERR) fatalsyserror(FILE_FAILED_CLOSE);

    setProcDatas(data, directory->d_name, cmdline);

    free(cmdline);
    free(path);
}

void freeListProcData(ProcData** list, int nbData) {
    int i;
    // Commence à 1 car on commence à allouer à 1 dans le main
    for (i=1; i < nbData; i++){
        free(list[i]->pid);
        free(list[i]->cmdline);
        free(list[i]);
    }
    free(list);
}

int main(int argc, char* argv[]) {
    DIR* rep = null;
    DirEnt* directoryEntity;
    ProcData** listProcData = (ProcData**) calloc(1, sizeof(ProcData*));
    int nbProcData = 1;

    if ((rep = opendir(DIR_PROC)) == null) fatalsyserror(PS_FAIL_OPENDIR);

    while ((directoryEntity = readdir(rep)) != null) {
        if (estNombre(directoryEntity->d_name)) {
            listProcData = (ProcData**) realloc(listProcData, (nbProcData + 1) * sizeof(ProcData*));
            listProcData[nbProcData] = (ProcData*) calloc(1, sizeof(ProcData));
            getDetailsProcessus(directoryEntity, listProcData[nbProcData]);
            printf("%s %s\n", listProcData[nbProcData]->pid, listProcData[nbProcData]->cmdline);
            nbProcData++;
        }
    }
    if (closedir(rep) == ERR) fatalsyserror(PS_FAIL_CLOSEDIR);
    freeListProcData(listProcData, nbProcData);
    return 0;
}