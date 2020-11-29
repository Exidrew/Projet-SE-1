#ifndef PROCDATA_H
#define PROCDATA_H

typedef struct procdata {
    char* pid;
    char* cmdline;
    char* statut;
    char* rss;
} ProcData;

// Set toutes les informations du processus
void setProcDatas(ProcData* data, char* pid, char* cmdline, char* statut, char* rss);

void afficherDetailsProcessus(ProcData* data);

void afficherTousLesProcessus(ProcData** list, int nbData);

void freeListProcData(ProcData** list, int nbData);

#endif