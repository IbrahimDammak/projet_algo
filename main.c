#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char* sha256file(char* file);
char* hashToString(char* hash);
void blobFile(char* file);

int main() {
    printf("====== Testing Hash Functions ======\n\n");

    // Test 1: hashToString function
    printf("Test 1: hashToString()\n");
    printf("------------------------\n");
    
    char* test_hash1 = "abc123def456";
    char* result1 = hashToString(test_hash1);
    if (result1 != NULL) {
        printf("Input hash:  %s\n", test_hash1);
        printf("Output path: %s\n", result1);
        free(result1);
    } else {
        printf("hashToString returned NULL\n");
    }
    printf("\n");

    // Test 2: hashToString with different hash
    printf("Test 2: hashToString() with another hash\n");
    printf("------------------------\n");
    
    char* test_hash2 = "fe1234567890abcdef";
    char* result2 = hashToString(test_hash2);
    if (result2 != NULL) {
        printf("Input hash:  %s\n", test_hash2);
        printf("Output path: %s\n", result2);
        free(result2);
    } else {
        printf("hashToString returned NULL\n");
    }
    printf("\n");

    // Test 3: Create a test file and test blobFile
    printf("Test 3: blobFile() - Create blob storage\n");
    printf("------------------------\n");
    
    // Create a test file
    char test_filename[] = "test_file.txt";
    FILE* test_file = fopen(test_filename, "w");
    if (test_file != NULL) {
        fprintf(test_file, "This is a test file for blob storage.\n");
        fclose(test_file);
        printf("Created test file: %s\n", test_filename);
        
        // Note: blobFile will fail because sha256file returns NULL
        printf("Cannot test blobFile() - sha256file() not yet implemented (returns NULL)\n");
        printf("\n");
    } else {
        printf("Failed to create test file\n");
    }

    // Test 4: hashToString with short hash (edge case)
    printf("Test 4: hashToString() with short hash (edge case)\n");
    printf("------------------------\n");
    
    char* test_hash3 = "ab";
    char* result3 = hashToString(test_hash3);
    if (result3 != NULL) {
        printf("Input hash:  %s\n", test_hash3);
        printf("Output path: %s\n", result3);
        free(result3);
    } else {
        printf("hashToString returned NULL (expected for hash < 3 chars)\n");
    }
    printf("\n");

    // Test 5: hashToString with NULL pointer (edge case)
    printf("Test 5: hashToString() with NULL pointer (edge case)\n");
    printf("------------------------\n");
    // BUG FIX: Now that we've fixed the pointer check, NULL is properly handled
    char* result4 = hashToString(NULL);
    if (result4 != NULL) {
        printf("Output: %s\n", result4);
        free(result4);
    } else {
        printf("hashToString returned NULL (expected for NULL input)\n");
    }
    printf("\n");

    // Test 6: sha256file function
    printf("Test 6: sha256file()\n");
    printf("------------------------\n");
    
    char* hash_result = sha256file(test_filename);
    if (hash_result != NULL) {
        printf("SHA256 of %s: %s\n", test_filename, hash_result);
        free(hash_result);
    } else {
        printf("sha256file() returned NULL (not yet implemented)\n");
    }
    printf("\n");

    // Cleanup
    if (access(test_filename, F_OK) == 0) {
        remove(test_filename);
        printf("Cleaned up test file.\n");
    }

    printf("====== Testing Complete ======\n");
    return 0;
}
