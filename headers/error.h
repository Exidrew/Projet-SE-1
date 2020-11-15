#ifndef DEF_ERRORH
#define DEF_ERRORH

#define ERR -1

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

char* errormsg[] = {
    "No error", // 0
    ROUGE("Imposible to fork process"), // 1
    ROUGE("Exec failed"), // 2
    ROUGE("Shared Memory key creation failed"), //3
    ROUGE("Shared Memory creation failed"), // 4
    ROUGE("Failed to attach pointer to shared memory"), //5
    ROUGE("Shared Memory detachment failed"), // 6
    ROUGE("Shared Memory deletion failed") // 7
};

#endif