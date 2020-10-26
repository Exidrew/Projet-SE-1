typedef struct var_locale var_locale;
struct var_locale
{
    char *nom;
    char *valeur;
    struct var_locale *suivant;
};

typedef var_locale* list_var;


list_var ajouterEnFin(list_var liste, char *nom, char *valeur);

list_var supprimerVar(list_var liste, char *nom);

list_var var_existe(list_var liste, char *nom);

void afficher_variables(list_var liste);
