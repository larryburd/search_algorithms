#include "node.h"
#include <stdlib.h>

Node* create_node(char name, int x, int y) {
    Node* node = malloc(sizeof(Node));
    node->name = name;
    node->numParents = 0;
    node->capParents = INIT_CAP;
    node->parents = malloc(INIT_CAP * sizeof(struct Node*));

    node->numChildren = 0;
    node->capChildren = INIT_CAP;
    node->children = malloc(INIT_CAP * sizeof(struct Node*));

    node->x = x;
    node->y = y;
    node->childCosts = NULL;
    node->parentCosts = NULL;

    return node;
}

void addParent(Node* node, Node* parent) {
    if (node->numParents >= node->capParents) {
        node->capParents *= 2;
        node->parents = realloc(node->parents, node->capParents * sizeof(struct Node*));
    }

    node->parents[node->numParents++] = parent;
}

void addChild(Node* node, Node* child) {
    if (node->numChildren >= node->capChildren) {
        node->capChildren *= 2;
        node->children = realloc(node->children, node->capChildren * sizeof(struct Node*));
    }

    node->children[node->numChildren++] = child;
}

void free_node(Node* node) {
    free(node->parents);
    free(node->children);
    free(node);
}
