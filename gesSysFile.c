#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "liste.h"

List* listdir(char* root_dir);
int file_exists(char* file);
void cp(char* to, char* from);


void cp(char* to, char* from) {
    FILE* src = fopen(from, "r");
    if (src == NULL) {
        perror("fopen source");
        return;
    }

    FILE* dst = fopen(to, "w");
    if (dst == NULL) {
        perror("fopen destination");
        fclose(src);
        return;
    }

    char buffer[1024];  // taille maximale d'une ligne
    while (fgets(buffer, sizeof(buffer), src) != NULL) {
        fputs(buffer, dst);
    }

    fclose(src);
    fclose(dst);
}
/*
52.	Ouvrir le répertoire avec opendir(root_dir). Vérifier que le pointeur retourné n'est pas NULL.
53.	Initialiser une liste vide avec initList().
54.	Boucler avec readdir() tant que ep != NULL. Pour chaque entrée, créer une cellule avec buildCell(ep->d_name) et l'ajouter à la liste (avec insertFirst).
55.	Fermer le répertoire avec closedir() et retourner la liste.
Remarque : readdir retourne aussi les entrées "." et "..", pensez à les filtrer avec strcmp() si nécessaire.
*/

List* listdir(char* root_dir){
    DIR* dir = opendir(root_dir);
    if (dir == NULL) {
        fprintf(stderr, "Error: Unable to open directory %s\n", root_dir);
        return NULL;
    }
    List* L = initList();
    struct dirent* ep;
    while ((ep = readdir(dir)) != NULL) {
        // Filter out "." and ".." entries
        if (strcmp(ep->d_name, ".") != 0 && strcmp(ep->d_name, "..") != 0) {
            insertFirst(L, buildCell(ep->d_name));
        }
    }
    closedir(dir);
    return L;
}
/*
Retourne 1 si le fichier existe dans le répertoire courant, 0 sinon.
Étapes :
56.	Appeler listdir(".") pour obtenir la liste des fichiers du répertoire courant.
57.	Appeler searchList() sur cette liste avec le nom file.
58.	Si searchList retourne non-NULL, retourner 1. Sinon retourner 0.
59.	Libérer la liste après usage.
*/

int file_exists(char* file) {
    List* files = listdir(".");
    if (files == NULL) {
        return 0; // Could not read directory, assume file does not exist
    }
    Cell* found = searchList(files, file);
    free(files); // Free the list after searching
    return found != NULL ? 1 : 0; // Return 1 if found, 0 if not
}


