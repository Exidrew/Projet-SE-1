#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/error.h"
#include "headers/procdata.h"

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

void setProcRss(ProcData* data, char* rss) {
    int lenRss;

    lenRss = strlen(rss);
    data->rss = (char*) calloc((lenRss + 1), sizeof(char));
    if (data->rss == NULL) fatalsyserror(MEM_FAILED_ALLOCATION);
    strcpy(data->rss, rss);
}

void setProcCpuUsage(ProcData* data, float cpu) {
    data->cpu = cpu;
}

void setProcDatas(ProcData* data, char* pid, char* cmdline, char* statut, char* rss, float cpu) {
    setProcPid(data, pid);
    setProcCmd(data, cmdline);
    setProcStatut(data, statut);
    setProcRss(data, rss);
    setProcCpuUsage(data, cpu);
}

void afficherDetailsProcessus(ProcData* data) {
    char* message = "%s %s %s %s %.2f%%\n";

    printf(message,
                data->pid,
                data->cmdline,
                data->statut,
                data->rss,
                data->cpu
    );
}

void afficherTousLesProcessus(ProcData** list, int nbData) {
    int i;
    if (list != NULL) {
        for (i=0; i < nbData; i++) afficherDetailsProcessus(list[i]);
    }
}

void freeListProcData(ProcData** list, int nbData) {
    int i;

    for (i=0; i < nbData; i++){
        free(list[i]->pid);
        free(list[i]->cmdline);
        free(list[i]->statut);
        free(list[i]->rss);
        free(list[i]);
    }
    free(list);
}