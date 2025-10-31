#include "tsp.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

// Very large number to represent infinity
#define INF 1e18


// Start at node 0, visit all nodes, end at dump node
TSPSolution* solve_tsp(Graph* graph, int dump_node) {
    int n = graph->num_nodes;
    int total_states = (1 << n);  // Total possible subsets (bitmask)
    
    // Safety check: if too many nodes, use simple fallback solution
    if (n > 20) {
        TSPSolution* solution = (TSPSolution*)malloc(sizeof(TSPSolution));
        if (n >= 2) {
            // Simple path: Truck -> Dump
            solution->total_length = graph->distances[0][dump_node];
            solution->path = (int*)malloc(2 * sizeof(int));
            solution->path[0] = 0;
            solution->path[1] = dump_node;
            solution->path_length = 2;
        } else {
            solution->total_length = 0.0;
            solution->path = NULL;
            solution->path_length = 0;
        }
        return solution;
    }
    
    // Handle trivial cases
    if (n <= 1) {
        TSPSolution* solution = (TSPSolution*)malloc(sizeof(TSPSolution));
        solution->total_length = 0.0;
        if (n == 1) {
            solution->path = (int*)malloc(sizeof(int));
            solution->path[0] = 0;
            solution->path_length = 1;
        } else {
            solution->path = NULL;
            solution->path_length = 0;
        }
        return solution;
    }
    
    // DP table: dp[mask][i] = minimum cost to visit nodes in 'mask' ending at node i
    double** dp = (double**)malloc(total_states * sizeof(double*));
    int** parent = (int**)malloc(total_states * sizeof(int*));
    
    for (int i = 0; i < total_states; i++) {
        dp[i] = (double*)malloc(n * sizeof(double));
        parent[i] = (int*)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            dp[i][j] = INF;      // Initialize with infinity
            parent[i][j] = -1;   // No parent initially
        }
    }
    
    // Start at truck node (node 0) with only that node visited
    dp[1 << 0][0] = 0.0;
    
    // Dynamic Programming: build solutions for all subsets
    for (int mask = 1; mask < total_states; mask++) {
        for (int current_node = 0; current_node < n; current_node++) {
            // Skip if current node not in subset or no valid path exists
            if (!(mask & (1 << current_node)) || dp[mask][current_node] >= INF) {
                continue;
            }
            
            // Try extending path to all unvisited nodes
            for (int next_node = 0; next_node < n; next_node++) {
                if (mask & (1 << next_node)) continue;  // Skip if already visited
                
                int new_mask = mask | (1 << next_node);
                double new_cost = dp[mask][current_node] + graph->distances[current_node][next_node];
                
                // Update if we found a better path
                if (new_cost < dp[new_mask][next_node]) {
                    dp[new_mask][next_node] = new_cost;
                    parent[new_mask][next_node] = current_node;
                }
            }
        }
    }
    
    // Find optimal path ending at dump node
    double best_cost = dp[total_states - 1][dump_node];
    
    // Reconstruct optimal path using stack
    TSPSolution* solution = (TSPSolution*)malloc(sizeof(TSPSolution));
    solution->total_length = best_cost;
    
    if (best_cost >= INF) {
        // No valid path found
        solution->path = NULL;
        solution->path_length = 0;
    } else {
        // Reconstruct path backwards from dump node to truck node
        Stack* path_stack = create_stack();
        int current_node = dump_node;
        int current_mask = total_states - 1;
        
        // Backtrack through parents to build path
        while (current_node != -1) {
            stack_push(path_stack, current_node);
            
            // Stop when we reach the start node (truck)
            if (current_node == 0) break;
            
            int prev_node = parent[current_mask][current_node];
            current_mask ^= (1 << current_node);  // Remove current node from mask
            current_node = prev_node;
        }
        
        // Transfer path from stack to array
        solution->path_length = path_stack->size;
        solution->path = (int*)malloc(solution->path_length * sizeof(int));
        for (int i = solution->path_length - 1; i >= 0; i--) {
            solution->path[i] = stack_pop(path_stack);
        }
        
        free_stack(path_stack);
    }
    
    // Free DP tables
    for (int i = 0; i < total_states; i++) {
        free(dp[i]);
        free(parent[i]);
    }
    free(dp);
    free(parent);
    
    return solution;
}

// Frees memory allocated for TSP solution
void free_tsp_solution(TSPSolution* solution) {
    if (solution == NULL) return;
    if (solution->path != NULL) {
        free(solution->path);
    }
    free(solution);
}