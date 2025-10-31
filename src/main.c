#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graph.h"
#include "pathFinder.h"

double calc_dist(double x1, double y1, double x2, double y2, int grid) {
    if (grid) return fabs(x1-x2)+fabs(y1-y2);
    double dx = x1-x2, dy = y1-y2;
    return sqrt(dx*dx+dy*dy);
}

int main() {
    int n, cap, grid;
    scanf("%d", &n);
    scanf("%d %d", &cap, &grid);
    int total = n+2, dump = total-1;
    double* x = (double*)malloc(total*sizeof(double));
    double* y = (double*)malloc(total*sizeof(double));
    int* qty = (int*)malloc(total*sizeof(int));
    
    for (int i=0; i<total; i++)
        scanf("%lf %lf %d", &x[i], &y[i], &qty[i]);
    
    Graph* g = create_graph(total);
    for (int i=0; i<total; i++) {
        g->points[i].x = x[i]; 
        g->points[i].y = y[i];
        for (int j=0; j<total; j++)
            g->distances[i][j] = calc_dist(x[i],y[i],x[j],y[j],grid);
    }
    
    // extended pathFinder with capacity support
    pathFinderSolution* sol = solve_pathFinder(g, dump, qty, cap);
    
    // Format output for the web interface
    printf("{\"success\":true,\"totalDistance\":%.2f,\"numRoutes\":1,\"routes\":[", sol->total_length);
    printf("{\"routeId\":1,\"distance\":%.2f,\"totalQuantity\":%d,\"route\":[", sol->total_length, 0);
    
    for (int j=0; j<sol->path_length; j++) {
        printf("%d", sol->path[j]);
        if(j<sol->path_length-1) printf(",");
    }
    
    printf("],\"summary\":{\"weights\":[");
    for(int j=0;j<sol->path_length-1;j++) {
        printf("%.2f",g->distances[sol->path[j]][sol->path[j+1]]);
        if(j<sol->path_length-2) printf(",");
    }
    printf("]}}");
    printf("]}");
    
    free_pathFinder_solution(sol);
    free_graph(g);
    free(x); free(y); free(qty);
    return 0;
}