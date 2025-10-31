# 🚛Garbage Collection Route Optimizer

**Team Medium**: Lohith, Bharath, Jagadesh

## Overview

An intelligent garbage collection route optimizer that solves the routing problem using efficient data structures and algorithms.

Problem: Given multiple houses with waste quantities and a truck with limited capacity, find the shortest routes to collect all waste.

**Solution**: Greedy nearest-neighbor algorithm with optimization for house selection.

## Key Features

**Distance-First Optimization** - Prioritizes nearest houses  
**Interactive Web UI** - Visual route planning on grid  
**Dual Distance Metrics** - Euclidean and Manhattan  
**Capacity Management** - Automatic multi-route handling  

## Data Structures Used

| Data Structure | Purpose | Complexity |
|---|---|---|
| **Graph (Adjacency Matrix)** | Store distances between all locations | O(1) lookup |
| **Stack** | LIFO data structure for general processing | O(1) operations |
| **Route Array** | Store paths and distances | O(1) access |

## Algorithm

**Greedy Nearest Neighbor**

```
1. Start at depot
2. While unvisited houses exist:
   - Create sorted list of candidates (distance first, quantity second)
   - Select best house
   - Add to route, update capacity
   - When capacity full, return to depot
3. Output all routes
```

**Complexity**: O(n²)  
**Space**: O(n²)

## Project Structure

```
waste-collection-optimizer/
├── src/
│   ├── main.c       (I/O & JSON output)
│   ├── pathFinder.c/h     (Core routing algorithm)
│   ├── graph.c/h    (Distance matrix)
│   └── stack.c/h  
├── web/
│   ├── index.html   (Interactive UI)
│   └── server.js    (Node.js backend)
├── Makefile         (Build configuration)
├── README.md        (This file)
└── USER_MANUAL.md   (Usage guide)
```

## Quick Start

### 1. Compile
```bash
make
```

### 2. Set Up Web Interface
```bash
cd web
npm install express
node server.js
```

### 3. Open Browser
Navigate to `http://localhost:3002`

## Technology Stack

**Backend**: C (optimized performance)  
**Frontend**: HTML5 Canvas + JavaScript  
**Server**: Node.js + Express  
**Algorithm**: Greedy nearest-neighbor  

## Results

- **Distance Reduction**: 30-40% vs fixed routes
- **Performance**: < 1 second for 15 houses
