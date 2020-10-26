#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "headers/variables.h"


int setVariable(char** tabcmd) {
    for (int i = 0; tabcmd[i] != NULL; i++) {
        printf("La commande : %s\n", tabcmd[i]);
    }
    

    return 0;
}