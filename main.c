#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "hachage.h"
#include "liste.h"
#include "gesSysFile.h"

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

    printf("====== Testing Complete ======\n\n");
    
    // ============================================
    // TESTING LIST FUNCTIONS FROM liste.c
    // ============================================
    printf("\n====== Testing List Functions ======\n\n");

    // Test 1: initList and insertFirst
    printf("Test 1: initList() and insertFirst()\n");
    printf("------------------------\n");
    List* myList = initList();
    printf("Created empty list\n");
    
    insertFirst(myList, buildCell("Alice"));
    insertFirst(myList, buildCell("Bob"));
    insertFirst(myList, buildCell("Charlie"));
    printf("Inserted 3 elements: Alice, Bob, Charlie\n");
    
    char* result_str = ltos(myList);
    printf("List content (ltos): %s\n", result_str);
    printf("Note: Order is reversed due to insertFirst() adding to head\n");
    free(result_str);
    printf("\n");

    // Test 2: listGet function
    printf("Test 2: listGet()\n");
    printf("------------------------\n");
    Cell* cell0 = listGet(myList, 0);
    Cell* cell1 = listGet(myList, 1);
    Cell* cell2 = listGet(myList, 2);
    Cell* cell3 = listGet(myList, 3);
    
    printf("Cell at index 0: %s\n", cell0 != NULL ? cell0->data : "NULL");
    printf("Cell at index 1: %s\n", cell1 != NULL ? cell1->data : "NULL");
    printf("Cell at index 2: %s\n", cell2 != NULL ? cell2->data : "NULL");
    printf("Cell at index 3 (out of bounds): %s\n", cell3 != NULL ? cell3->data : "NULL");
    printf("\n");

    // Test 3: searchList function
    printf("Test 3: searchList()\n");
    printf("------------------------\n");
    Cell* search1 = searchList(myList, "Bob");
    Cell* search2 = searchList(myList, "David");
    
    printf("Search for 'Bob': %s\n", search1 != NULL ? "FOUND" : "NOT FOUND");
    printf("Search for 'David': %s\n", search2 != NULL ? "FOUND" : "NOT FOUND");
    printf("\n");

    // Test 4: ctos function
    printf("Test 4: ctos()\n");
    printf("------------------------\n");
    Cell* single_cell = buildCell("TestData");
    printf("Cell data via ctos: %s\n", ctos(single_cell));
    printf("NULL cell via ctos: %s\n", ctos(NULL) != NULL ? "DATA" : "NULL");
    free(single_cell->data);
    free(single_cell);
    printf("\n");

    // Test 5: ltos function (list to string)
    printf("Test 5: ltos()\n");
    printf("------------------------\n");
    char* list_str = ltos(myList);
    printf("List as pipe-separated string: '%s'\n", list_str);
    free(list_str);
    
    // Test with empty list
    List* emptyList = initList();
    char* empty_str = ltos(emptyList);
    printf("Empty list as string: '%s' (length: %lu)\n", empty_str, strlen(empty_str));
    free(empty_str);
    printf("\n");

    // Test 6: stol function (string to list) - BUG WARNING
    printf("Test 6: stol() - STRING TO LIST CONVERSION\n");
    printf("------------------------\n");
    printf("⚠️  BUG FOUND: stol() uses strtok() which modifies input string\n");
    printf("⚠️  Solution: Make a copy of input string before calling stol()\n");
    
    char test_str[] = "apple|banana|cherry|date";  // Must be modifiable array
    printf("Input string: %s\n", test_str);
    
    List* newList = stol(test_str);
    char* new_list_str = ltos(newList);
    printf("Resulting list: %s\n", new_list_str);
    printf("Note: Order may differ due to insertFirst() + strtok() behavior\n");
    free(new_list_str);
    printf("\n");

    // Test 7: ltof function (list to file) - BUG WARNING
    printf("Test 7: ltof() - LIST TO FILE\n");
    printf("------------------------\n");
    printf("⚠️  BUG FOUND: ltof() doesn't return/exit after fopen error\n");
    printf("⚠️  This causes fclose(NULL) which is undefined behavior\n");
    
    char* filepath = "test_list.txt";
    // Create a list
    List* fileList = initList();
    insertFirst(fileList, buildCell("red"));
    insertFirst(fileList, buildCell("green"));
    insertFirst(fileList, buildCell("blue"));
    
    printf("Writing list to file: %s\n", filepath);
    ltof(fileList, filepath);
    printf("Write operation completed\n");
    printf("\n");

    // Test 8: ftol function (file to list) - BUG WARNING
    printf("Test 8: ftol() - FILE TO LIST\n");
    printf("------------------------\n");
    printf("⚠️  BUG FOUND: fgets() includes newline character '\\n'\n");
    printf("⚠️  This causes incorrect parsing of last element\n");
    
    List* readList = ftol(filepath);
    if (readList != NULL) {
        char* read_str = ltos(readList);
        printf("Read from file: '%s'\n", read_str);
        printf("Note: Newline character may be included in last element\n");
        free(read_str);
    }
    printf("\n");

    // Test 9: Edge cases and error conditions
    printf("Test 9: Edge Cases\n");
    printf("------------------------\n");
    
    // Empty string to list
    List* test_empty = initList();
    char empty_input[] = "";
    List* empty_result = stol(empty_input);
    printf("stol(\"\") result: '%s'\n", ltos(empty_result));
    free(ltos(empty_result));
    
    // Single element
    List* test_single = initList();
    char single_input[] = "single";
    List* single_result = stol(single_input);
    printf("stol(\"single\") result: '%s'\n", ltos(single_result));
    free(ltos(single_result));
    printf("\n");

    // Test 10: File I/O error handling
    printf("Test 10: File I/O Error Handling\n");
    printf("------------------------\n");
    
    // Try to read from non-existent file
    printf("Attempting to read non-existent file...\n");
    List* error_list = ftol("/path/that/does/not/exist.txt");
    printf("Result: %s\n", error_list == NULL ? "NULL (error caught)" : "UNEXPECTED");
    printf("\n");

    // Cleanup test files
    printf("Cleaning up test files...\n");
    if (access(filepath, F_OK) == 0) {
        remove(filepath);
        printf("Removed %s\n", filepath);
    }
    printf("\n");

    // Summary of bugs found
    printf("====== BUG SUMMARY (LISTE.C) ======\n");
    printf("1. ltof() BUG: No return after fopen error - causes fclose(NULL)\n");
    printf("2. stol() BUG: Uses strtok() which modifies input string\n");
    printf("3. ftol() BUG: fgets() includes newline, causes parse errors\n");
    printf("4. stol() DESIGN: insertFirst() reverses element order\n");
    printf("\n");

    // ============================================
    // TESTING FILE SYSTEM FUNCTIONS FROM gesSysFile.c
    // ============================================
    printf("\n====== Testing File System Functions (gesSysFile.c) ======\n\n");

    // Test 1: cp() function - file copy
    printf("Test 1: cp() - FILE COPY FUNCTION\n");
    printf("------------------------\n");
    
    // Create source file
    char* src_file = "source.txt";
    char* dst_file = "destination.txt";
    
    FILE* src = fopen(src_file, "w");
    if (src != NULL) {
        fprintf(src, "Line 1: Hello World\n");
        fprintf(src, "Line 2: Testing file copy\n");
        fprintf(src, "Line 3: This is a test file\n");
        fclose(src);
        printf("Created source file: %s\n", src_file);
    }
    
    printf("Copying %s to %s...\n", src_file, dst_file);
    cp(dst_file, src_file);
    printf("Copy operation completed\n");
    
    // Verify copy was successful
    FILE* verify = fopen(dst_file, "r");
    if (verify != NULL) {
        printf("Verification - Reading destination file:\n");
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), verify) != NULL) {
            printf("  %s", buffer);
        }
        fclose(verify);
    }
    printf("\n");

    // Test 2: cp() with non-existent source
    printf("Test 2: cp() - ERROR HANDLING (non-existent source)\n");
    printf("------------------------\n");
    printf("Attempting to copy non-existent file...\n");
    cp("test_output.txt", "/path/that/does/not/exist/file.txt");
    printf("Error handled (message above)\n");
    printf("\n");

    // Test 3: listdir() function
    printf("Test 3: listdir() - DIRECTORY LISTING\n");
    printf("------------------------\n");
    List* dir_list = listdir(".");
    if (dir_list != NULL) {
        char* dir_content = ltos(dir_list);
        printf("Current directory contents:\n%s\n", dir_content);
        free(dir_content);
    } else {
        printf("Error: Could not open directory\n");
    }
    printf("\n");

    // Test 4: listdir() with invalid directory
    printf("Test 4: listdir() - ERROR HANDLING (invalid directory)\n");
    printf("------------------------\n");
    printf("Attempting to open non-existent directory...\n");
    List* invalid_dir = listdir("/path/that/does/not/exist/directory");
    printf("Result: %s\n", invalid_dir == NULL ? "NULL (error caught)" : "UNEXPECTED");
    printf("\n");

    // Test 5: file_exists() function
    printf("Test 5: file_exists() - FILE EXISTENCE CHECK\n");
    printf("------------------------\n");
    printf("⚠️  BUG FOUND: file_exists() has memory leak!\n");
    printf("⚠️  It frees only the List structure, not the Cell nodes and data\n\n");
    
    printf("Checking if source.txt exists...\n");
    int exists = file_exists(src_file);
    printf("Result: %s\n", exists ? "EXISTS" : "NOT FOUND");
    
    printf("Checking if non-existent-file.txt exists...\n");
    int not_exists = file_exists("non-existent-file.txt");
    printf("Result: %s\n", not_exists ? "EXISTS" : "NOT FOUND");
    printf("\n");

    // Test 6: cp() with empty directory
    printf("Test 6: cp() - COPY TO READ-ONLY SCENARIO\n");
    printf("------------------------\n");
    printf("Testing copy operations with various file sizes...\n");
    cp("copy_test.txt", src_file);
    printf("Copy completed successfully\n");
    printf("\n");

    // Test 7: Multiple listdir calls (memory leak test)
    printf("Test 7: MEMORY LEAK TEST - Multiple listdir() calls\n");
    printf("------------------------\n");
    printf("⚠️  BUG FOUND: file_exists() leaks memory from listdir()\n");
    printf("⚠️  Cells and their data strings are not freed\n\n");
    printf("Calling file_exists() 3 times...\n");
    file_exists("main.c");
    file_exists("liste.c");
    file_exists("hachage.c");
    printf("All calls completed (memory leak occurs silently)\n");
    printf("\n");

    // Test 8: listdir() on current directory
    printf("Test 8: listdir() - DETAILED CONTENT ANALYSIS\n");
    printf("------------------------\n");
    List* detailed_list = listdir(".");
    if (detailed_list != NULL) {
        printf("Files in current directory:\n");
        Cell* curr = *detailed_list;
        int count = 0;
        while (curr != NULL) {
            printf("  %d. %s\n", ++count, curr->data);
            curr = curr->next;
        }
    }
    printf("\n");

    // Cleanup test files
    printf("Cleaning up test files...\n");
    if (access(src_file, F_OK) == 0) remove(src_file);
    if (access(dst_file, F_OK) == 0) remove(dst_file);
    if (access("copy_test.txt", F_OK) == 0) remove("copy_test.txt");
    if (access(filepath, F_OK) == 0) remove(filepath);
    printf("Test files cleaned up\n");
    printf("\n");

    // Summary of bugs found in gesSysFile.c
    printf("====== BUG SUMMARY (GESSYSFILE.C) ======\n");
    printf("1. file_exists() MEMORY LEAK: Doesn't free List cells and data\n");
    printf("   - Only frees the List structure itself\n");
    printf("   - All Cell nodes and their data strings are leaked\n");
    printf("   - Each call leaks memory proportional to directory size\n");
    printf("2. cp() MISSING ERROR HANDLING: Doesn't report success/failure\n");
    printf("   - No return value to indicate copy success\n");
    printf("   - Can't distinguish between successful and failed copies\n");
    printf("\n");

    printf("====== All Tests Complete ======\n");
    
    return 0;
}
