#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "liste.h"

List* listdir(char* root_dir);
int file_exists(char* file);
void cp(char* to, char* from);

