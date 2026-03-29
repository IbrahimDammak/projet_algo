#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "workfile.h"
typedef struct {
    char* name;
    char* hash;
    int mode;
} WorkFile;
WorkFile* createWorkFile(char* name) {
    // 65. Allouer la structure
    WorkFile* wf = (WorkFile*) malloc(sizeof(WorkFile));
    if (wf == NULL) return NULL;

    // 66. Allouer et copier name
    wf->name = (char*) malloc(strlen(name) + 1);
    if (wf->name == NULL) {
        free(wf);
        return NULL;
    }
    strcpy(wf->name, name);

    // 67. Initialiser les autres champs
    wf->hash = NULL;
    wf->mode = 0;

    // 68. Retourner le pointeur
    return wf;
}

WorkFile* stwf(char* ch) {
    if (ch == NULL) return NULL;

    // 73. copier la chaîne (car strtok modifie la chaîne)
    char* copy = strdup(ch);
    if (copy == NULL) return NULL;

    // 74. découpage avec strtok
    char* name = strtok(copy, "\t");
    char* hash = strtok(NULL, "\t");
    char* modeStr = strtok(NULL, "\t");

    // vérification
    if (name == NULL || modeStr == NULL) {
        free(copy);
        return NULL;
    }

    // 75. créer le WorkFile
    WorkFile* wf = createWorkFile(name);

    // gérer hash
    if (hash != NULL && strcmp(hash, "NULL") != 0) {
        wf->hash = strdup(hash);
    } else {
        wf->hash = NULL;
    }

    // convertir mode
    wf->mode = atoi(modeStr);

    // 76. libérer la copie
    free(copy);

    return wf;
}