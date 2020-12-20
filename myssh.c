#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <regex.h>
#include <string.h>

#include "headers/error.h"
#include "headers/client.h"
#include "headers/myssh.h"
#include "headers/gestionChaine.h"

/* ============= Fonctions privées ============= */

int checkIdentification(char* id) {
    regex_t regex;
    const char schema[43] = "^[a-zA-Z0-9_]+@([0-9]{1,3}.){3}[0-9]{1,3}$";

    if (regcomp(&regex, schema, REG_EXTENDED)) {
        regfree(&regex);
        fatalsyserror(8);
    }
    
    if (regexec(&regex, id, 0, NULL, 0)) {
        printf("Invalid User/password for host1\n");
        regfree(&regex);
        return 0;
    }
    regfree(&regex);
    return 1;
}

int checkArguments(int nbArgs) {
    return nbArgs == 2;
}

void printUsage() {
    printf("Usage : myssh username@host\n");
    exit(0);
}

int main(int argc, char* argv[]) {
    char* password = calloc(MAXIMUM, sizeof(char));
    if (!checkArguments(argc)) printUsage();
    if (!checkIdentification(argv[1])) printUsage();

    prompt(password, "Password: ");

    printf("Le password : %s\n", password);
    printf("Test\n");
    lancerClient();

    free(password);
    return 0;
}