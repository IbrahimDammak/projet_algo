#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "workfile.h"

/*
Format attendu : name\thash\tmode (champs séparés par tabulation).
69.	Calculer la taille nécessaire : strlen(name) + strlen(hash) + longueur de mode en texte + 3 (tabulations + '\0').
70.	Allouer la chaîne et utiliser sprintf() pour formater : "%s\t%s\t%d".
71.	Gérer le cas où hash est NULL (remplacer par "NULL" ou chaîne vide).
72.	Retourner la chaîne.

*/
char* wfts(WorkFile* wf){
    if (wf == NULL) {
        fprintf(stderr, "Error: WorkFile pointer is NULL\n");
        return NULL;
    }
    const char* hash_str = (wf->hash != NULL) ? wf->hash : "NULL";
    int mode_len = snprintf(NULL, 0, "%d", wf->mode);
    size_t total_len = strlen(wf->name) + strlen(hash_str) + mode_len + 3; // 2 tabs + null terminator
    char* result = (char*) malloc(total_len);
    if (result == NULL) {
        perror("malloc");
        return NULL;
    }
    sprintf(result, "%s\t%s\t%d", wf->name, hash_str, wf->mode);
    return result;      
}