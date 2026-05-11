#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "worktree.h"
#include "svgRes.h"
#include "hachage.h"
#include "gesSysFile.h"
#include "liste.h"

/* ===== HELPER FUNCTIONS ===== */

/**
 * Helper: Construct a destination path by joining parent_path and filename
 * Returns allocated path string, or NULL on failure
 */
static char* constructPath(const char* parent_path, const char* filename) {
    if (parent_path == NULL || filename == NULL) {
        return NULL;
    }
    size_t len = strlen(parent_path) + strlen(filename) + 2;  // +2 for "/" and null terminator
    char* result = malloc(len);
    if (result == NULL) {
        perror("malloc");
        return NULL;
    }
    snprintf(result, len, "%s/%s", parent_path, filename);
    return result;
}

/**
 * Helper: Construct a blob path with ".t" extension
 * Returns allocated path string, or NULL on failure
 */
static char* constructBlobPath(const char* hash) {
    if (hash == NULL) {
        return NULL;
    }
    size_t len = strlen(hash) + strlen(".t") + 1;  // +1 for null terminator
    char* result = malloc(len);
    if (result == NULL) {
        perror("malloc");
        return NULL;
    }
    snprintf(result, len, "%s.t", hash);
    return result;
}

/**
 * Helper: Execute mkdir command safely
 * Returns 0 on success, -1 on failure
 */
static int execMkdir(const char* path) {
    if (path == NULL) {
        fprintf(stderr, "Error: NULL path for mkdir\n");
        return -1;
    }
    char cmd[1024];
    int ret = snprintf(cmd, sizeof(cmd), "mkdir -p %s", path);
    if (ret < 0 || ret >= (int)sizeof(cmd)) {
        fprintf(stderr, "Error: Path too long for mkdir command\n");
        return -1;
    }
    return system(cmd) == 0 ? 0 : -1;
}

char* saveWorkTree(WorkTree* wt, char* path) {
    if (wt == NULL || path == NULL) {
        return NULL;
    }

    for (int i = 0; i < wt->n; i++) {
        WorkFile* wf = &wt->tab[i];
        char* full_path = constructPath(path, wf->name);
        if (full_path == NULL) {
            return NULL;
        }

        struct stat st;
        if (stat(full_path, &st) == -1) {
            free(full_path);
            return NULL;
        }

        if (S_ISREG(st.st_mode)) {
            blobFile(full_path);
            char* hash = sha256file(full_path);
            if (hash == NULL) {
                free(full_path);
                return NULL;
            }
            wf->hash = hash;
            wf->mode = st.st_mode & 0777;
        } else if (S_ISDIR(st.st_mode)) {
            List* dir_list = listdir(full_path);
            if (dir_list == NULL) {
                free(full_path);
                return NULL;
            }

            WorkTree* new_wt = initWorkTree();
            if (new_wt == NULL) {
                freeList(dir_list);
                free(full_path);
                return NULL;
            }

            Cell* curr = *dir_list;
            while (curr != NULL) {
                appendWorkTree(new_wt, curr->data, "NULL", 0);
                curr = curr->next;
            }

            char* hash = saveWorkTree(new_wt, full_path);
            if (hash == NULL) {
                free(new_wt);
                freeList(dir_list);
                free(full_path);
                return NULL;
            }

            wf->hash = hash;
            wf->mode = st.st_mode & 0777;
            free(new_wt);
            freeList(dir_list);
        }

        free(full_path);
    }

    return blobWorkTree(wt);
}

