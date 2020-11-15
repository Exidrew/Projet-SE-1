#ifndef MEMOIRE_PARTAGEE_H
#define MEMOIRE_PARTAGEE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <assert.h>

#include "error.h"

#define NAME_FILE "/bin/sh"

#define allouerMemoirePartagee(clef, id, num, structure, pointeur, taille)\
    if ((clef = ftok(NAME_FILE,num))==ERR) fatalsyserror(SHM_KCREATION_FAIL);\
	if ((id = shmget(clef, taille * sizeof(structure), IPC_CREAT | 0600))==ERR) fatalsyserror(SHM_CREATION_FAIL);\
	if ((pointeur = shmat(id,NULL,0))==(structure*)ERR) fatalsyserror(SHM_ATTACH_FAIL);\

#define detruireMemoirePartagee(id, pointeur)\
    if (shmdt(pointeur)==ERR) fatalsyserror(SHM_DETACH_FAIL);\
	if (shmctl(id,IPC_RMID,NULL)==ERR) fatalsyserror(SHM_DELETE_FAIL);\

#endif