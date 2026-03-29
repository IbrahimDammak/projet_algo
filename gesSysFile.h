#ifndef GESSYSFILE_H
#define GESSYSFILE_H

#include "liste.h"

// Copy file from source to destination
// Parameters: to - destination file path, from - source file path
// Returns: void (no status indication - BUG: should return int for success/failure)
void cp(char* to, char* from);

// List directory contents
// Parameters: root_dir - directory path to list
// Returns: pointer to List containing directory entries, or NULL on error
List* listdir(char* root_dir);

// Check if file exists in current directory
// Parameters: file - filename to check
// Returns: 1 if file exists, 0 if not found
// BUG WARNING: This function has a memory leak in file_exists()
int file_exists(char* file);

#endif
