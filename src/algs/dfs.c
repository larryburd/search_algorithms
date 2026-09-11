#include "dfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int dfs_recursive(Graph graph, int currIdx, int endIdx,
                         char visited[], char** path, int* pathIdx, int* checks) {
    visited[currIdx] = 1;
    ++checks;
    path[*pathIdx] = malloc(2);
    path[*pathIdx][0] = graph.nodes[currIdx]->name;
    path[*pathIdx][1] = '\0';
    (*pathIdx)++;

    /* If we have reached our goal */
    if (currIdx == endIdx) {
        return 1;
    }

    Node* currNode = graph.nodes[currIdx];

    /* Explore children of current node */
    for (int i = 0; i < currNode->numChildren; ++i) {
        Node* childNode = currNode->children[i];

        /* Find child index */
        int childIdx = -1;
        for (int j = 0; j < graph.numNodes; ++j) {
            if (graph.nodes[j] == childNode) {
                childIdx = j;
                break;
            }
        }

        if (childIdx != -1 && !visited[childIdx]) {
            if (dfs_recursive(graph, childIdx, endIdx, visited, path, pathIdx, checks)) {
                return 1; /* Goal is found */
            }
        }
    }

    /* Backtrack if goal not found */
    (*pathIdx)--;

    return 0; /* Goal isn't found in this path */
}

char** dfs_search(Graph graph, char start, char goal, int* checks) {
    int startIdx = -1;
    int endIdx = -1;
    int pathIdx = 0;
    char** path = malloc(sizeof(char*) * (MAX_PATH_LENGTH + 1));
    char visited[MAX_NODES];

    /* Initialize visited and path arrays */
    path[0] = NULL;
    memset(visited, 0, MAX_NODES);

    /* Find start and end nodes */
    for (int i = 0; i < graph.numNodes; ++i) {
        if (graph.nodes[i]->name == start) {
            startIdx = i;
        } else if (graph.nodes[i]->name == goal) {
            endIdx = i;
        }
    }

    if (startIdx == -1 || endIdx == -1) {
        return path;
    }

    /* perform the search */
    dfs_recursive(graph, startIdx, endIdx, visited, path, &pathIdx, checks);

    /* Add NULL termination to end of path array */
    path[pathIdx] = NULL;

    return path;
}
