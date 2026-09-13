#include "hillClimb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

/* Calculate heuristic: city block distance to goal */
static float calculate_heuristic(Node* current, Node* goal) {
    return fabs(current->x - goal->x) + fabs(current->y - goal->y);
}

char** hillClimbing(Graph graph, char start, char goal, int* checks) {
    int currIdx = findNode(graph, start);
    int goalIdx = findNode(graph, goal);
    int pathSize = 2;
    char** path = malloc(pathSize * sizeof(char*));
    path[0] = malloc(2);
    path[0][0] = start;
    path[0][1] = '\0';
    path[1] = NULL;
    Node* currNode = graph.nodes[currIdx];
    Node* goalNode = graph.nodes[goalIdx];
    int bestChild;
    float bestHeuristic;

    *checks = 0;

    while (1) {
        (*checks)++;

        if (currNode == NULL || currNode->name == goal)
            return path;

        float currentHeuristic = calculate_heuristic(currNode, goalNode);

        bestChild = -1;
        bestHeuristic = INFINITY;

        /* Compare each child by its heuristic distance to the goal,
           not by the cost of the edge used to reach it */
        for (int i = 0; i < currNode->numChildren; ++i) {
            float childHeuristic = calculate_heuristic(currNode->children[i], goalNode);

            if (bestChild == -1 || childHeuristic < bestHeuristic) {
                bestChild = i;
                bestHeuristic = childHeuristic;
            }
        }

        /* Stuck: no children at all, or none of them improve on
           where we already are -- this is hill climbing's local optimum */
        if (bestChild == -1 || bestHeuristic >= currentHeuristic) {
            return path;
        }

        currNode = currNode->children[bestChild];

        ++pathSize;
        path = realloc(path, (pathSize * sizeof(char*)));
        path[pathSize - 2] = malloc(2);
        path[pathSize - 2][0] = currNode->name;
        path[pathSize - 2][1] = '\0';
        path[pathSize - 1] = NULL;
    }
}
