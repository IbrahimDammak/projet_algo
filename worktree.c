#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "worktree.h"

#ifndef WORKTREE_MAX_SIZE
#define WORKTREE_MAX_SIZE 100
#endif

WorkTree* initWorkTree() {
    WorkTree* wt = malloc(sizeof(WorkTree));
    if (!wt) {
        return NULL;
    }

    wt->tab = malloc(WORKTREE_MAX_SIZE * sizeof(WorkFile));
    if (!wt->tab) {
        free(wt);
        return NULL;
    }

    wt->size = WORKTREE_MAX_SIZE;
    wt->n = 0;
    return wt;
}

int inWorkTree(WorkTree* wt, char* name) {
    if (!wt || !name) {
        return -1;
    }

    for (int i = 0; i < wt->n; i++) {
        if (wt->tab[i].name && strcmp(wt->tab[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

int appendWorkTree(WorkTree* wt, char* name, char* hash, int mode) {
    if (!wt || !name || !hash) {
        return -1;
    }

    if (inWorkTree(wt, name) != -1) {
        return -1;
    }

    if (wt->n >= wt->size) {
        return -1;
    }

    char* nameCopy = malloc(strlen(name) + 1);
    if (!nameCopy) {
        return -1;
    }
    strcpy(nameCopy, name);

    char* hashCopy = malloc(strlen(hash) + 1);
    if (!hashCopy) {
        free(nameCopy);
        return -1;
    }
    strcpy(hashCopy, hash);

    wt->tab[wt->n].name = nameCopy;
    wt->tab[wt->n].hash = hashCopy;
    wt->tab[wt->n].mode = mode;
    wt->n++;
    return 0;
}
