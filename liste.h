#ifndef LISTE_H
#define LISTE_H

// Structure definition for list cells
typedef struct cell {
    char* data;
    struct cell* next;
} Cell;

typedef Cell* List;

// Function declarations from liste.c
List* initList();
Cell* buildCell(char* ch);
void insertFirst(List* L, Cell* C);
char* ctos(Cell* c);
char* ltos(List* L);
Cell* listGet(List* L, int i);
Cell* searchList(List* L, char* str);
List* stol(char* s);
void ltof(List* l, char* path);
List* ftol(char* path);

// Helper function to completely free a list and all its nodes
// Parameters: L - pointer to the list to free
// Returns: void
void freeList(List* L);

#endif
