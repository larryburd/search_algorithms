#include "str_util/string_util.h"
#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include <jansson.h>

// To compile
// gcc -I str_util -c dfs.c -o dfs.o
// gcc string_util.o dfs.o dfs -ljansson

int main(int argc, char* argv[]) {
    if (argc < 2) {
        puts("Please provide the filepath to a json file.");
        return 1;
    }

    FILE *fptr;
    char* filepath = argv[1];

    fptr = fopen(filepath, "r");
    if(fptr == NULL) {
        printf("%s not found\n", filepath);
        return 1;
    }

    char* jsonString = malloc(1);
    char line[100];

    // Read in the provided JSON file
    while(fgets(line, 100, fptr)) {
            jsonString = str_append(jsonString, line);
    }

    // Fill in the json data object
    size_t i;
    json_t *root;
    json_error_t error;
    // printf("JSON: %s\n", jsonString);
    root = json_loads(jsonString, 0, &error);
    free(jsonString);
    
    if(!root) {
        fprintf(stderr, "error on line %d: %s\n", error.line, error.text);
        return 1;
    }
   
    // Free memory
    json_decref(root);
    return 0;
}
