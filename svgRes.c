#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/stat.h>
#include "worktree.h"
#include "svgRes.h"
#include "liste.h"




/*
char* saveWorkTree(WorkTree* wt, char* path)
Sauvegarde récursivement tout le contenu du WorkTree, puis crée l'instantané du WorkTree lui-même. Retourne le hash de cet instantané.
Étapes — Boucle principale sur wt->tab :
113.	Pour chaque WorkFile WF dans wt->tab (de 0 à wt->n - 1) :
   a) Construire le chemin complet du fichier/répertoire : path + "/" + WF.name.
   b) Déterminer si c'est un fichier ou un répertoire (utiliser stat() ou tenter opendir()).
   c) Si c'est un FICHIER ordinaire :
      → Appeler blobFile(chemin_complet)
      → Appeler sha256file(chemin_complet) → stocker dans WF.hash
      → Appeler getChmod(chemin_complet) → stocker dans WF.mode
   d) Si c'est un RÉPERTOIRE :
      → Appeler listdir(chemin_complet) pour lister son contenu
      → Créer un nouveau WorkTree newWT avec initWorkTree()
      → Parcourir la liste et ajouter chaque entrée à newWT (appendWorkTree)
      → Appel récursif : char* h = saveWorkTree(newWT, chemin_complet)
      → Stocker h dans WF.hash, getChmod() dans WF.mode
114.	Une fois tous les WorkFile traités, appeler blobWorkTree(wt) pour créer l'instantané du WorkTree.
115.	Retourner le hash retourné par blobWorkTree.
*/

char* saveWorkTree(WorkTree* wt, char* path) {
    if (!wt || !path) {
        return NULL;
    }

    for (int i = 0; i < wt->n; i++) {
        WorkFile* wf = &wt->tab[i];
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, wf->name);

        struct stat st;
        if (stat(full_path, &st) == -1) {
            perror("stat");
            return NULL;
        }

        if (S_ISREG(st.st_mode)) {  // Regular file
            blobFile(full_path);
            char* hash = sha256file(full_path);
            if (!hash) {
                return NULL;
            }
            wf->hash = hash;
            wf->mode = st.st_mode & 0777;  // Store permissions
        } else if (S_ISDIR(st.st_mode)) {  // Directory
            List* dir_list = listdir(full_path);
            if (!dir_list) {
                return NULL;
            }
            WorkTree* newWT = initWorkTree();
            if (!newWT) {
                freeList(dir_list);
                return NULL;
            }
            Cell* curr = *dir_list;
            while (curr) {
                appendWorkTree(newWT, curr->data, "NULL", 0);  // Placeholder hash and mode
                curr = curr->next;
            }
            char* h = saveWorkTree(newWT, full_path);
            if (!h) {
                freeList(dir_list);
                free(newWT);
                return NULL;
            }
            wf->hash = h;
            wf->mode = st.st_mode & 0777;  // Store permissions
            freeList(dir_list);
            free(newWT);
        }
    }

    // After processing all WorkFiles, create blob for the WorkTree itself
    char* tree_hash = blobWorkTree(wt);
    return tree_hash;  // Return the hash of the WorkTree snapshot
}