#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "headers/cd.h"

int executerCd(char* commande, int nbCommandes) {
    printf("branche léo");
    int taille = strlen(commande);
    if(taille <= 3){
        return chdir("/home");
    } else {
        char *souscommande = commande + 3;
        return chdir(souscommande);
    }
}