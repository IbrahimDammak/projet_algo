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

/*Format : représentation de chaque WorkFile séparée par '\n'.
89.	Pour chaque WorkFile dans wt->tab, appeler wfts() pour obtenir sa représentation.
90.	Calculer la taille totale nécessaire (somme des longueurs + séparateurs '\n').
91.	Allouer et construire la chaîne finale avec strcat(). Ajouter '\n' entre chaque entrée.
92.	Retourner la chaîne résultante
*/


char* wtts(WorkTree* wt) {  // Function to convert entire WorkTree to a single string representation
    if (!wt) {  // Check if input WorkTree pointer is NULL
        return NULL;  // Return NULL if invalid input (prevents crash)
    }

    size_t total_len = 0;  // Initialize variable to accumulate total string length needed
    char** entries = malloc(wt->n * sizeof(char*));  // Allocate array of pointers to hold each WorkFile string
    if (!entries) {  // Check if memory allocation succeeded
        return NULL;  // Return NULL if malloc failed (out of memory)
    }

    for (int i = 0; i < wt->n; i++) {  // Loop through each WorkFile in the tree
        entries[i] = wfts(&wt->tab[i]);  // Convert each WorkFile to string format (name\thash\tmode)
        if (entries[i]) {  // Check if wfts() successfully created the string
            total_len += strlen(entries[i]) + 1;  // Add string length + 1 for newline separator
        }
    }

    char* result = malloc(total_len + 1);  // Allocate memory for final concatenated string (+1 for null terminator)
    if (!result) {  // Check if memory allocation for result succeeded
        for (int i = 0; i < wt->n; i++) {  // If malloc failed, loop through entries
            free(entries[i]);  // Free each allocated entry string to prevent memory leak
        }
        free(entries);  // Free the entries array itself
        return NULL;  // Return NULL to signal allocation failure
    }

    result[0] = '\0';  // Initialize result as empty string (set first char to null terminator)
    for (int i = 0; i < wt->n; i++) {  // Loop through each entry again
        if (entries[i]) {  // Check if the entry string exists
            strcat(result, entries[i]);  // Append the entry string to result
            strcat(result, "\n");  // Append newline character as separator
            free(entries[i]);  // Free individual entry string after concatenating (prevent leak)
        }
    }
    free(entries);  // Free the entries array pointer
    return result;  // Return the complete concatenated string containing all WorkFiles

}


/*93.	Initialiser un WorkTree avec initWorkTree().
94.	Faire une copie de s. Utiliser strtok() avec '\n' pour extraire chaque ligne.
95.	Pour chaque ligne non vide, appeler stwf() pour créer un WorkFile.
96.	Ajouter chaque WorkFile au WorkTree via appendWorkTree().
97.	Retourner le WorkTree.
*/

WorkTree* stwt(char* s) {  // Function to parse string back into a WorkTree structure
    if (!s) {  // Check if input string is NULL or empty
        return NULL;  // Return NULL if invalid input
    }

    WorkTree* wt = initWorkTree();  // Create and initialize an empty WorkTree
    if (!wt) {  // Check if initialization succeeded
        return NULL;  // Return NULL if WorkTree initialization failed
    }

    char* copy = malloc(strlen(s) + 1);  // Allocate memory for a copy of the input string
    if (!copy) {  // Check if malloc succeeded
        free(wt);  // Free the WorkTree (cleanup before returning)
        return NULL;  // Return NULL if memory allocation failed
    }
    strcpy(copy, s);  // Copy the input string to preserve original (strtok modifies its input)

    char* line = strtok(copy, "\n");  // Extract first line using newline as delimiter
    while (line) {  // Loop while strtok returns non-NULL (more lines to process)
        if (strlen(line) > 0) {  // Check if line is not empty (skip empty lines)
            WorkFile* wf = stwf(line);  // Parse the line string into a WorkFile structure
            if (wf) {  // Check if parsing succeeded
                appendWorkTree(wt, wf->name, wf->hash ? wf->hash : "NULL", wf->mode);  // Add parsed WorkFile to tree (use "NULL" if hash is NULL)
                free(wf->name);  // Free the name string from temporary WorkFile
                free(wf->hash);  // Free the hash string from temporary WorkFile
                free(wf);  // Free the temporary WorkFile structure itself
            }
        }
        line = strtok(NULL, "\n");  // Get the next line (continue parsing from where we left off)
    }
    free(copy);  // Free the copy of the input string
    return wt;  // Return the populated WorkTree
}

/*98.	Appeler wtts(wt) pour sérialiser le WorkTree.
99.	Ouvrir file en écriture avec fopen(). Vérifier l'ouverture.
100.	Écrire la chaîne avec fprintf(). Fermer le fichier.
101.	Retourner 0 en cas de succès, -1 sinon.
*/

int wttf(WorkTree* wt, char* file) {  // Function to write WorkTree to a file (persistence)
    if (!wt || !file) {  // Check if either WorkTree or filename pointer is NULL
        return -1;  // Return -1 to indicate failure
    }

    char* serialized = wtts(wt);  // Convert entire WorkTree to a single string
    if (!serialized) {  // Check if serialization succeeded
        return -1;  // Return -1 if wtts() returned NULL (failure)
    }

    FILE* f = fopen(file, "w");  // Open file for writing (create if doesn't exist, truncate if does)
    if (!f) {  // Check if file opening succeeded
        free(serialized);  // Free the serialized string before returning
        return -1;  // Return -1 to indicate file opening failure
    }

    fprintf(f, "%s", serialized);  // Write the entire serialized string to the file
    fclose(f);  // Close the file to ensure data is written to disk
    free(serialized);  // Free the allocated serialized string
    return 0;  // Return 0 to indicate success
}

/*
102.	Ouvrir file en lecture avec fopen(). Vérifier l'ouverture.
103.	Lire tout le contenu (utiliser fseek/ftell pour connaitre la taille, puis fread).
104.	Appeler stwt() sur le contenu lu.
105.	Fermer le fichier et retourner le WorkTree.
*/

WorkTree* ftwt(char* file) {  // Function to read WorkTree from a file (load from disk)
    if (!file) {  // Check if filename pointer is NULL
        return NULL;  // Return NULL if invalid input
    }

    FILE* f = fopen(file, "r");  // Open file for reading
    if (!f) {  // Check if file opening succeeded
        return NULL;  // Return NULL if file cannot be opened (doesn't exist, permission denied, etc)
    }

    fseek(f, 0, SEEK_END);  // Move file pointer to end of file
    long size = ftell(f);  // Get current position (equals file size in bytes)
    fseek(f, 0, SEEK_SET);  // Move file pointer back to beginning for reading

    char* content = malloc(size + 1);  // Allocate buffer to hold entire file (+1 for null terminator)
    if (!content) {  // Check if memory allocation succeeded
        fclose(f);  // Close file before returning (cleanup resource)
        return NULL;  // Return NULL if malloc failed (out of memory)
    }

    fread(content, 1, size, f);  // Read entire file into the buffer (1 byte at a time, 'size' bytes total)
    content[size] = '\0';  // Add null terminator to make it a valid C string
    fclose(f);  // Close the file after reading all data from it

    WorkTree* wt = stwt(content);  // Parse the file content string back into WorkTree structure
    free(content);  // Free the temporary buffer that held file contents
    return wt;  // Return the reconstructed WorkTree
}

