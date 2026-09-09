#include "str_util/string_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"
#include <jansson.h>

#define MAX_NODES 20
#define MAX_PATH 50

typedef struct {
    char* cities;
    Node* nodes[MAX_NODES];
    int num_nodes;
} Graph;

typedef struct {
    char path[MAX_PATH];
    int found;
} DFSResult;

// Global data for DFS traversal
char visited[MAX_NODES];
char dfs_path[MAX_PATH];
int path_length = 0;

Graph graph;
char* city_names[] = {"A", "B", "C", "D", "E", "F", "G", "H", "M", "N", "P", "S"};

int get_city_index(const char* city) {
    for (int i = 0; i < graph.num_nodes; i++) {
        if (strcmp(city_names[i], city) == 0) {
            return i;
        }
    }
    return -1;
}

// Calculate city block distance
float city_block_distance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// DFS recursive function
int dfs(int current_idx, int goal_idx, int depth) {
    printf("\nStep %d: Visiting %s at (%d, %d)\n",
           path_length + 1, city_names[current_idx],
           graph.nodes[current_idx]->x, graph.nodes[current_idx]->y);

    visited[current_idx] = 1;
    dfs_path[path_length++] = current_idx;

    // Check if we reached the goal
    if (current_idx == goal_idx) {
        printf("✓ GOAL FOUND: %s\n", city_names[goal_idx]);
        return 1;
    }

    // Print available children and their distances to goal
    Node* current_node = graph.nodes[current_idx];
    if (current_node->numChildren > 0) {
        printf("  Children available: ");
        for (int i = 0; i < current_node->numChildren; i++) {
            int child_idx = -1;
            for (int j = 0; j < graph.num_nodes; j++) {
                if (graph.nodes[j] == current_node->children[i]) {
                    child_idx = j;
                    break;
                }
            }
            if (child_idx != -1) {
                float dist_to_goal = city_block_distance(
                    graph.nodes[child_idx]->x, graph.nodes[child_idx]->y,
                    graph.nodes[goal_idx]->x, graph.nodes[goal_idx]->y
                );
                printf("%s(dist=%.0f) ", city_names[child_idx], dist_to_goal);
            }
        }
        printf("\n");
    }

    // Explore children in order (DFS follows pointer list order)
    for (int i = 0; i < current_node->numChildren; i++) {
        int child_idx = -1;
        for (int j = 0; j < graph.num_nodes; j++) {
            if (graph.nodes[j] == current_node->children[i]) {
                child_idx = j;
                break;
            }
        }

        if (child_idx != -1 && !visited[child_idx]) {
            printf("  → Choosing %s (unvisited child)\n", city_names[child_idx]);
            if (dfs(child_idx, goal_idx, depth + 1)) {
                return 1;
            }
        }
    }

    // Backtrack
    printf("  ↶ Backtracking from %s\n", city_names[current_idx]);
    path_length--;
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        puts("Please provide the filepath to a json file.");
        return 1;
    }

    FILE *fptr;
    char* filepath = argv[1];

    fptr = fopen(filepath, "r");
    if(fptr == NULL) {
        printf("%s not found\n", filepath);
        return 1;
    }

    char* jsonString = malloc(1);
    char line[100];

    // Read in the provided JSON file
    while(fgets(line, 100, fptr)) {
        jsonString = str_append(jsonString, line);
    }
    fclose(fptr);

    // Parse JSON
    json_t *root;
    json_error_t error;
    root = json_loads(jsonString, 0, &error);
    free(jsonString);

    if(!root) {
        fprintf(stderr, "error on line %d: %s\n", error.line, error.text);
        return 1;
    }

    // Build graph from JSON
    graph.num_nodes = 0;
    const char *city_name;
    json_t *city_data;

    json_object_foreach(root, city_name, city_data) {
        int x = json_integer_value(json_object_get(city_data, "x"));
        int y = json_integer_value(json_object_get(city_data, "y"));

        int idx = get_city_index(city_name);
        if (idx >= 0) {
            graph.nodes[idx] = create_node(idx, x, y);
            graph.num_nodes++;
        }
    }

    // Build parent-child relationships
    json_object_foreach(root, city_name, city_data) {
        json_t *parents = json_object_get(city_data, "parents");
        int child_idx = get_city_index(city_name);

        if (json_is_array(parents)) {
            size_t parent_idx;
            json_t *parent_name_obj;
            json_array_foreach(parents, parent_idx, parent_name_obj) {
                const char *parent_name = json_string_value(parent_name_obj);
                int parent_idx_num = get_city_index(parent_name);
                if (parent_idx_num >= 0 && child_idx >= 0) {
                    addChild(graph.nodes[parent_idx_num], graph.nodes[child_idx]);
                }
            }
        }
    }

    // Initialize visited array
    memset(visited, 0, MAX_NODES);

    // Run DFS from A (index 0) to S (index 11)
    printf("========================================\n");
    printf("DEPTH FIRST SEARCH (DFS)\n");
    printf("Start: A (0,0) → Goal: S (100,100)\n");
    printf("========================================\n");

    int start_idx = get_city_index("A");
    int goal_idx = get_city_index("S");

    if (dfs(start_idx, goal_idx, 0)) {
        printf("\n✓ PATH FOUND!\n");
        printf("Path: ");
        for (int i = 0; i < path_length; i++) {
            printf("%s", city_names[dfs_path[i]]);
            if (i < path_length - 1) printf(" → ");
        }
        printf("\nPath Length: %d nodes\n", path_length);
    } else {
        printf("\n✗ No path found to goal S\n");
    }

    printf("\n========================================\n");

    // Free memory
    json_decref(root);
    for (int i = 0; i < graph.num_nodes; i++) {
        free_node(graph.nodes[i]);
    }

    return 0;
}
