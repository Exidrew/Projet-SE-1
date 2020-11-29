#ifndef PROCDATA_H
#define PROCDATA_H

typedef struct procdata {
    char* pid;
    char* cmdline;
    char* statut;
} ProcData;

// Set toutes les informations du processus
void setProcDatas(ProcData* data, char* pid, char* cmdline, char* statut);

void afficherDetailsProcessus(ProcData* data);

#endif