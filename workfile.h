#ifndef WORKFILE_H
#define WORKFILE_H

// WorkFile structure representing a file in the working tree
// Contains: filename, its SHA256 hash, and file mode/permissions
typedef struct {
    char* name;   // File name
    char* hash;   // SHA256 hash of the file
    int mode;     // File mode/permissions
} WorkFile;

// Create a new WorkFile structure
// Parameters: name - the filename
// Returns: pointer to a new WorkFile, or NULL on error
WorkFile* createWorkFile(char* name);

// Convert a WorkFile to string format
// Parameters: wf - pointer to the WorkFile
// Returns: pointer to a string representation of the WorkFile
char* wfts(WorkFile* wf);

// Parse a string to create a WorkFile
// Parameters: ch - string containing WorkFile data
// Returns: pointer to a new WorkFile parsed from the string
WorkFile* stwf(char* ch);

#endif
