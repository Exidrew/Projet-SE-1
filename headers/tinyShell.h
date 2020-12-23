#ifndef DEF_TINYSHELLH
#define DEF_TINYSHELLH

#define FAIL_EXEC 127
#define COMMANDE_INTERNE "my"

#define CMD_EXIT "exit"
#define CMD_MYPS "myps"

#define estCommande(commande, programme)\
    !strncmp(commande, programme, strlen(programme))

#define estCommandeMy(commande)\
    !strncmp(commande, COMMANDE_INTERNE, strlen(COMMANDE_INTERNE))

#define estSeparateur(commande)\
    !strcmp(commande, "||") || !strcmp(commande, "&&") ||\
    !strcmp(commande, "|")

void afficherRetour(char** tabcmd, int nbCommandes, int nbRedirection, int status);
void executerSetVariable(char* tabcmd);
void executerDelVariable(char* tabcmd);
int executerCommande(char** tabcmd, int nbCommandes, int* status);
void freeCommandes(char** commandes);
void executerProgrammeExterne(char* commandes, int redirection, int finDeRedirection, int* status);

#endif