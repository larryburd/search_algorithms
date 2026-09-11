#include "bfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Queue structure for BFS */
typedef struct {
    int items[MAX_QUEUE];
    int front;
    int rear;
} Queue;

Queue* create_queue() {
    Queue* q = malloc(sizeof(Queue));
    q->front = 0;
    q->rear = -1;
    return q;
}

void enqueue(Queue* q, int value) {
    if (q->rear < MAX_QUEUE - 1) {
        q->rear++;
        q->items[q->rear] = value;
    }
}

int dequeue(Queue* q) {
    if (q->front <= q->rear) {
        return q->items[q->front++];
    }
    return -1;
}

int is_empty(Queue* q) {
    return q->front > q->rear;
}

void free_queue(Queue* q) {
    free(q);
}

char** bfs_search(Graph graph, char start, char goal, int* checks) {
    int startIdx = -1;
    int endIdx = -1;
    int pathIdx = 0;
    char** path = malloc(sizeof(char*) * (MAX_PATH_LENGTH + 1));
    char visited[MAX_NODES];
    int parent[MAX_NODES];

    /* Initialize visited and parent arrays */
    path[0] = NULL;
    memset(visited, 0, MAX_NODES);
    memset(parent, -1, MAX_NODES * sizeof(int));

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

    /* Perform BFS */
    Queue* q = create_queue();
    enqueue(q, startIdx);
    visited[startIdx] = 1;
    (*checks)++;

    while (!is_empty(q)) {
        int currIdx = dequeue(q);
        Node* currNode = graph.nodes[currIdx];

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

            free_queue(q);
            return path;
        }

        /* Explore children */
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

            /* Enqueue if not visited */
            if (childIdx != -1 && !visited[childIdx]) {
                visited[childIdx] = 1;
                (*checks)++;
                parent[childIdx] = currIdx;
                enqueue(q, childIdx);
            }
        }
    }

    free_queue(q);
    return path;
}
