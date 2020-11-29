#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/error.h"
#include "headers/procdata.h"


//#####################################################################//
//                          Fonctions privées                          //
//#####################################################################//

void setProcPid(ProcData* data, char* pid) {
    int lenPID;

    lenPID = strlen(pid);
    data->pid = (char*) calloc((lenPID + 1), sizeof(char));
    if (data->pid == NULL) fatalsyserror(MEM_FAILED_ALLOCATION);
    strcpy(data->pid, pid);
}

void setProcCmd(ProcData* data, char* cmdline) {
    int lenCmdLine;

    lenCmdLine = strlen(cmdline);
    data->cmdline = (char*) calloc((lenCmdLine + 1), sizeof(char));
    if (data->cmdline == NULL) fatalsyserror(MEM_FAILED_ALLOCATION);
    strcpy(data->cmdline, cmdline);
}

void setProcStatut(ProcData* data, char* statut) {
    int lenStatut;

    lenStatut = strlen(statut);
    data->statut = (char*) calloc((lenStatut + 1), sizeof(char));
    if (data->statut == NULL) fatalsyserror(MEM_FAILED_ALLOCATION);
    strcpy(data->statut, statut);
}

//#####################################################################//
//                          Fonctions publiques                        //
//#####################################################################//

void setProcDatas(ProcData* data, char* pid, char* cmdline, char* statut) {
    setProcPid(data, pid);
    setProcCmd(data, cmdline);
    setProcStatut(data, statut);
}

void afficherDetailsProcessus(ProcData* data) {
    char* message = "%s %s %s\n";
    printf(message, data->pid, data->cmdline, data->statut);
}