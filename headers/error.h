#ifndef DEF_ERRORH
#define DEF_ERRORH

#define ERR -1

#define ROUGE(message) "\033[01;31m"message"\033[0m"
#define BLEU(m) "\033[01;34m"m"\033[0m"
#define VERT(message) "\033[01;32m"message"\033[0m"

#define syserror(x) perror(errormsg[x]);
#define fatalsyserror(x) {\
    syserror(x);\
    exit(x);\
}

#define NO_ERROR 0
#define FORK_FAIL 1
#define EXEC_FAIL 2
#define SHM_KCREATION_FAIL 3
#define SHM_CREATION_FAIL 4
#define SHM_ATTACH_FAIL 5
#define SHM_DETACH_FAIL 6
#define SHM_DELETE_FAIL 7
#define PS_FAIL_OPENDIR 8
#define PS_FAIL_CLOSEDIR 9

extern char* errormsg[];

#endif