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

char* getDetailsProcessus(DirEnt* directory, char* message) {
    int lenPID = strlen(directory->d_name);
    char* cmdline = getCmdLine(directory->d_name);
    int totalLen = strlen(message) + lenPID + strlen(cmdline) + 4;
    message = (char*) realloc(message, totalLen * sizeof(char));
    if (message == null) fatalsyserror(MEM_FAILED_ALLOCATION);

    message = strcat(message, directory->d_name);
    message = strcat(message, " ");
    message = strcat(message, cmdline);
    message = strcat(message, "\n");

    free(cmdline);
    return message;
}

int main(int argc, char* argv[]) {
    DIR* rep = null;
    DirEnt* directoryEntity;
    char* details;
    if ((details = (char*) calloc(10, sizeof(char))) == NULL) fatalsyserror(MEM_FAILED_ALLOCATION);
    
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