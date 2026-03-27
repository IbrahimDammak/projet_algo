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