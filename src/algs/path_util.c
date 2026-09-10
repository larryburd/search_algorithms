#include "path_util.h"
#include <stdio.h>
#include <stdlib.h>

void free_path(char** path) {
    if (path == NULL) {
        return;
    }

    /* Free each string in the path */
    for (int i = 0; path[i] != NULL; i++) {
        free(path[i]);
    }

    /* Free the array itself */
    free(path);
}

void print_path(char** path) {
    if (path == NULL) {
        printf("No path found\n");
        return;
    }

    printf("Path: ");
    for (int i = 0; path[i] != NULL; i++) {
        printf("%s", path[i]);
        if (path[i + 1] != NULL) {
            printf(" → ");
        }
    }
    printf("\n");
}
