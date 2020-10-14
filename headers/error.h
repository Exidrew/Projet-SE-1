#ifndef DEF_ERRORH
#define DEF_ERRORH

#define ERR -1

#define syserror(x) perror(errormsg[x]);
#define fatalsyserror(x)\
syserror(x);\
exit(x);

char* errormsg[] = {
    "No error", // 0
    ROUGE("Imposible to fork process"), // 1
    ROUGE("Exec failed") // 2
};

#endif