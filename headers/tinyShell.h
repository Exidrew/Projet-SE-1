#ifndef DEF_TINYSHELLH
#define DEF_TINYSHELLH

#include "liste_chaine.h"

#define FAIL_EXEC 127
#define sizelgcmd 4096
#define size 1024

#define ROUGE(message) "\033[01;31m"message"\033[0m"
#define VERT(message) "\033[01;32m"message"\033[0m"

int tubeSetVariable[2];

void afficherRetour(char** tabcmd);
void executerCommande(char** tabcmd);

#endif