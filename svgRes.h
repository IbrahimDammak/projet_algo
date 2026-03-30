#ifndef SVGRES_H
#define SVGRES_H

#include "worktree.h"

// Create a blob (hash) representation of a WorkTree
// This function serializes the entire WorkTree and computes its hash
// Parameters: wt - pointer to the WorkTree to blob
// Returns: pointer to a string containing the blob hash, or NULL on error
// The returned string must be freed by the caller
char* blobWorkTree(WorkTree* wt);

// Save a WorkTree to disk with blob versioning
// Saves the WorkTree to a file at the specified path
// Parameters: wt - pointer to the WorkTree to save, path - file path to save to
// Returns: pointer to a string containing the blob hash of the saved WorkTree,
//          or NULL on error
// The returned string must be freed by the caller
char* saveWorkTree(WorkTree* wt, char* path);

// Restore a WorkTree from disk
// Loads a previously saved WorkTree from the specified file path
// Parameters: wt - pointer to the WorkTree structure to populate (should be initialized),
//             path - file path to restore from
// Returns: void (modifies the WorkTree structure in-place)
// Note: The WorkTree should be created with initWorkTree() before calling this function
void restoreWorkTree(WorkTree* wt, char* path);

#endif
