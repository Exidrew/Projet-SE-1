#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <pwd.h>
#include <linux/kdev_t.h>

#include "headers/error.h"
#include "headers/myps.h"
#include "headers/procdata.h"
#include "headers/redirections.h"

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

void getStatDesc(int fileDescriptorStat, int* tty, int *userTime, int* systemTime, int* cutime, int* cstime, int* startTime) {
    char* description = getNextLigne(fileDescriptorStat);

    sscanf(description, "%*d %*s %*c %*d %*d %*d %d %*d %*d %*d %*d %*d %*d %d %d"
                        "%d %d %*d %*d %*d %*d %d", tty, userTime, systemTime, cutime, cstime, startTime);

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

int getVSZ(int fileDescriptorStatus) {
    int virtualMemSize = 0;
    char* vsz = searchInFile("VmSize:", fileDescriptorStatus);

    if (strlen(vsz) > 0) {
        virtualMemSize = atoi(vsz);
    }
    
    free(vsz);
    return virtualMemSize;
}

int getUID(int fileDescriptorStatus) {
    int uid;
    char* ligne = searchInFile("Uid:", fileDescriptorStatus);
    
    sscanf(ligne, "%d %*d %*d %*d", &uid);

    free(ligne);
    return uid;
}

char* getUserName(uid_t uid) {
    char *name = (char*) calloc(strlen(getpwuid(uid)->pw_name) + 1, sizeof(char));
    name = strcpy(name, getpwuid(uid)->pw_name);
    return name;
}

char* getTtyName(int tty) {
    int isTty = isatty(tty), majeur = MAJOR(tty), mineur = MINOR(tty);
    int taille, i, ind = 0;
    char* name = (char*) calloc(20, sizeof(char));
    char* ttyName = (char*) calloc(20, sizeof(char));

    if (majeur || mineur) {
        if (majeur && mineur) {
            taille = snprintf(NULL, 0, "%s%d", "tty", mineur);
            snprintf(name, (taille + 1) * sizeof(char), "%s%d", "tty", mineur);
        } else {
            taille = snprintf(NULL, 0, "%s%d", "pts/", mineur);
            snprintf(name, (taille + 1) * sizeof(char), "%s%d", "pts/", mineur);
        }
    }
    else if (isTty) {
        strcpy(ttyName, ttyname(tty));
        taille = strlen("/dev/");
        for (i=taille; i < strlen(ttyName); i++) name[ind++] = ttyName[i];
    }
    else strcpy(name, "?");

    free(ttyName);
    return name;
}

int getTime(int userTime, int systemTime) {
    return (userTime + systemTime) / sysconf(_SC_CLK_TCK);
}

int getTotalMemory() {
    int fileDescriptor, memory;
    char* ligne;

    if ((fileDescriptor = open(DIR_MEMORY, O_RDONLY)) == ERR) fatalsyserror(FILE_FAILED_OPEN);
    ligne = searchInFile("MemTotal:", fileDescriptor);
    if (close(fileDescriptor) == ERR) fatalsyserror(FILE_FAILED_CLOSE);

    sscanf(ligne, "%d %*s", &memory);

    free(ligne);
    return memory;
}

void getDetailsProcessus(DirEnt* directory, ProcData* data, Time bootTime, int totalMemory) {
    int fileDescriptorStatus, fileDescriptorProcStat, fileDescriptorStat;
    char* cmdline, *statut, *rss, *userName, *ttyName;
    int userTime, systemTime, cutime, cstime, startTime, virtualMemSize, tty;
    int time;
    uid_t uid;
    float pourcentageCPU = 0;
    char* statusPath = getPath(directory->d_name, DIR_STATUS);
    char* statPath = getPath(directory->d_name, DIR_STAT);

    if ((fileDescriptorStatus = open(statusPath, O_RDONLY)) == ERR) fatalsyserror(FILE_FAILED_OPEN);
    if ((fileDescriptorProcStat = open(DIR_PROC_STAT, O_RDONLY)) == ERR) fatalsyserror(FILE_FAILED_OPEN);
    if ((fileDescriptorStat = open(statPath, O_RDONLY)) == ERR) fatalsyserror(FILE_FAILED_OPEN);

    uid = getUID(fileDescriptorStatus);
    userName = getUserName(uid);
    cmdline = searchInFile("Name:", fileDescriptorStatus);
    statut = searchInFile("State:", fileDescriptorStatus);
    virtualMemSize = getVSZ(fileDescriptorStatus);
    rss = getRss(fileDescriptorStatus);
    getStatDesc(fileDescriptorStat, &tty, &userTime, &systemTime, &cutime, &cstime, &startTime);

    if (close(fileDescriptorStatus) == ERR) fatalsyserror(FILE_FAILED_CLOSE);
    if (close(fileDescriptorProcStat) == ERR) fatalsyserror(FILE_FAILED_CLOSE);
    if (close(fileDescriptorStat) == ERR) fatalsyserror(FILE_FAILED_CLOSE);

    ttyName = getTtyName(tty);
    pourcentageCPU = getPourcentageCPU((float)startTime, userTime);
    time = getTime(userTime, systemTime);
    statut[1] = '\0';
    setProcDatas(data, userName, bootTime, totalMemory, directory->d_name, cmdline, statut, rss, pourcentageCPU, virtualMemSize, ttyName, time, startTime);
    free(cmdline), free(statusPath), free(statPath), free(statut), free(rss);
    free(userName), free(ttyName);
}

int main(int argc, char* argv[]) {
    DIR* rep = null;
    DirEnt* directoryEntity;
    ProcData** listProcData = (ProcData**) calloc(1, sizeof(ProcData*));
    int nbProcData = 0, totalMemory, tailleMessage = 0, tailleActuelle = 1;
    Time bootTime;
    char* in = (char*) calloc(1, sizeof(char));
    char car = '\0';
    char* arguments = (char*) calloc(1, sizeof(char));

    if (argc-1 > 0 && !strncmp(argv[argc-1], "l", strlen("l"))) {
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
    }

    if (contientRedirection(argv[0])) gererRedirection(argv[0]);
    
    clock_gettime(CLOCK_MONOTONIC, &bootTime);
    totalMemory = getTotalMemory();

    if ((rep = opendir(DIR_PROC)) == null) fatalsyserror(PS_FAIL_OPENDIR);

    while ((directoryEntity = readdir(rep)) != null) {
        if (estNombre(directoryEntity->d_name)) {
            listProcData = (ProcData**) realloc(listProcData, (nbProcData + 2) * sizeof(ProcData*));
            listProcData[nbProcData] = (ProcData*) calloc(1, sizeof(ProcData));
            getDetailsProcessus(directoryEntity, listProcData[nbProcData], bootTime, totalMemory);
            nbProcData++;
        }
    }
    if (closedir(rep) == ERR) fatalsyserror(PS_FAIL_CLOSEDIR);

    afficherTousLesProcessus(listProcData, nbProcData);

    freeListProcData(listProcData, nbProcData);
    free(in);
    free(arguments);
    return 0;
}