#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct cell {
    char* data;
    struct cell* next;
} Cell;

typedef Cell* List;

List* initList() {
    List* L = (List*) malloc(sizeof(List));
    *L = NULL;
    return L;
}
Cell* buildCell(char* ch) {
    Cell* c = (Cell*) malloc(sizeof(Cell));
    
    c->data = (char*) malloc(strlen(ch) + 1);
    strcpy(c->data, ch);

    c->next = NULL;

    return c;
}
void insertFirst(List* L, Cell* C) 
{
    C->next = *L;
    *L = C;
}
char* ctos(Cell* c) 
{
    if (c == NULL) return NULL;
    return c->data;
}

char* ltos(List* L) 
{
    if (*L == NULL) {
        char* empty = (char*) malloc(1);
        empty[0] = '\0';
        return empty;
    }
    Cell* curr = *L;
    int len = 0;
    // Calcul taille
    while (curr != NULL)
    {
        len += strlen(curr->data);
        if (curr->next != NULL)
            len += 1; // pour '|'
        curr = curr->next;
    }
    char* result = (char*) malloc(len + 1);
    result[0] = '\0';
    curr = *L;
    while (curr != NULL) 
    {
        strcat(result, curr->data);
        if (curr->next != NULL)
            strcat(result, "|");
        curr = curr->next;
    }

    return result;
}


// Get the cell at index i in the list
// Parameters: L - pointer to the list, i - index of the cell to retrieve
// Returns: pointer to the Cell at index i, or NULL if index is out of bounds
Cell* listGet(List* L, int i){
    Cell* curr = *L;  // Start from the head of the list
    int c = 0;        // Counter to track current position
    // Traverse the list until we reach index i or end of list
    while (curr != NULL && c < i){
        curr = curr->next;  // Move to next cell
        c++;                // Increment counter
    }
    return curr;  // Return the cell at index i (or NULL if out of bounds)
}



// Search for a cell containing the specified string in the list
// Parameters: L - pointer to the list, str - string to search for
// Returns: pointer to the first Cell containing str, or NULL if not found
Cell* searchList(List* L, char* str){
    Cell *curr = *L;  // Start from the head of the list
    // Traverse the list looking for matching data
    while(curr != NULL){
        if (strcmp(curr->data, str) == 0){  // Compare cell data with search string
            return curr;  // Return cell if match found
        }
        curr = curr->next;  // Move to next cell
    }
    return NULL;  // Return NULL if string not found in list
}


// Convert a string (pipe-separated values) to a List
// Parameters: s - string with pipe-separated tokens (e.g., "val1|val2|val3")
// Returns: pointer to a new List containing the parsed tokens
List* stol(char* s){
    List* L = initList();  // Create a new empty list
    // BUG FIX: strtok() modifies the input string, make a copy to preserve original
    char* s_copy = (char*) malloc(strlen(s) + 1);
    strcpy(s_copy, s);
    
    char* token = strtok(s_copy, "|");  // Extract first token (split by pipe '|')
    // Parse all tokens separated by '|' and add to list
    while (token != NULL)
    {
        insertFirst(L, buildCell(token));  // Create cell and insert at beginning
        token = strtok(NULL, "|");  // Get next token
    }
    free(s_copy);  // BUG FIX: Free the copy after strtok processing
    return L;  // Return the populated list
}

// Write a list to a file (list is converted to pipe-separated string format)
// Parameters: l - pointer to the list, path - file path to write to
// Returns: void
void ltof(List* l, char* path){
    char* str = ltos(l);  // Convert list to pipe-separated string
    FILE* f = fopen(path, "w");  // Open file for writing
    if (f == NULL) {
        fprintf(stderr, "Error: Unable to open file %s for writing\n", path);
        free(str);  // BUG FIX: Free allocated string before returning
        return;     // BUG FIX: Return instead of trying to write to NULL pointer
    }
    fprintf(f, "%s", str);  // Write string to file
    fclose(f);  // Close the file
    free(str);  // Free the allocated string
}

// Read a file and convert its contents to a List (expects pipe-separated format)
// Parameters: path - file path to read from
// Returns: pointer to a new List parsed from file contents, or NULL on error
List* ftol(char* path){
    FILE* f = fopen(path, "r");  // Open file for reading
    if (f == NULL) {
        fprintf(stderr, "Error: Unable to open file %s for reading\n", path);
        return NULL;  // Return NULL if file cannot be opened
    }
    char buffer[1024];  // Buffer to store file contents
    if (fgets(buffer, sizeof(buffer), f) == NULL) {
        fprintf(stderr, "Error: Unable to read from file %s\n", path);
        fclose(f);
        return NULL;  // Return NULL if file cannot be read
    }
    fclose(f);  // Close the file
    
    // BUG FIX: fgets() includes the newline character, remove it
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';  // Replace newline with null terminator
    }
    
    List* L = stol(buffer);  // Convert string to list (parse pipe-separated values)
    return L;  // Return the newly created list
}