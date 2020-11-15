#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/variables.h"

#define TRUE 1
#define FALSE 0

int existe(TableauVariables* variables, char* nom) {
    int i;
    Variables* var = variables->variables;
    for (i = 0; i < variables->nbVar; i++) {
        if (!strcmp(var[i].nom, nom)) {
            return i;
        }
    }
    return FALSE;
}

void ajouterVariable(TableauVariables* variables, char* nom, char* valeur) {
    int i, ind;
    if ((ind = existe(variables, nom))) {
        for (i = 0; i < strlen(valeur); i++) variables->variables[ind].valeur[i] = valeur[i];
        return;
    }

    Variables* nouvelle = (Variables*) calloc(1024, sizeof(Variables));
    for (i = 0; i < strlen(nom); i++) nouvelle->nom[i] = nom[i];
    for (i = 0; i < strlen(valeur); i++) nouvelle->valeur[i] = valeur[i];

    variables->variables[variables->nbVar++] = *nouvelle;
}

void afficherVariables(TableauVariables* variables) {
    int i;
    Variables* var = variables->variables;
    for (i=0; i < variables->nbVar; i++) {
        printf("- %s : %s\n", var[i].nom, var[i].valeur);
    }
}

void supprimerVariable(TableauVariables* variables, char* nom) {
    int i;
    Variables* var = variables->variables;
    for(i = 0; i < variables->nbVar; i++) {
        if (!strcmp(var[i].nom, nom)) {
            var[i] = var[variables->nbVar-1];
            variables->nbVar--;
        }
    }
}

void freeVariables(TableauVariables* variables) {
    free(variables->variables);
    free(variables);
}

// int main() {
//     initialiserTableauVariable(variables, 1024);
//     ajouterVariable(variables, "var", "5");
//     ajouterVariable(variables, "truc", "2");
//     ajouterVariable(variables, "truc", "0");
//     supprimerVariable(variables, "truc");

//     afficherVariables(variables);
//     freeVariables(variables);
//     return 0;
// }