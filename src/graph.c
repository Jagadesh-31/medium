#include "graph.h"
#include <stdlib.h>
#include <math.h>


Graph* create_graph(int num_nodes) {
    // Allocate memory for the graph structure itself
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->num_nodes = num_nodes;
    
    // Allocate memory for points array to store coordinates
    graph->points = (Point*)malloc(num_nodes * sizeof(Point));
    
    // Allocate memory for distance matrix (2D array)
    // This matrix will store distances between every pair of nodes
    graph->distances = (double**)malloc(num_nodes * sizeof(double*));
    for (int i = 0; i < num_nodes; i++) {
        graph->distances[i] = (double*)malloc(num_nodes * sizeof(double));
    }
    
    return graph;
}


void free_graph(Graph* graph) {
    if (graph == NULL) return;
    
    // Free the points array
    free(graph->points);
    
    // Free each row of the distance matrix
    for (int i = 0; i < graph->num_nodes; i++) {
        free(graph->distances[i]);
    }
    
    // Free the distance matrix itself
    free(graph->distances);
    
    // Finally, free the graph structure
    free(graph);
}


double calculate_distance(Point a, Point b) {
    // Calculate differences in x and y coordinates
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    
    // Apply Pythagorean theorem: distance = sqrt(dx^2 + dy^2)
    return sqrt(dx * dx + dy * dy);
}