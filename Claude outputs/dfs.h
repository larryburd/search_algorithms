#ifndef DFS_H
#define DFS_H

#include "../node.h"
#include "path_util.h"

#define MAX_PATH_LENGTH 50

/* Function declarations */
char** dfs_search(Graph graph, char start, char goal);

#endif
