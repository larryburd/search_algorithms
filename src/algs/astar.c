#include "astar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

/* Frontier node for priority queue, sorted by f(n) = g(n) + h(n) */
typedef struct {
    int nodeIdx;
    float priority;
} FrontierNode;

/* Calculate heuristic: city block distance to goal */
static float calculate_heuristic(Node* current, Node* goal) {
    return fabs(current->x - goal->x) + fabs(current->y - goal->y);
}

/* Insert node into frontier, maintaining sorted order by priority (lowest first) */
static void insert_into_frontier(FrontierNode frontier[], int* frontierSize,
                                  int nodeIdx, float priority) {
    if (*frontierSize >= MAX_FRONTIER) {
        return;
    }

    int i = *frontierSize;
    while (i > 0 && frontier[i - 1].priority > priority) {
        frontier[i] = frontier[i - 1];
        i--;
    }

    frontier[i].nodeIdx = nodeIdx;
    frontier[i].priority = priority;
    (*frontierSize)++;
}

/* Remove and return node with lowest priority from frontier */
static int pop_frontier(FrontierNode frontier[], int* frontierSize) {
    if (*frontierSize == 0) {
        return -1;
    }

    int nodeIdx = frontier[0].nodeIdx;
    (*frontierSize)--;

    for (int i = 0; i < *frontierSize; i++) {
        frontier[i] = frontier[i + 1];
    }

    return nodeIdx;
}

char** astar_search(Graph graph, char start, char goal, int* checks) {
    int startIdx = -1;
    int endIdx = -1;
    int pathIdx = 0;
    char** path = malloc(sizeof(char*) * (MAX_PATH_LENGTH + 1));
    char visited[MAX_NODES];
    int parent[MAX_NODES];
    float gCost[MAX_NODES];
    FrontierNode frontier[MAX_FRONTIER];
    int frontierSize = 0;

    /* Initialize arrays */
    *checks = 0;
    path[0] = NULL;
    memset(visited, 0, MAX_NODES);
    memset(parent, -1, MAX_NODES * sizeof(int));
    for (int i = 0; i < MAX_NODES; ++i) {
        gCost[i] = FLT_MAX;
    }

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

    /* g(start) = 0, f(start) = h(start) */
    gCost[startIdx] = 0.0f;
    float startPriority = calculate_heuristic(graph.nodes[startIdx], graph.nodes[endIdx]);
    insert_into_frontier(frontier, &frontierSize, startIdx, startPriority);

    /* A* search loop */
    while (frontierSize > 0) {
        int currIdx = pop_frontier(frontier, &frontierSize);

        /* Lazy deletion: a cheaper entry for this node may already have been
           expanded, leaving a stale, more expensive copy behind in the frontier */
        if (visited[currIdx]) {
            continue;
        }

        visited[currIdx] = 1;
        (*checks)++;

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

            if (childIdx == -1 || visited[childIdx]) {
                continue;
            }

            /* Cost to reach child by going through the current node */
            float tentativeG = gCost[currIdx] + currNode->childCosts[i];

            /* Only take this route if it beats any previously known route to child */
            if (tentativeG < gCost[childIdx]) {
                gCost[childIdx] = tentativeG;
                parent[childIdx] = currIdx;

                float h = calculate_heuristic(graph.nodes[childIdx], graph.nodes[endIdx]);
                float f = tentativeG + h;
                insert_into_frontier(frontier, &frontierSize, childIdx, f);
            }
        }
    }

    /* No path found */
    return path;
}
