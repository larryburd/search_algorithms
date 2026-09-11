#include "greedy.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Frontier node for priority queue */
typedef struct {
    int nodeIdx;
    float heuristic;
} FrontierNode;

/* Calculate heuristic: city block distance to goal */
float calculate_heuristic(Node* current, Node* goal) {
    return fabs(current->x - goal->x) + fabs(current->y - goal->y);
}

/* Insert node into frontier, maintaining sorted order by heuristic */
void insert_into_frontier(FrontierNode frontier[], int* frontierSize,
                         int nodeIdx, float heuristic) {
    if (*frontierSize >= MAX_FRONTIER) {
        return;
    }

    int i = *frontierSize;
    /* Find correct position to insert */
    while (i > 0 && frontier[i - 1].heuristic > heuristic) {
        frontier[i] = frontier[i - 1];
        i--;
    }

    frontier[i].nodeIdx = nodeIdx;
    frontier[i].heuristic = heuristic;
    (*frontierSize)++;
}

/* Remove and return node with lowest heuristic from frontier */
int pop_frontier(FrontierNode frontier[], int* frontierSize) {
    if (*frontierSize == 0) {
        return -1;
    }

    int nodeIdx = frontier[0].nodeIdx;
    (*frontierSize)--;

    /* Shift all elements down */
    for (int i = 0; i < *frontierSize; i++) {
        frontier[i] = frontier[i + 1];
    }

    return nodeIdx;
}

/* Check if node is in frontier */
int is_in_frontier(FrontierNode frontier[], int frontierSize, int nodeIdx) {
    for (int i = 0; i < frontierSize; i++) {
        if (frontier[i].nodeIdx == nodeIdx) {
            return 1;
        }
    }
    return 0;
}

char** greedy_search(Graph graph, char start, char goal, int* checks) {
    int startIdx = -1;
    int endIdx = -1;
    int pathIdx = 0;
    char** path = malloc(sizeof(char*) * (MAX_PATH_LENGTH + 1));
    char visited[MAX_NODES];
    int parent[MAX_NODES];
    FrontierNode frontier[MAX_FRONTIER];
    int frontierSize = 0;

    /* Initialize arrays */
    path[0] = NULL;
    memset(visited, 0, MAX_NODES);
    memset(parent, -1, MAX_NODES * sizeof(int));

    /* Find start and goal nodes */
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

    /* Add start node to frontier */
    float startHeuristic = calculate_heuristic(graph.nodes[startIdx],
                                               graph.nodes[endIdx]);
    insert_into_frontier(frontier, &frontierSize, startIdx, startHeuristic);

    /* Greedy search loop */
    while (frontierSize > 0) {
        int currIdx = pop_frontier(frontier, &frontierSize);

        if (visited[currIdx]) {
            continue;
        }

        visited[currIdx] = 1;
        ++checks;

        /* Check if we reached the goal */
        if (currIdx == endIdx) {
            /* Reconstruct path from parent array */
            int temp[MAX_PATH_LENGTH];
            int count = 0;
            int idx = endIdx;

            while (idx != -1) {
                temp[count++] = idx;
                idx = parent[idx];
            }

            /* Reverse and build path array */
            for (int i = count - 1; i >= 0; i--) {
                path[pathIdx] = malloc(2);
                path[pathIdx][0] = graph.nodes[temp[i]]->name;
                path[pathIdx][1] = '\0';
                pathIdx++;
            }
            path[pathIdx] = NULL;

            return path;
        }

        /* Explore children */
        Node* currNode = graph.nodes[currIdx];
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

            /* Add unvisited children to frontier */
            if (childIdx != -1 && !visited[childIdx] &&
                !is_in_frontier(frontier, frontierSize, childIdx)) {
                float childHeuristic = calculate_heuristic(graph.nodes[childIdx],
                                                          graph.nodes[endIdx]);
                insert_into_frontier(frontier, &frontierSize, childIdx, childHeuristic);
                parent[childIdx] = currIdx;
            }
        }
    }

    /* No path found */
    return path;
}

char** greedy_search_weighted(Graph graph, char start, char goal, int* checks) {
    int startIdx = -1;
    int endIdx = -1;
    int pathIdx = 0;
    char** path = malloc(sizeof(char*) * (MAX_PATH_LENGTH + 1));
    char visited[MAX_NODES];
    int parent[MAX_NODES];
    FrontierNode frontier[MAX_FRONTIER];
    int frontierSize = 0;

    /* Initialize arrays */
    path[0] = NULL;
    memset(visited, 0, MAX_NODES);
    memset(parent, -1, MAX_NODES * sizeof(int));

    /* Find start and goal nodes */
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

    /* Add start node to frontier */
    float startHeuristic = calculate_heuristic(graph.nodes[startIdx],
                                               graph.nodes[endIdx]);
    insert_into_frontier(frontier, &frontierSize, startIdx, startHeuristic);

    /* Greedy search loop with edge weights */
    while (frontierSize > 0) {
        int currIdx = pop_frontier(frontier, &frontierSize);

        if (visited[currIdx]) {
            continue;
        }

        visited[currIdx] = 1;
        ++checks;

        /* Check if we reached the goal */
        if (currIdx == endIdx) {
            /* Reconstruct path from parent array */
            int temp[MAX_PATH_LENGTH];
            int count = 0;
            int idx = endIdx;

            while (idx != -1) {
                temp[count++] = idx;
                idx = parent[idx];
            }

            /* Reverse and build path array */
            for (int i = count - 1; i >= 0; i--) {
                path[pathIdx] = malloc(2);
                path[pathIdx][0] = graph.nodes[temp[i]]->name;
                path[pathIdx][1] = '\0';
                pathIdx++;
            }
            path[pathIdx] = NULL;

            return path;
        }

        /* Explore children */
        Node* currNode = graph.nodes[currIdx];
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

            /* Add unvisited children to frontier */
            if (childIdx != -1 && !visited[childIdx] &&
                !is_in_frontier(frontier, frontierSize, childIdx)) {
                float childHeuristic = calculate_heuristic(graph.nodes[childIdx],
                                                          graph.nodes[endIdx]);
                /* Include edge weight in priority calculation */
                float edgeWeight = currNode->childCosts[i];
                float priority = edgeWeight + childHeuristic;
                insert_into_frontier(frontier, &frontierSize, childIdx, priority);
                parent[childIdx] = currIdx;
            }
        }
    }

    /* No path found */
    return path;
}