char* blobWorkTree(WorkTree* wt) {
    if (wt == NULL) {
        fprintf(stderr, "Error: NULL WorkTree pointer in blobWorkTree\n");
        return NULL;
    }

    char fname[] = "/tmp/wtXXXXXX";  // Template for temporary file
    int fd = mkstemp(fname);  // Create temporary file, returns file descriptor
    if (fd == -1) {
        perror("mkstemp");
        return NULL;  // Return NULL on error
    }
    close(fd);  // Close the file descriptor as we will write using wttf()

    if (wttf(wt, fname) != 0) {  // Write WorkTree to temporary file
        fprintf(stderr, "Error: Failed to write WorkTree to temporary file\n");
        unlink(fname);  // Clean up temporary file
        return NULL;  // Return NULL on error
    }

    char* hash = sha256file(fname);  // Get SHA256 hash of the temporary file
    if (hash == NULL) {
        fprintf(stderr, "Error: Failed to compute hash of WorkTree\n");
        unlink(fname);
        return NULL;  // Return NULL on error
    }

    // Create blob path with ".t" extension using helper function
    char* dest_path = constructBlobPath(hash);
    if (dest_path == NULL) {
        free(hash);
        unlink(fname);
        return NULL;
    }

    // FIX: Store WorkTree blobs under .git/objects/ like regular blobs.
    char blob_dir[256];
    snprintf(blob_dir, sizeof(blob_dir), ".git/objects/%.*s", 2, hash);
    if (execMkdir(blob_dir) != 0) {
        fprintf(stderr, "Error: Failed to create blob directory\n");
        free(hash);
        free(dest_path);
        unlink(fname);
        return NULL;
    }

    // FIX: Copy the temporary WorkTree blob into the .git/objects path.
    char* blob_path = constructPath(blob_dir, dest_path);
    if (blob_path == NULL) {
        free(hash);
        free(dest_path);
        unlink(fname);
        return NULL;
    }
    cp(blob_path, fname);

    unlink(fname);  // Remove the temporary file
    free(blob_path);
    free(dest_path);  // Free allocated memory for destination path
    return hash;  // Return the hash of the stored WorkTree blob (caller must free this string)
}
 


void restoreWorkTree(WorkTree* wt, char* path) {
    if (wt == NULL || path == NULL) {
        fprintf(stderr, "Error: NULL pointer in restoreWorkTree\n");
        return;
    }

    for (int i = 0; i < wt->n; i++) {  // Loop through each WorkFile in the WorkTree
        WorkFile* wf = &wt->tab[i];  // Get pointer to current WorkFile
        char* snapshot_path = hashToPath(wf->hash);  // Get path to the snapshot file from hash
        if (snapshot_path == NULL) {
            fprintf(stderr, "Error: Invalid hash for WorkFile '%s'\n", wf->name);
            continue;  // Skip this entry on error
        }

        // Use helper function to construct destination path
        char* dest_path = constructPath(path, wf->name);
        if (dest_path == NULL) {
            free(snapshot_path);
            continue;
        }

        // FIX: Only treat paths that end with ".t" as trees, not any path containing ".t".
        size_t snapshot_len = strlen(snapshot_path);
        if (snapshot_len < 2 || strcmp(snapshot_path + snapshot_len - 2, ".t") != 0) {
            cp(dest_path, snapshot_path);  // Copy the file from snapshot to destination
            // FIX: Use chmod() directly because setMode() is not defined in this project.
            chmod(dest_path, wf->mode);  // Set the file permissions
        } else {  // It's a directory (has ".t" extension)
            WorkTree* sub_wt = ftwt(snapshot_path);  // Reconstruct the WorkTree for the subdirectory
            if (sub_wt) {
                // Use helper function to create directory
                if (execMkdir(dest_path) == 0) {
                    restoreWorkTree(sub_wt, dest_path);  // Recursive call to restore the subdirectory
                } else {
                    fprintf(stderr, "Error: Failed to create directory '%s'\n", wf->name);
                }
                free(sub_wt);  // Free the allocated WorkTree for the subdirectory
            } else {
                fprintf(stderr, "Error: Failed to reconstruct WorkTree for '%s'\n", wf->name);
            }
        }
        free(snapshot_path);  // Free the allocated snapshot path string
        free(dest_path);  // Free the allocated destination path
    }
}
