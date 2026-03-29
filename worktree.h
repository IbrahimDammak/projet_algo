#ifndef WORKTREE_H
#define WORKTREE_H

#include "workfile.h"

// WorkTree structure representing the current working tree
// Contains an array of WorkFile entries
typedef struct {
    WorkFile* tab;  // Array of WorkFile entries
    int size;       // Capacity of the array
    int n;          // Number of entries currently in the tree
} WorkTree;

// Initialize an empty WorkTree
// Parameters: none
// Returns: pointer to a new empty WorkTree
WorkTree* initWorkTree();

// Check if a file exists in the WorkTree
// Parameters: wt - pointer to the WorkTree, name - filename to search for
// Returns: 1 if file exists in tree, 0 otherwise
int inWorkTree(WorkTree* wt, char* name);

// Add a file to the WorkTree
// Parameters: wt - pointer to the WorkTree, name - filename, hash - SHA256 hash, mode - file mode
// Returns: 1 on success, 0 on failure
int appendWorkTree(WorkTree* wt, char* name, char* hash, int mode);

// Convert WorkTree to string format
// Parameters: wt - pointer to the WorkTree
// Returns: pointer to a string representation of the entire WorkTree
char* wtts(WorkTree* wt);

// Parse a string to create a WorkTree
// Parameters: s - string containing WorkTree data
// Returns: pointer to a new WorkTree parsed from the string
WorkTree* stwt(char* s);

// Write WorkTree to file
// Parameters: wt - pointer to the WorkTree, file - filename to write to
// Returns: 1 on success, 0 on failure
int wttf(WorkTree* wt, char* file);

// Read WorkTree from file
// Parameters: file - filename to read from
// Returns: pointer to a new WorkTree read from file, or NULL on error
WorkTree* ftwt(char* file);

#endif
