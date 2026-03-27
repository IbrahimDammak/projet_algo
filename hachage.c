#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char* sha256file(char* file);
char* hashToString(char* hash);
void blobFile(char* file);




char* sha256file(char* file){
   
char fname[] = "/tmp/hashXXXXXX";//le nom de fichée temporaire
int fd = mkstemp(fname); //creation du fichier temporaire retourne -1 en cas d erreur
 if (fd == -1) 
 {
        perror("mkstemp");
        return NULL;
 }
    close(fd);
    char cmd[1024];
    sprintf(cmd, "cat %s | sha256sum > %s", file, fname);

    if (system(cmd) != 0) 
    {// execute la commande et retourne -1 en cas d erreur
     perror("system");
     unlink(fname);
    return NULL;
     }
     FILE* f = fopen(fname, "r");
    if (f == NULL)
    {
        perror("fopen");
        unlink(fname);
        return NULL;
    }

    char buffer[128];
    if (fgets(buffer, sizeof(buffer), f) == NULL) {
        perror("fgets");
        fclose(f);
        unlink(fname);
        return NULL;
    }
      char* hash = (char*)malloc(65);
    if (hash == NULL) {
        perror("malloc");
        fclose(f);
        unlink(fname);
        return NULL;
    }
      // Copier les 64 premiers caractères
    strncpy(hash, buffer, 64);
    hash[64] = '\0';
    fclose(f);
    unlink(fname);
    return hash;
}


char* hashToString(char* hash){
    // convertir un hash en chemain en inserant un "/" entre le 2eme et le 3eme caractere
    // BUG FIX: Changed &hash to hash - &hash always points to valid memory location on stack
    // We need to check if the pointer itself is NULL, not its address
    if (hash != NULL && strlen(hash) >= 3)
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