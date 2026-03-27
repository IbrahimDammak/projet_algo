#ifndef HACHAGE_H
#define HACHAGE_H

// Function declarations from hachage.c

// Compute SHA256 hash of a file and return it as a string
// Parameters: file - path to the file to hash
// Returns: pointer to a string containing the 64-character SHA256 hash, or NULL on error
char* sha256file(char* file);

// Convert a hash string to a path by inserting "/" after the first 2 characters
// Parameters: hash - the hash string (e.g., "abc123def456")
// Returns: pointer to a new string with format "ab/c123def456", or NULL if invalid input
char* hashToString(char* hash);

// Create a blob file in .git/objects directory (Git-like storage)
// Computes SHA256 hash of file and stores it in .git/objects/XX/YY... format
// Parameters: file - path to the file to store as a blob
// Returns: void
void blobFile(char* file);

#endif
