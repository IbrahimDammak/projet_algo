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
    if (c == NULL) 
    return NULL;
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