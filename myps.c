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

char* getCmdLine(char* pid) {
    int fileDescriptor, index = 0, lenCmdLine = 0, i,j=0;
    char* path;
    path = getStatusPath(pid);
    char car = '\0';
    char* cmdline = (char*) calloc(2, sizeof(char));
    char* cmd;

    if ((fileDescriptor = open(path, O_RDONLY)) == ERR) fatalsyserror(FILE_FAILED_OPEN);

    while (car != '\n' && read(fileDescriptor, &car, 1) != EOF) {
        cmdline[lenCmdLine] = car;
        cmdline = (char*) realloc(cmdline, (lenCmdLine + 2) * sizeof(char));
        lenCmdLine++;
        if (cmdline == null) fatalsyserror(MEM_FAILED_ALLOCATION);
    }
    cmdline[lenCmdLine-1] = '\0';

    // Suppression des espaces
    while (!isspace(cmdline[index])) index++;
    index++;

    // Copie de la commande (pour retirer le "name: ")
    // Important pour free cmdline correctement
    cmd = (char*) calloc(lenCmdLine, sizeof(char));
    for(i=index; cmdline[i]; i++,j++) {
        cmd[j] = cmdline[i];
    }

    if ((close(fileDescriptor)) == ERR) fatalsyserror(FILE_FAILED_CLOSE);
    free(path);
    free(cmdline);
    return cmd;
}

void getDetailsProcessus(DirEnt* directory, ProcData* data) {
    char* cmdline = getCmdLine(directory->d_name);
    int lenPID = strlen(directory->d_name);
    int lencmdline = strlen(cmdline);
    data->pid = (char*) calloc((lenPID + 1), sizeof(char));
    data->cmdline = (char*) calloc((lencmdline + 1), sizeof(char));

    if (data->pid == null || data->cmdline == null) fatalsyserror(MEM_FAILED_ALLOCATION);
    strcpy(data->pid, directory->d_name);
    strcpy(data->cmdline, cmdline);

    free(cmdline);
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