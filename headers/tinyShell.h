#ifndef DEF_TINYSHELLH
#define DEF_TINYSHELLH

#define FAIL_EXEC 127
#define CMD_EXIT "exit"

#define estCommande(commande, programme)\
    !strncmp(commande, programme, strlen(programme))

void afficherRetour(char** tabcmd, int nbCommandes, int status);
void executerSetVariable(char* tabcmd);
void executerDelVariable(char* tabcmd);
void executerCommande(char** tabcmd, int nbCommandes, int* status);
void freeCommandes(char** commandes);

#endif