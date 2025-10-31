#include "pathFinder.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define INF 1e18  // Very large number to represent infinity

// Main solver function - decides which algorithm to use based on capacity
pathFinderSolution* solve_pathFinder(Graph* graph, int dump_node, int* quantities, int capacity)
{
    int n = graph->num_nodes;
    
    // If capacity is disabled or only one node, use original dis (no capacity limits)
    if (capacity <= 0 || n <= 1) {
        return solve_dis_only(graph, dump_node);
    }

    // Use capacity-constrained approach when truck has limited capacity
    return solve_capacity_dis(graph, dump_node, quantities, capacity);
}

// Original dis solver - finds shortest route visiting all locations once
pathFinderSolution* solve_dis_only(Graph* graph, int dump_node) {
    int n = graph->num_nodes;
    int total_states = (1 << n);  // 2^n possible visited state combinations

    // For large problems, return empty solution
    if (n > 20) {
        pathFinderSolution* sol = (pathFinderSolution*)malloc(sizeof(pathFinderSolution));
        sol->total_length = 0.0;
        sol->path = NULL;
        sol->path_length = 0;
        return sol;
    }

    // no nodes or just depot
    if (n <= 1) {
        pathFinderSolution* sol = (pathFinderSolution*)malloc(sizeof(pathFinderSolution));
        sol->total_length = 0.0;
        if (n == 1) {
            sol->path = (int*)malloc(sizeof(int));
            sol->path[0] = 0;  // Just the depot
            sol->path_length = 1;
        } else {
            sol->path = NULL;
            sol->path_length = 0;
        }
        return sol;
    }

    // Dynamic programming tables:
    // dp[mask][v] = minimum cost to reach node v having visited nodes in 'mask'
    // parent[mask][v] = which node we came from to reach v with this mask
    double **dp = (double**)malloc(total_states * sizeof(double*));
    int    **parent = (int**)malloc(total_states * sizeof(int*));

    // Initialize DP tables with infinite distances and no parents
    for (int s = 0; s < total_states; ++s) {
        dp[s] = (double*)malloc(n * sizeof(double));
        parent[s] = (int*)malloc(n * sizeof(int));
        for (int v = 0; v < n; ++v) {
            dp[s][v] = INF;      // No path found yet
            parent[s][v] = -1;   // No parent node
        }
    }

    // Start at depot (node 0) with only depot visited - cost is 0
    dp[1 << 0][0] = 0.0;


    for (int mask = 1; mask < total_states; ++mask) {
        for (int cur = 0; cur < n; ++cur) {
            // Skip if current node not in subset or unreachable
            if (!(mask & (1 << cur)) || dp[mask][cur] >= INF) continue;

            // Try extending path to all unvisited nodes
            for (int nxt = 0; nxt < n; ++nxt) {
                if (mask & (1 << nxt)) continue;  // Skip already visited

                int new_mask = mask | (1 << nxt);  // Add next node to visited set
                double cost = dp[mask][cur] + graph->distances[cur][nxt];

                // Update if we found a cheaper path
                if (cost < dp[new_mask][nxt]) {
                    dp[new_mask][nxt] = cost;
                    parent[new_mask][nxt] = cur;  // Remember where we came from
                }
            }
        }
    }

    // Best solution visits all nodes and ends at dump/recycling center
    double best = dp[total_states - 1][dump_node];
    pathFinderSolution* sol = (pathFinderSolution*)malloc(sizeof(pathFinderSolution));
    sol->total_length = best;

    if (best >= INF) {
        // No valid path found
        sol->path = NULL;
        sol->path_length = 0;
    } else {
        // Reconstruct the optimal path by following parent pointers backwards
        Stack* stk = create_stack();
        int cur_node = dump_node;
        int cur_mask = total_states - 1;  // All nodes visited

        // Trace back from dump to depot
        while (cur_node != -1) {
            stack_push(stk, cur_node);
            if (cur_node == 0) break;  // Reached depot
            int prev = parent[cur_mask][cur_node];
            cur_mask ^= (1 << cur_node);  // Remove current node from mask
            cur_node = prev;
        }

        // Convert stack to forward-order path
        sol->path_length = stk->size;
        sol->path = (int*)malloc(sol->path_length * sizeof(int));
        for (int i = sol->path_length - 1; i >= 0; --i)
            sol->path[i] = stack_pop(stk);

        free_stack(stk);
    }

    // Clean up DP tables
    for (int s = 0; s < total_states; ++s) {
        free(dp[s]);
        free(parent[s]);
    }
    free(dp);
    free(parent);

    return sol;
}

// Capacity-aware dis solver - returns to depot when truck gets full
pathFinderSolution* solve_capacity_dis(Graph* graph, int dump_node, int* quantities, int capacity) {
    int n = graph->num_nodes;
    
    // Track which houses we've collected from
    int* visited = (int*)calloc(n, sizeof(int));
    int current_capacity = 0;  // How full the truck is
    int total_visited = 0;     // How many houses collected from
    
    // Allocate solution - worst case we visit depot many times
    pathFinderSolution* sol = (pathFinderSolution*)malloc(sizeof(pathFinderSolution));
    sol->path = (int*)malloc((2 * n) * sizeof(int));
    sol->path_length = 0;
    sol->total_length = 0.0;
    
    // Start at depot
    int current_node = 0;
    sol->path[sol->path_length++] = current_node;
    
    // Keep collecting until all houses are visited
    while (total_visited < n - 2) {  // Exclude depot and dump node from count
        int next_node = -1;
        double min_dist = INF;
        
        // Find closest unvisited house that fits in remaining capacity
        for (int i = 1; i < n - 1; i++) {  // Only consider houses (not depot/dump)
            if (!visited[i] && quantities[i] > 0 && current_capacity + quantities[i] <= capacity) {
                double dist = graph->distances[current_node][i];
                if (dist < min_dist) {
                    min_dist = dist;
                    next_node = i;
                }
            }
        }
        
        if (next_node == -1) {
            // Truck is full or no houses left that fit - return to depot to empty
            sol->total_length += graph->distances[current_node][0];
            sol->path[sol->path_length++] = 0;
            current_node = 0;
            current_capacity = 0;  // Truck emptied at depot
            continue;
        }
        
        // Drive to the chosen house and collect its waste
        sol->total_length += graph->distances[current_node][next_node];
        sol->path[sol->path_length++] = next_node;
        current_capacity += quantities[next_node];
        visited[next_node] = 1;
        total_visited++;
        current_node = next_node;
    }
    
    // After all houses, return to dump/recycling center
    sol->total_length += graph->distances[current_node][dump_node];
    sol->path[sol->path_length++] = dump_node;
    
    free(visited);
    return sol;
}

// Clean up solution memory
void free_pathFinder_solution(pathFinderSolution* sol) {
    if (!sol) return;
    if (sol->path) free(sol->path);
    free(sol);
}