#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

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

void getDiffTime(Time a, Time b, Time* result) {
    do {
        result->tv_sec = a.tv_sec - b.tv_sec;
    } while (0);
}

void setStartTime(ProcData* data, int startTime, Time bootTime) {
    int heures, minutes;
    Time start, result;
    time_t timeRaw;

    startTime = startTime / sysconf(_SC_CLK_TCK);
    start.tv_sec = startTime;

    timeRaw = time(NULL);

    getDiffTime(bootTime, start, &result);

    heures = localtime(&timeRaw)->tm_hour - (result.tv_sec / 3600);
    minutes = localtime(&timeRaw)->tm_min - (result.tv_sec % 60);

    data->startTimeHeures = heures;
    data->startTimeMinutes = abs(minutes);
}

void setProcMemUsage(ProcData* data, char* rss, int totalMemory) {
    float pourcentage = 0;
    int mem = atoi(rss);

    pourcentage = (mem / (float) totalMemory);

    data->memUsage = pourcentage;
}

void setProcDatas(ProcData* data, char* userName, Time bootTime, int totalMemory, char* pid, char* cmdline, char* statut, char* rss, float cpu, int virtualMemSize, char* ttyName, int time, int startTime) {
    setProcPid(data, pid);
    setUserName(data, userName);
    setProcCmd(data, cmdline);
    setProcStatut(data, statut);
    setProcRss(data, rss);
    setProcCpuUsage(data, cpu);
    setProcMemUsage(data, rss, totalMemory);
    setProcvirtualMemSize(data, virtualMemSize);
    setTtyName(data, ttyName);
    setTime(data, time);
    setStartTime(data, startTime, bootTime);
}

void afficherDetailsProcessus(ProcData* data) {
    char* message = "%s\t\t\t%s\t%.2f\t%.2f\t%d\t%s\t%s\t%s\t\t%d:%2d\t%d:%2d\t%s\n";

    printf(message,
                data->userName,
                data->pid,
                data->cpu,
                data->memUsage,
                data->virtualMemSize,
                data->rss,
                data->ttyName,
                data->statut,
                data->startTimeHeures,
                data->startTimeMinutes,
                data->minutes,
                data->secondes,
                data->cmdline
    );
}

void afficherTousLesProcessus(ProcData** list, int nbData) {
    int i;
    printf("USER\t\t\tPID\t%%CPU\t%%MEM\tVSZ\tRSS\tTYY\tSTAT\t\t\tSTART\tTIME\tCOMMAND\n");
    if (list != NULL) {
        for (i=0; i < nbData; i++) afficherDetailsProcessus(list[i]);
    }
    printf("\n");
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