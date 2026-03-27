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
    char cmd[1024]
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

}

void blobFile(char* file){

}