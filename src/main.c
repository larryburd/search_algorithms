#include "str_util/string_util.h"
#include "algs/dfs.h"
#include "algs/bfs.h"
#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include <jansson.h>

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

    // Fill in the json data object
    size_t i = 0;
    json_t *root;
    json_error_t error;
    // printf("JSON: %s\n", jsonString);
    root = json_loads(jsonString, 0, &error);
    free(jsonString);
    
    if(!root) {
        fprintf(stderr, "error on line %d: %s\n", error.line, error.text);
        return 1;
    }
    
    Graph graph;
    graph.numNodes = 0;
    const char *cityName;
    json_t *cityData;

    json_object_foreach(root, cityName, cityData) {
        int x = json_integer_value(json_object_get(cityData, "x"));
        int y = json_integer_value(json_object_get(cityData, "y"));

        Node* node = create_node(cityName[0], x, y);
        graph.nodes[i] = node;
        i++;
    }
    graph.numNodes = i;

    // Build parent-child relationships
    json_object_foreach(root, cityName, cityData) {
        json_t *parents = json_object_get(cityData, "parents");
        // Find this city's node (it's the child)
        int childIdx = -1;
        for (int i = 0; i < graph.numNodes; i++) {
            if (graph.nodes[i]->name == cityName[0]) {
                childIdx = i;
                break;
            }
        }
    
        // Get all parents of this city
        if (json_is_array(parents)) {
            size_t idx;
            json_t *parentNameObj;
            json_array_foreach(parents, idx, parentNameObj) {
                const char *parentName = json_string_value(parentNameObj);

                // Find parent node index
                int parentIdx = -1;
                for (int i = 0; i < graph.numNodes; i++) {
                    if (graph.nodes[i]->name == parentName[0]) {
                        parentIdx = i;
                        break;
                    }
                }

                // Add current city as child to the parent
                if (parentIdx != -1 && childIdx != -1) {
                    addChild(graph.nodes[parentIdx], graph.nodes[childIdx]);
                }
            }
        }
    }

    json_decref(root);

    // Perform DFS search from A to S
    printf("\n========================================\n");
    printf("DEPTH FIRST SEARCH (DFS)\n");
    printf("Start: A → Goal: S\n");
    printf("========================================\n");

    char** path = dfs_search(graph, 'A', 'S');
    print_path(path);
    free_path(path);

    // Perform BFS search from A to S
    printf("\n========================================\n");
    printf("BREADTH FIRST SEARCH (BFS)\n");
    printf("Start: A → Goal: S\n");
    printf("========================================\n");

    path = bfs_search(graph, 'A', 'S');
    print_path(path);
    free_path(path);

    // Free graph nodes
    for (int i = 0; i < graph.numNodes; i++) {
        free_node(graph.nodes[i]);
    }

    return 0;
}
