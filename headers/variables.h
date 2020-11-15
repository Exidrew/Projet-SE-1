typedef struct variables {
    char nom[1024];
    char valeur[1024];
} Variables;

typedef struct tabVar {
    Variables* variables;
    int nbVar;
} TableauVariables;

/* Verifie si une variable existe dans la lite donnée en paramètre par son nom.
Elle retourne l'indice de la variable si elle exitre, 0 sinon. */
int existe(TableauVariables* variables, char* nom);

/* Ajoute une variable dans la liste de variable donnée en paramètre.
Si la variable existe déjà, seul la valeur de cette variable est modifiée. */
void ajouterVariable(TableauVariables* variables, char* nom, char* valeur);

void afficherVariables(TableauVariables* variables);

/* Supprime une variable de la liste donnée en paramètre si elle existe. */
void supprimerVariable(TableauVariables* variables, char* nom);

/* Libère la mémoire allouée aux variables. */
void freeVariables(TableauVariables* variables);