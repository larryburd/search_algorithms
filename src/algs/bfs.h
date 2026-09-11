#ifndef BFS_H
#define BFS_H

#include "../node.h"
#include "path_util.h"

#define MAX_PATH_LENGTH 50
#define MAX_QUEUE 100

/* Function declarations */
char** bfs_search(Graph graph, char start, char goal, int* checks);

#endif
