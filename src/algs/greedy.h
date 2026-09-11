#ifndef GREEDY_H
#define GREEDY_H

#include "../node.h"
#include "path_util.h"

#define MAX_PATH_LENGTH 50
#define MAX_FRONTIER 100

/* Function declarations */
char** greedy_search(Graph graph, char start, char goal, int* checks);
char** greedy_search_weighted(Graph graph, char start, char goal, int* checks);

#endif
