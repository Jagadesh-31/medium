#ifndef GRAPH_H
#define GRAPH_H

// Represents a 2D point (x, y coordinates)
typedef struct {
    double x;
    double y;
} Point;

// Represents a graph with distance matrix
typedef struct {
    int num_nodes;          // Total number of nodes
    double** distances;     // 2D array storing distances between nodes
    Point* points;          // Array storing coordinates of each node
} Graph;

// Function declarations
Graph* create_graph(int num_nodes);
void free_graph(Graph* graph);
double calculate_distance(Point a, Point b);

#endif