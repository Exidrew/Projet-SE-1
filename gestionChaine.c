#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <regex.h>

#include "headers/error.h"
#include "headers/gestionChaine.h"

int doitRetirerEspace(char* commande) {
    return (strncmp(commande, "test", strlen("test")));
}

char** gererChaine(char commande[sizelgcmd], char** commandeSansEspaces, int* nbCommandes) {
    int i, nbEspace = 0, j = 0, indexCommandes = 0;
    for (i = 0; i < strlen(commande); i++) {
        if (commande[i] == '\n') continue;
        if (commande[i] == ';') {
            indexCommandes++;
            nbEspace = 0;
            if (i+1 < strlen(commande) && isspace(commande[i+1]))
                i++;
            j = 0;
            continue;
        }
        if (!strncmp(commande + i, "||", 2) || !strncmp(commande + i, "&&", 2)) {
            indexCommandes++;
            if (commande[i] == '|') commandeSansEspaces[indexCommandes] = strcpy(commandeSansEspaces[indexCommandes], "||");
            else if (commande[i] == '&') commandeSansEspaces[indexCommandes] = strcpy(commandeSansEspaces[indexCommandes], "&&");
            indexCommandes++;
            i++;
            nbEspace = 0;
            if (i+1 < strlen(commande) && isspace(commande[i+1]))
                i++;
            j = 0;
            continue;
        }
        else if (!strncmp(commande + i,"|",1) && (i+1 < strlen(commande) && commande[i+1] != '|')) {
            // Retire un espace très gênant
            commandeSansEspaces[indexCommandes][i-1] = '\0';
            indexCommandes++;
            commandeSansEspaces[indexCommandes] = strcpy(commandeSansEspaces[indexCommandes], "|");
            indexCommandes++;
            nbEspace++;
            i++;
            if (i+1 < strlen(commande) && isspace(commande[i+1]))
                i++;
            j = 0;

            continue;
        }
        if (!strncmp(commande + i, " > ", 3)) {
            strcat(commandeSansEspaces[indexCommandes], " > ");
            i += 2;
            j += 3;
            continue;
        } else if (!strncmp(commande + i, " >> ", 4)) {
            strcat(commandeSansEspaces[indexCommandes], " >> ");
            i += 3;
            j += 4;
            continue;
        } else if (!strncmp(commande + i, " 2> ", 4)) {
            strcat(commandeSansEspaces[indexCommandes], " 2> ");
            i += 3;
            j += 4;
            continue;
        } else if (!strncmp(commande + i, " 2>> ", 5)) {
            strcat(commandeSansEspaces[indexCommandes], " 2>> ");
            i += 4;
            j += 5;
            continue;
        } else if (!strncmp(commande + i, " >& ", 4)) {
            strcat(commandeSansEspaces[indexCommandes], " >& ");
            i += 3;
            j += 4;
            continue;
        } else if (!strncmp(commande + i, " >>& ", 5)) {
            strcat(commandeSansEspaces[indexCommandes], " >>& ");
            i += 4;
            j += 5;
            continue;
        } else if (!strncmp(commande + i, " < ", 3)) {
            strcat(commandeSansEspaces[indexCommandes], " < ");
            i += 2;
            j += 3;
            continue;
        }
        if (nbEspace < 1) {
            if (!strncmp(commande + i, " ||", 3) || !strncmp(commande + i, "||", 2) || !strncmp(commande + i, "|| ", 3)) continue;
            else if (!strncmp(commande + i, " &&", 3) || !strncmp(commande + i, "&&", 2) || !strncmp(commande + i, "&& ", 3)) continue;
            else if (commande[i] == '|') {
                continue;
            }
            else {
                commandeSansEspaces[indexCommandes][j] = commande[i];
                j++;
                if (isspace(commande[i]) && doitRetirerEspace(commandeSansEspaces[indexCommandes])) nbEspace++;
            }
        }
        else {
            if (!isspace(commande[i])) {
                commandeSansEspaces[indexCommandes][j] = commande[i];
                j++;
            }
            else if (doitRetirerEspace(commandeSansEspaces[indexCommandes])) {
                nbEspace++;
            }
        }
    }

    *nbCommandes = indexCommandes+1; // +1 car on commence l'index à 0
    return commandeSansEspaces;
}

void affichageLigneShell(){
    char repertory[100];
    char hostName[100];
    getPwd(repertory);
    getComputerName(hostName);
    
    printf(VERT("%s"), hostName);
    putchar(':');
    printf(BLEU("%s"), repertory);
    putchar('$');
    putchar(' ');
}

