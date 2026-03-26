#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char* sha256file(char* file);
char* hashToString(char* hash);
void blobFile(char* file);




char* sha256file(char* file){
    return NULL;
}


char* hashToString(char* hash){
    // convertir un hash en chemain en inserant un "/" entre le 2eme et le 3eme caractere
    if (&hash != NULL && strlen(hash) >= 3)
    {
        char* path = malloc(strlen(hash) + 2); 
        for (size_t i = 0; i < 2; i++)
        {
            path[i] = hash[i];
        }
        path[2] = '/';
        for (size_t i = 2; i < strlen(hash); i++)
        {
            path[i + 1] = hash[i];
        }
        path[strlen(hash) + 1] = '\0';
        return path;    
    }
    
    return NULL;
}

void blobFile(char* file){
    char* hash = sha256file(file);
    char* path = hashToString(hash);
    if (path != NULL){
        char parentPath[2];
        char commande[256];
        strncpy(parentPath, path, 2);
        parentPath[2] = '\0';
        //construire la cmd mkdir pour le fichier parent 
        snprintf(commande, sizeof(commande), "mkdir -p .git/objects/%s", parentPath);
        system(commande);
        //construire la cmd cp pour copier le fichier dans le dossier .git/objects
        snprintf(commande, sizeof(commande), "cp %s .git/objects/%s", file, path);
        system(commande);
        free(path); // Libérer la mémoire allouée pour le chemin   
    }

}