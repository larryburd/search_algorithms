#ifndef DFS_H
#define DFS_H

#include "node.h"
#include <stdlib.h>
#include <string.h>

#define MAX_PATH_LENGTH 50
#define MAX_NODES 20

/**
 * dfs_search - Find path from start to goal using Depth First Search
 * @graph: The graph to search
 * @start: Name of start node (e.g., 'A')
 * @goal: Name of goal node (e.g., 'S')
 * 
 * Returns: Null-terminated array of strings representing the path,
 *          or NULL if no path exists
 *          Caller is responsible for freeing with free_path()
 */
char** dfs_search(Graph graph, char start, char goal);

/**
 * dfs_recursive - Helper function that performs the actual DFS recursion
 * @graph: The graph to search
 * @current_idx: Index of current node being explored
 * @goal_idx: Index of goal node
 * @visited: Array tracking which nodes have been visited
 * @path: Array being built to store the path
 * @path_idx: Current position in the path array
 * 
 * Returns: 1 if goal found, 0 if not
 *          Modifies path array with nodes visited during search
 */
static int dfs_recursive(Graph graph, int current_idx, int goal_idx, 
                        char visited[], char** path, int* path_idx);

/**
 * free_path - Free allocated path array
 * @path: The path array to free
 */
void free_path(char** path);

/**
 * print_path - Print the path in readable format
 * @path: The path array to print
 */
void print_path(char** path);

#endif
