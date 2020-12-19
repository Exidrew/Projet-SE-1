#ifndef PROCDATA_H
#define PROCDATA_H

typedef struct timespec Time;

typedef struct procdata {
    char* userName;
    char* pid;
    char* cmdline;
    char* statut;
    char* rss;
    char* ttyName;
    float cpu;
    float memUsage;
    int virtualMemSize;
    int minutes;
    int secondes;
    int startTimeMinutes;
    int startTimeHeures;
} ProcData;

// Set toutes les informations du processus
void setProcDatas(ProcData* data, char* userName, Time bootTime, int totalMemory, char* pid, char* cmdline, char* statut, char* rss, float cpu, int vmz, char* ttyName, int time, int startTime);

void afficherDetailsProcessus(ProcData* data);

void afficherTousLesProcessus(ProcData** list, int nbData);

void freeListProcData(ProcData** list, int nbData);

#endif