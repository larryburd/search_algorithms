#ifndef NODE_H
#define NODE_H

#define INIT_CAP 10
#define MAX_NODES 20

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* A node data structure for use in searching */
typedef struct Node{
    char name;
    struct Node** parents;
    struct Node** children;
    int numParents;
    int capParents;
    int numChildren;
    int capChildren;
    int x;
    int y;
    float* childCosts;
} Node;

typedef struct {
    Node* nodes[MAX_NODES];
    int numNodes;
} Graph;

/* Function declarations */
Node* create_node(char name, int x, int y);
void addParent(Node* node, Node* parent);
void addChild(Node* node, Node* child, float cost);
void free_node(Node* node);
int findNode(Graph graph, char name);

#endif
