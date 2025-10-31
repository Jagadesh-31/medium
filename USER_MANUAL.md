# User Manual - Waste Collection Route Optimizer

## How to Run

### Step 1: Compile the Solver

```bash
cd waste-collection-optimizer
make
```

You should see a `solver` (or `solver.exe` on Windows) executable created.

### Step 2: Set Up Web Server

```bash
cd web
npm install express
```

### Step 3: Start the Server

```bash
node server.js
```

### Step 4: Open the Website

In your browser, go to:
http://localhost:3002


## Using the Website

### Step-by-Step Guide

#### 1. Set Truck Capacity

- Look at the top: **"Truck Capacity: 100"**
- Click **⚙ Settings** button
- Change **Max Capacity** value
- Click **Update Cap.**

Example: Set to 100 units

#### 2. Place the Depot (Start Point)

- Click button: **Set Depot/Start (🚛)**
- Button will highlight in green
- Click anywhere on the grid
- A green truck icon (🚛) appears

**The truck starts and returns here!**

#### 3. Add Houses

- Click button: **Add House (🏠)**
- A popup asks for waste quantity
- Enter number (e.g., 10)
- Click **Set & Place**
- Click on grid to place house
- A green house (🏠) with quantity appears

**Repeat for each house**

#### 4. Solve

- Click **Solve** button
- Wait for routes to calculate (instant!)
- Colored lines appear showing routes
- Results box shows details

#### 5. View Results

Results show:
```
Route 1:
  Path: Depot → House 1 → House 2 → Depot
  Distance: 15.23
  Quantity: 12/100
  Segment Weights: [3.61, 3.61, 8.01]
```

#### 6. Clear and Restart

- Click **Clear All** to remove everything
- Start fresh with new problem

---

## Understanding Results

### Output Explanation

```
🚛 CVRP Solution Complete
Total Distance: 28.29
Number of Routes: 2
```

**Total Distance**: Sum of all route distances

**Number of Routes**: How many trips the truck needs

### Each Route Shows

| Item | Meaning |
|------|---------|
| **Path** | Sequence of visits: Depot → Houses → Depot |
| **Distance** | Total km/miles for this route |
| **Quantity** | Waste collected / Truck capacity |
| **Segment Weights** | Distance between each consecutive pair |

### Example

```
Route 1: Depot (0,0) → House 1 (2,3) → House 2 (5,1) → Depot (0,0)
- Distance from Depot to House 1: 3.61
- Distance from House 1 to House 2: 3.61
- Distance from House 2 back to Depot: 8.01
- Total: 15.23
- Collected: 12 units out of 100 capacity
```

---

## Troubleshooting

### Problem: "gcc: command not found"

**Solution**: Install GCC compiler
- Windows: Download MinGW
- Linux: `sudo apt-get install build-essential`
- macOS: `xcode-select --install`

### Problem: Web page won't load

**Check:**
1. Is `node server.js` running? (should show "Server running...")
2. Did you compile solver? (`make` command)
3. Is solver in parent directory of web/?

### Problem: "Cannot find module 'express'"

**Solution:**
```bash
cd web
npm install express
```

### Problem: Port 3002 already in use

**Solution:**
```bash
# Linux/Mac: Kill process
lsof -ti:3002 | xargs kill

# Or: Edit web/server.js, change PORT to 3003
```

### Problem: No routes appear after clicking Solve

**Check:**
1. Did you place depot? (green truck)
2. Did you add at least 1 house? (green house)
3. Are all quantities ≤ capacity?

If house quantity > capacity, solver can't visit that house.

### Problem: Compilation errors

**Check:**
- All files in `src/` folder? (main.c, cvrp.c, graph.c, bst.c, and all .h files)
- Use: `gcc -o solver src/main.c src/cvrp.c src/graph.c src/bst.c -lm`

---

## Quick Reference

### Basic Commands

```bash
# Compile
make

# Remove compiled files
make clean

# Rebuild
make clean && make
```

### Website Quick Start

```bash
cd web
npm install express
node server.js
# Open: http://localhost:3002
```

---


### Efficient Route Planning

1. **Group nearby houses** in capacity limits
2. **Set realistic capacity** based on truck size
3. **Use settings** to adjust grid size for better visibility
4. **Check "Segment Weights"** to see individual distances

### Website Features

- **Legend** (top-left): Shows route colors and distances
- **Settings**: Adjust grid size for better placement
- **Multiple routes**: Automatically handled if capacity exceeded
- **Euclidean distance**: Straight-line 

**End of User Manual**