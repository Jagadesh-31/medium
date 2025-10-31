#ifndef CVRP_H
#define CVRP_H

#include "graph.h"

#define NO_CAPACITY -1  // Special value to disable capacity constraints

// Structure for a single route
typedef struct {
    int* path;             // Array of node indices
    int path_length;       // Number of nodes in path
    double distance;       // Total distance for this route
    int total_quantity;    // Total waste collected in this route
} Route;

// Structure for complete CVRP solution
typedef struct {
    Route* routes;         // Array of routes
    int num_routes;        // Number of routes
    double total_distance; // Sum of all route distances
} CVRPSolution;

// Function declarations
CVRPSolution* solve_cvrp(Graph* graph, int* quantities, int capacity, int dump_node);
void free_cvrp_solution(CVRPSolution* solution);

#endif