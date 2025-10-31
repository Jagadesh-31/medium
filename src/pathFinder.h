#ifndef pathFinder_H
#define pathFinder_H

#include "graph.h"
#include "stack.h"

typedef struct {
    double total_length;
    int* path;
    int path_length;
} pathFinderSolution;

// Function declarations
pathFinderSolution* solve_pathFinder(Graph* graph, int dump_node, int* quantities, int capacity);
pathFinderSolution* solve_dis_only(Graph* graph, int dump_node);
pathFinderSolution* solve_capacity_dis(Graph* graph, int dump_node, int* quantities, int capacity);
void free_pathFinder_solution(pathFinderSolution* solution);

#endif