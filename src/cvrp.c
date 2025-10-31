#include "cvrp.h"
#include "tsp.h"
#include "bst.h"
#include "stack.h"
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <string.h>

#define EPSILON 1e-9

typedef struct {
    int node_index;
    double distance;
    int quantity;
} HouseInfo;

// Compare by increasing distance; tie-break by larger quantity
static int compare_houses(const void* a, const void* b) {
    const HouseInfo* ha = (const HouseInfo*)a;
    const HouseInfo* hb = (const HouseInfo*)b;
    double diff = ha->distance - hb->distance;
    if (diff < -EPSILON) return -1;
    if (diff > EPSILON)  return 1;
    if (ha->quantity > hb->quantity) return -1;
    if (ha->quantity < hb->quantity) return 1;
    return 0;
}

// Use BST to track unserved houses for efficient lookup
static TreeNode* create_unserved_bst(int num_houses) {
    TreeNode* root = NULL;
    for (int i = 1; i <= num_houses; i++) {
        root = insert_bst(root, i);
    }
    return root;
}

// Helper function to collect unserved nodes into an array
static void collect_unserved_nodes(TreeNode* root, int* array, int* index) {
    if (root == NULL) return;
    collect_unserved_nodes(root->left, array, index);
    array[(*index)++] = root->val;
    collect_unserved_nodes(root->right, array, index);
}

// Check if any unserved house can fit in remaining capacity using BST traversal
static int can_any_house_fit(TreeNode* unserved_root, int* quantities, int remaining_capacity, int capacity) {
    if (unserved_root == NULL) return 0;
    
    // Inorder traversal to check all unserved houses
    int left_result = can_any_house_fit(unserved_root->left, quantities, remaining_capacity, capacity);
    if (left_result) return 1;
    
    int house_index = unserved_root->val;
    int q = quantities[house_index - 1];
    if (q <= remaining_capacity) {
        return 1;
    }
    
    int right_result = can_any_house_fit(unserved_root->right, quantities, remaining_capacity, capacity);
    return right_result;
}

// Choose nearest feasible unserved house
static int find_best_house(Graph* graph, TreeNode* unserved_root, int* quantities,
                           int current_node, int current_load, int capacity, int num_houses) {
    HouseInfo candidates[256];
    int count = 0;
    
    // Use array for traversal since we need all candidates for sorting
    int unserved_nodes[256];
    int index = 0;
    
    // Collect all unserved nodes into array
    collect_unserved_nodes(unserved_root, unserved_nodes, &index);
    
    for (int i = 0; i < index; i++) {
        int house_idx = unserved_nodes[i];
        int q = quantities[house_idx - 1];
        
        // CRITICAL: Check if adding this house would exceed TOTAL capacity
        if (capacity == NO_CAPACITY || (current_load + q <= capacity)) {
            candidates[count].node_index = house_idx;
            candidates[count].distance = graph->distances[current_node][house_idx];
            candidates[count].quantity = q;
            count++;
        }
    }
    
    if (count == 0) return -1;
    qsort(candidates, count, sizeof(HouseInfo), compare_houses);
    return candidates[0].node_index;
}

static int all_served(TreeNode* unserved_root) {
    return unserved_root == NULL;
}

static Route* create_empty_route(int max_size) {
    Route* route = (Route*)malloc(sizeof(Route));
    route->path = (int*)malloc(max_size * sizeof(int));
    route->path_length = 0;
    route->distance = 0.0;
    route->total_quantity = 0;
    return route;
}

static void add_to_route(Route* route, int node) {
    route->path[route->path_length++] = node;
}

// Helper function to remove node from BST
static TreeNode* remove_from_bst(TreeNode* root, int val) {
    if (root == NULL) return NULL;
    
    if (val < root->val) {
        root->left = remove_from_bst(root->left, val);
    } else if (val > root->val) {
        root->right = remove_from_bst(root->right, val);
    } else {
        // Node to be deleted found
        if (root->left == NULL) {
            TreeNode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            TreeNode* temp = root->left;
            free(root);
            return temp;
        }
        
        // Node with two children: get inorder successor
        TreeNode* temp = root->right;
        while (temp && temp->left != NULL) {
            temp = temp->left;
        }
        
        // Copy the inorder successor's content to this node
        root->val = temp->val;
        
        // Delete the inorder successor
        root->right = remove_from_bst(root->right, temp->val);
    }
    return root;
}

