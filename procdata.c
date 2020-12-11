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

void setProcvirtualMemSize(ProcData* data, int virtualMemSize) {
    data->virtualMemSize = virtualMemSize;
}

void setUserName(ProcData* data, char* userName) {
    int lenUserName;

    lenUserName = strlen(userName);
    data->userName = (char*) calloc((lenUserName + 1), sizeof(char));
    if (data->userName == NULL) fatalsyserror(MEM_FAILED_ALLOCATION);
    strcpy(data->userName, userName);
}

void setTtyName(ProcData* data, char* ttyName) {
    int lenTtyName;

    lenTtyName = strlen(ttyName);
    data->ttyName = (char*) calloc((lenTtyName + 1), sizeof(char));
    if (data->ttyName == NULL) fatalsyserror(MEM_FAILED_ALLOCATION);
    strcpy(data->ttyName, ttyName);
}

void setTime(ProcData* data, int time) {
    data->secondes = time % 60;
    data->minutes = (int) time / 60;
}

void setProcDatas(ProcData* data, char* userName, char* pid, char* cmdline, char* statut, char* rss, float cpu, int virtualMemSize, char* ttyName, int time) {
    setProcPid(data, pid);
    setUserName(data, userName);
    setProcCmd(data, cmdline);
    setProcStatut(data, statut);
    setProcRss(data, rss);
    setProcCpuUsage(data, cpu);
    setProcvirtualMemSize(data, virtualMemSize);
    setTtyName(data, ttyName);
    setTime(data, time);
}

void afficherDetailsProcessus(ProcData* data) {
    char* message = "%s %s %s %s %s %.2f%% %d %s %d:%02d\n";

    printf(message,
                data->userName,
                data->pid,
                data->cmdline,
                data->statut,
                data->rss,
                data->cpu,
                data->virtualMemSize,
                data->ttyName,
                data->minutes,
                data->secondes
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
        free(list[i]->userName);
        free(list[i]->pid);
        free(list[i]->cmdline);
        free(list[i]->statut);
        free(list[i]->rss);
        free(list[i]->ttyName);
        free(list[i]);
    }
    free(list);
}