int getPwd(char *repertory){
    if(getcwd(repertory, UCHAR_MAX) == NULL){
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int getComputerName(char *hostName){
    if(gethostname(hostName, UCHAR_MAX) == -1){
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}   

char** demanderCommande(char** commande, int* nbCommandes) {
    char commandeEntree[sizelgcmd], **commandes;
    affichageLigneShell();
    fgets(commandeEntree, sizelgcmd-1, stdin);

    commandes = gererChaine(commandeEntree, commande, nbCommandes);

    return commandes;
}

void afficherLesCommandesEntrees(char** commandes, int nbCommandes) {
    printf("Voici les commandes entrées : \n");
    for (int i = 0; i < nbCommandes && commandes[i] != NULL; i++) {
        printf("- <%s>\n", commandes[i]);
    }
}

void afficherEnBrutLesCommandesEntrees(char** commandes, int nbCommandes) {
    for (int i = 0; i < nbCommandes && commandes[i] != NULL; i++) {
        printf("%s; ", commandes[i]);
    }
}

char** viderCommande(char** commandes) {
    int i;
    for (i = 0; i < sizelgcmd; i++) {
        if (commandes[i] == NULL) commandes[i] = (char*) calloc(sizeWord, sizeof(char));
        memset(commandes[i], '\0', sizeWord);
    }
    return commandes;
}

char** allouerMemoireCommandes() {
    int i;
    char** cmd = (char**) calloc(sizelgcmd, sizeof(char*));
    if (cmd == NULL) free(cmd), exit(EXIT_FAILURE);

    for (i = 0; i < sizelgcmd; i++) {
        cmd[i] = (char*) calloc(sizeWord, sizeof(char));
        if (cmd[i] == NULL) viderCommande(cmd), exit(EXIT_FAILURE);
    }
    
    return cmd;
}

char* chercherNomVariableRemplacer(char* commande, char* nom, int* index) {
    int i=0, j, k = 0;

    while (commande[i] != '$') {
        if (i < strlen(commande) && !isspace(commande[i]) && commande[i+1] == '$') i++;
        i++;
    }

    *index = i;
    // On commence à m+1 pour virer le " $"
    for(j=i+1; commande[j]; j++) {
        nom[k] = commande[j];
        k++;
    }
    nom[k] = '\0';
    return nom;
}

void remplacerDollarParVariable(char* commande, char* cmd) {
    int j, k, m, index;
    char* valeur;
    char* nom = (char*) calloc(sizeWord, sizeof(char));

    
    nom = chercherNomVariableRemplacer(commande, nom, &index);

    for(m=0; commande[m] && m < index; m++) {
        cmd[m] = commande[m];
    }

    if ((valeur = getenv(nom)) == NULL) {
        cmd = NULL;
    } else {
        for(j=0; valeur[j]; j++) {
            cmd[m++] = valeur[j];
        }
    }

    for(k=m; commande[k+strlen(nom)]; k++) cmd[k] = commande[k+strlen(nom)];
    cmd[k] = '\0';

    free(nom);
}

char** remplacerLesVariablesDansLesCommandes(char** commandes, int nbCommandes, int* status) {
    int i,j;
    regex_t regex;
    char* cmd = (char*) calloc(sizeWord, sizeof(char));
    const char schema[19] = " +\\$[a-zA-Z0-9_-]+";
    if (regcomp(&regex, schema, REG_EXTENDED)) {
        regfree(&regex);
        fatalsyserror(8);
    }
    for (i=0; i < nbCommandes; i++) {
        while (commandes[i] != NULL && regexec(&regex, commandes[i], 0, NULL, 0) == 0) {
            remplacerDollarParVariable(commandes[i], cmd);
            if (cmd != NULL) {
                memset(commandes[i], '\0', sizeWord);
                for (j=0; cmd[j]; j++) {
                    commandes[i][j] = cmd[j];
                }
            }
            else {
                free(cmd);
                *status = -1;
                return commandes;
            }
        }
    }
    if (cmd != NULL) free(cmd);
    regfree(&regex);
    return commandes;
}

void recupererNomProgramme(char nomProgramme[100], char* commande) {
    int i;
    for (i = 0; commande[i] != ' ' && commande[i] != '\n' && commande[i] != '\0'; i++) {
        nomProgramme[i] = commande[i];
    }
}

int recupererArguments(char* args[], char* commande) {
    int i, indice = 0, ind = 0;
    for (i=0; i < strlen(commande); i++) {
        if (isspace(commande[i])) {
                indice++;
                ind = 0;
            continue;
        }
        args[indice][ind++] = commande[i];
    }
    return indice;
}

void prompt(char* chaine, char* message, int initialSize) {
    size_t max = initialSize;
    size_t size = 0;
    char car;

    printf("%s", message);

    while(1) {
        car = getchar();
        size++;

        if (size >= max) break;
        
        chaine[size-1] = (char) car;
        
        if ((char) car == '\n') {
            chaine[size] = '\0';
            break;
        }
    }
}

int match(char *first, char * second) 
{ 
    // If we reach at the end of both strings, we are done 
    if (*first == '\0' && *second == '\0') 
        return 1; 

    // Make sure that the characters after '*' are present 
    // in second string. This function assumes that the first 
    // string will not contain two consecutive '*' 
    if (*first == '*' && *(first+1) != '\0' && *second == '\0') 
        return 0; 

    // If the first string contains '?', or current characters 
    // of both strings match 
    if (*first == '?' || *first == *second) 
        return match(first+1, second+1); 

    // If there is *, then there are two possibilities 
    // a) We consider current character of second string 
    // b) We ignore current character of second string. 
    if (*first == '*') 
        return match(first+1, second) || match(first, second+1); 
    return 0;  
}


int reg_match(char *string, char *pattern)
{
    int status;
    regex_t re;
    if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) {
        return(0);      /* Report error. */
    }
    status = regexec(&re, string, (size_t) 0, NULL, 0);
    regfree(&re);
    if (status != 0) {
        return(0);      /* Report error. */
    }
    return(1);
}

int wildcardMatch(char *text, char *pattern) {
    int n = strlen(text);
    int m = strlen(pattern);
    int backslash = 0;
    char *regex_pattern = malloc(sizeof(pattern));
    char *regex_text = malloc(sizeof(text));
    if (m == 0)    //when pattern is empty
        return (n == 0);

    int i = 0, j = 0, r1 = 0, r2 = 0, textPointer = -1, pattPointer = -1;
    while (i < n) {
        if (text[i] == pattern[j]) {    //matching text and pattern characters
            i++;
            j++;
            backslash = 0;
        } else if (j < m && pattern[j] == '?' && !backslash) {    //as ? used for one character
            i++;
            j++;
        } else if (j < m && pattern[j] == '*' && !backslash) {    //as * used for one or more character
            textPointer = i;
            pattPointer = j;
            printf("lol\n");
            j++;
        } else if(j < m && pattern[j] == '[' && !backslash) {
            r1 = j;
            r2 = r1;
            j++;
            printf("i : %d \n", i);
            while(j < m && pattern[j] != ']') {
                r2++;
                j++;
            }
            r2++;
            printf("R1 : %d et R2 : %d\n", r1, r2);
            strncpy(regex_pattern, pattern + r1, (r2 - r1) + 1);
            regex_pattern[(r2-r1) + 1] = '\0';
            printf("char à tester : %c", text[i]);
            printf("%s\n", regex_pattern);
            regex_text[0] = text[i];
            regex_text[1] = '\0';
            j++;
            i++;
            if (!reg_match(regex_text,regex_pattern)){
                free(regex_pattern);
                return 0;
            }
        } else if (j < m && pattern[j] == '\\'){
            backslash = 1;
            j++;
        } else if (pattPointer != -1) {
            j = pattPointer + 1;
            i = textPointer + 1;
            textPointer++;
        } else {
            return 0;
        }
        printf("j%d et i %d  pj : %d et pi : %d\n",j,i, pattPointer, textPointer);
    }

    while (j < m && pattern[j] == '*') {
        printf("j : %d ----------\n", j);
        j++;     //j will increase when wildcard is *
    }

    if (j == m) {    //check whether pattern is finished or not
        return 1;
    }
    return 0;
}


// int main(void) {
//     char** commandes = allouerMemoireCommandes();
//     int nbCommandes;
//     printf("Programme lancé ...\n");
//     commandes = demanderCommande(commandes, &nbCommandes);
//     commandes = remplacerLesVariablesDansLesCommandes(commandes, nbCommandes);
//     printf("Apres gestion :\n");
//     afficherLesCommandesEntrees(commandes, nbCommandes);
// }