CVRPSolution* solve_cvrp(Graph* graph, int* quantities, int capacity, int dump_node) {
    // If capacity is disabled, use TSP instead
    if (capacity == NO_CAPACITY) {
        TSPSolution* tsp_sol = solve_tsp(graph, dump_node);
        CVRPSolution* cvrp_sol = (CVRPSolution*)malloc(sizeof(CVRPSolution));
        cvrp_sol->num_routes = 1;
        cvrp_sol->total_distance = tsp_sol->total_length;
        cvrp_sol->routes = (Route*)malloc(sizeof(Route));
        
        cvrp_sol->routes[0].path_length = tsp_sol->path_length;
        cvrp_sol->routes[0].path = (int*)malloc(tsp_sol->path_length * sizeof(int));
        memcpy(cvrp_sol->routes[0].path, tsp_sol->path, tsp_sol->path_length * sizeof(int));
        
        cvrp_sol->routes[0].distance = tsp_sol->total_length;
        cvrp_sol->routes[0].total_quantity = 0;
        for (int i = 0; i < graph->num_nodes - 2; i++) {
            cvrp_sol->routes[0].total_quantity += quantities[i];
        }
        
        free_tsp_solution(tsp_sol);
        return cvrp_sol;
    }

    int num_houses = graph->num_nodes - 2;
    
    // Use BST to track unserved houses
    TreeNode* unserved_root = create_unserved_bst(num_houses);

    CVRPSolution* solution = (CVRPSolution*)malloc(sizeof(CVRPSolution));
    solution->routes = NULL;
    solution->num_routes = 0;
    solution->total_distance = 0.0;

    int max_routes = 20;
    solution->routes = (Route*)malloc(max_routes * sizeof(Route));

    // Prevalidate capacity constraints
    for (int i = 0; i < num_houses; ++i) {
        if (quantities[i] > capacity) {
            fprintf(stderr, "Infeasible instance: house %d demand %d exceeds capacity %d\n",
                    i + 1, quantities[i], capacity);
        }
    }

    while (!all_served(unserved_root)) {
        Route* route = create_empty_route(graph->num_nodes + 2);
        int current_node = 0;
        int current_load = 0;
        add_to_route(route, current_node);

        int added_any = 0;
        
        for (;;) {
            // Use BST to find best house that fits TOTAL capacity
            int next_house = find_best_house(graph, unserved_root, quantities, current_node, current_load, capacity, num_houses);
            if (next_house == -1) break;

            int q = quantities[next_house - 1];
            
            // Double-check capacity (should never fail with the fixed find_best_house)
            if (current_load + q > capacity) {
                break;
            }

            route->distance += graph->distances[current_node][next_house];
            add_to_route(route, next_house);
            added_any = 1;

            route->total_quantity += q;
            current_load += q;
            
            // Remove from unserved BST
            unserved_root = remove_from_bst(unserved_root, next_house);
            current_node = next_house;

            // Stop if capacity reached
            if (current_load >= capacity) {
                break;
            }
            
            // Use BST to check if any remaining houses can fit
            int remaining = capacity - current_load;
            if (!can_any_house_fit(unserved_root, quantities, remaining, capacity)) {
                break;
            }
        }

        if (added_any) {
            route->distance += graph->distances[current_node][dump_node];
            add_to_route(route, dump_node);

            solution->routes[solution->num_routes] = *route;
            solution->total_distance += route->distance;
            solution->num_routes++;

            if (solution->num_routes >= max_routes) {
                max_routes *= 2;
                solution->routes = (Route*)realloc(solution->routes, max_routes * sizeof(Route));
            }
        }
        
        free(route);
    }

    free_bst(unserved_root);
    return solution;
}

void free_cvrp_solution(CVRPSolution* solution) {
    if (solution == NULL) return;
    for (int i = 0; i < solution->num_routes; i++) {
        free(solution->routes[i].path);
    }
    free(solution->routes);
    free(solution);
}