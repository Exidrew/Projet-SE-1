#ifndef MYPS_H
#define MYPS_H

#define null NULL
#define DIR_PROC "/proc"
#define DIR_STATUS "status"

typedef struct procdata {
    char* pid;
    char* cmdline;
} ProcData;

typedef struct dirent DirEnt;

#endif