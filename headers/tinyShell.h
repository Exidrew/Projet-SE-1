#ifndef DEF_TINYSHELLH
#define DEF_TINYSHELLH

#include "liste_chaine.h"

#define FAIL_EXEC 127
#define CMD_EXIT "exit"

#define ROUGE(message) "\033[01;31m"message"\033[0m"
#define VERT(message) "\033[01;32m"message"\033[0m"

#define estCommande(commande, programme)\
    !strncmp(commande, programme, strlen(programme))

int tubeSetVariable[2];

void afficherRetour(char** tabcmd, int nbCommandes);
void executerSetVariable(char* tabcmd);
void executerDelVariable(char* tabcmd);
void executerCommande(char** tabcmd, int nbCommandes);
void freeCommandes(char** commandes);

#endif