#ifndef PROCDATA_H
#define PROCDATA_H

typedef struct procdata {
    char* userName;
    char* pid;
    char* cmdline;
    char* statut;
    char* rss;
    char* ttyName;
    float cpu;
    int virtualMemSize;
    int minutes;
    int secondes;
} ProcData;

// Set toutes les informations du processus
void setProcDatas(ProcData* data, char* userName, char* pid, char* cmdline, char* statut, char* rss, float cpu, int vmz, char* ttyName, int time);

void afficherDetailsProcessus(ProcData* data);

void afficherTousLesProcessus(ProcData** list, int nbData);

void freeListProcData(ProcData** list, int nbData);

#endif