#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "headers/error.h"
#include "headers/redirections.h"

FILE* fd = NULL;
int id = ERR;

int recupererTypeRedirection(char* commande, char redirection[5]) {
    int i, nbEspaces = 0, j = 0, taille = strlen(commande);
    for (i = taille; i > 0 && nbEspaces < 2; i--) {
        if (isspace(commande[i])) nbEspaces++;
    }
    i++;
    nbEspaces = 0;
    for (i = i; i < taille; i++) {
        if (nbEspaces >= 2) break;
        redirection[j++] = commande[i];
        if (isspace(commande[i])) nbEspaces++;
    }

    return i;
}

void gererSortieVersFichier(char* commande, char* nomFichier, int flags, int mode, int sortie) {
    if ((id = open(nomFichier, flags, mode)) == ERR) {
        if ((id = open(nomFichier, O_CREAT | O_WRONLY, mode)) == ERR) fatalsyserror(REDIRECTION_FAILED);
    }
    if (close(sortie) == ERR) fatalsyserror(REDIRECTION_FAILED);
    if ((dup(id)) == ERR) fatalsyserror(REDIRECTION_FAILED);
}

void fermerFichierSortie() {
    fflush(stdout);
    if (close(id) == ERR) fatalsyserror(REDIRECTION_FAILED);
}

char* recupererNomFichier(char* commande) {
    char* nomFichier = calloc(strlen(commande) + 1, sizeof(char));
    if ((nomFichier = strcpy(nomFichier, commande)) == NULL) fatalsyserror(REDIRECTION_FAILED);
    return nomFichier;
}

void gererRedirection(char* commande) {
    char redirection[5];
    int type;
    int sortie;
    int indexNomFichier;
    char* nomFichier;
    int flags, mode = S_IRWXU;

    indexNomFichier = recupererTypeRedirection(commande, redirection);
    nomFichier = recupererNomFichier(commande + indexNomFichier);
    
    if (!strcmp(redirection, STDOUT_ERASE)) flags = O_CREAT | O_WRONLY, type = SORTIE_FICHIER_ERASE, sortie = STDOUT_FILENO;
    else if (!strcmp(redirection, STDOUT_APPEND)) flags = O_APPEND | O_WRONLY, type = SORTIE_FICHIER_APPEND, sortie = STDOUT_FILENO;
    if (!strcmp(redirection, STDERR_ERASE)) flags = O_CREAT | O_WRONLY, type = SORTIE_FICHIER_ERASE, sortie = STDERR_FILENO;
    else if (!strcmp(redirection, STDERR_ERASE)) flags = O_APPEND | O_WRONLY, type = SORTIE_FICHIER_APPEND, sortie = STDERR_FILENO;

    switch(type) {
        case SORTIE_FICHIER_ERASE:
        case SORTIE_FICHIER_APPEND:
        case SORTIE_ERREUR_ERASE:
        case SORTIE_ERREUR_APPEND:
            gererSortieVersFichier(commande, nomFichier, flags, mode, sortie);
            break;
    }

    free(nomFichier);
}

int contientRedirection(char* commande) {
    regex_t regex;
    const char schema[35] = " > | >> | 2> | 2>> | >& | >>& | < ";
    if (regcomp(&regex, schema, REG_EXTENDED)) {
        regfree(&regex);
        fatalsyserror(8);
    }

    return !regexec(&regex, commande, 0, NULL, 0);
}

int mettreEnBackground(char* commande) {
    regex_t regex;
    const char schema[6] = " & *$";
    if (regcomp(&regex, schema, REG_EXTENDED)) {
        regfree(&regex);
        fatalsyserror(8);
    }

    return !regexec(&regex, commande, 0, NULL, 0);
}