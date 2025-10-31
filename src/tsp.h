#ifndef TSP_H
#define TSP_H

#include "graph.h"
#include "stack.h"

// Structure to hold TSP solution
typedef struct {
    double total_length;    // Total path length
    int* path;             // Array of node indices in optimal path
    int path_length;       // Number of nodes in path
} TSPSolution;

// Function declarations
TSPSolution* solve_tsp(Graph* graph, int dump_node);
void free_tsp_solution(TSPSolution* solution);

#endif