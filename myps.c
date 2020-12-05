#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

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

char* getPath(char* pid, char* but) {
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
    path = strcat(path, but);

    return path;
}

char* retirerKb(char* rss) {
    int i,j = 0;
    // -3 car on retire " kb"
    char* new = (char*) calloc(strlen(rss)-3, sizeof(char));

    for (i=0; isspace(rss[i]); i++);
    for (i=i; !isspace(rss[i]);i++) {
        new[j++] = rss[i];
    }
    new[j] = '\0';

    rss = strcpy(rss, new);
    free(new);
    return rss;
}

char* searchInFile(char* contain, int fileDescriptor) {
    char* line = (char*) calloc(2, sizeof(char));
    int lenLine, index = 0, i, j = 0, totalLen = 0;
    char car;
    char* theData;
    ssize_t fin;
    
    while (strncmp(line, contain, strlen(contain)) != 0) {
        lenLine = 0;
        lseek(fileDescriptor, totalLen, SEEK_SET);
        car = '\0';
        while (car != '\n' && (fin = read(fileDescriptor, &car, 1)) != EOF) {
            line[lenLine] = car;
            line = (char*) realloc(line, (lenLine + 2) * sizeof(char));
            lenLine++;
            if (line == null) fatalsyserror(MEM_FAILED_ALLOCATION);
            if (car == '\0') break;
        }
        if (car == '\0') {
            lseek(fileDescriptor, 0, SEEK_SET);
            free(line);
            theData = (char*) calloc(5, sizeof(char));
            return theData;
        };
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

char* getRss(int fileDescriptor) {
    char* rss;
    rss = searchInFile("VmRSS:", fileDescriptor);
    if (!strlen(rss)) strcpy(rss, "0");
    else rss = retirerKb(rss);
    return rss;
}

char* getNextLigne(int fileDescriptor) {
    char* ligne = (char*) calloc(2, sizeof(char)), car = '\0';
    int lenLigne = 0;

    while (car != '\n' && read(fileDescriptor, &car, 1) != EOF) {
        ligne[lenLigne] = car;
        ligne = (char*) realloc(ligne, (lenLigne + 2) * sizeof(char));
        lenLigne++;
        if (ligne == null) fatalsyserror(MEM_FAILED_ALLOCATION);
        if (car == '\0') break;
    }
    ligne[lenLigne-1] = '\0';
    return ligne;
}

void getTimes(int fileDescriptorStat, int *userTime, int* systemTime, int* cutime, int* cstime, int* startTime) {
    char* description = getNextLigne(fileDescriptorStat);

    sscanf(description, "%*d %*s %*c %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %d %d"
                        "%d %d %*d %*d %*d %*d %d", userTime, systemTime, cutime, cstime, startTime);

    lseek(fileDescriptorStat, 0, SEEK_SET);
    free(description);
}

float getPourcentageCPU(float startTime, float usageTime) {
    float pourcentage;
    int secondes;
    struct timespec spec;
    clock_gettime(CLOCK_BOOTTIME, &spec);

    secondes = (spec.tv_sec - (startTime / sysconf(_SC_CLK_TCK))) / sysconf(_SC_CLK_TCK);

    if (secondes == 0) return 0.0;

    pourcentage = (((usageTime / sysconf(_SC_CLK_TCK)) * 1000) / sysconf(_SC_CLK_TCK)) / secondes;

    return pourcentage / 10;
}

void getDetailsProcessus(DirEnt* directory, ProcData* data) {
    int fileDescriptorStatus, fileDescriptorProcStat, fileDescriptorStat;
    char* cmdline, *statut, *rss;
    int userTime, systemTime, cutime, cstime, startTime;
    float pourcentageCPU = 0;
    char* statusPath = getPath(directory->d_name, DIR_STATUS);
    char* statPath = getPath(directory->d_name, DIR_STAT);

    if ((fileDescriptorStatus = open(statusPath, O_RDONLY)) == ERR) fatalsyserror(FILE_FAILED_OPEN);
    if ((fileDescriptorProcStat = open(DIR_PROC_STAT, O_RDONLY)) == ERR) fatalsyserror(FILE_FAILED_OPEN);
    if ((fileDescriptorStat = open(statPath, O_RDONLY)) == ERR) fatalsyserror(FILE_FAILED_OPEN);

    cmdline = searchInFile("Name:", fileDescriptorStatus);
    statut = searchInFile("State:", fileDescriptorStatus);
    rss = getRss(fileDescriptorStatus);
    getTimes(fileDescriptorStat, &userTime, &systemTime, &cutime, &cstime, &startTime);


    if ((close(fileDescriptorStatus)) == ERR) fatalsyserror(FILE_FAILED_CLOSE);
    if ((close(fileDescriptorProcStat)) == ERR) fatalsyserror(FILE_FAILED_CLOSE);
    if ((close(fileDescriptorStat)) == ERR) fatalsyserror(FILE_FAILED_CLOSE);

    pourcentageCPU = getPourcentageCPU((float)startTime, userTime);
    setProcDatas(data, directory->d_name, cmdline, statut, rss, pourcentageCPU);

    free(cmdline), free(statusPath), free(statPath), free(statut), free(rss);
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