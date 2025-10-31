
const express = require("express");
const { spawn } = require("child_process");
const path = require("path");

const app = express();
const PORT = 3000;

// Middleware to parse JSON requests
app.use(express.json());

// Serve static files (HTML, CSS, JS) from current directory
app.use(express.static(path.join(__dirname)));

/**
 * Route: GET /
 * Serves the main HTML interface
 */
app.get("/", (req, res) => {
    res.sendFile(path.join(__dirname, "index.html"));
});

/**
 * Route: POST /solve
 * Main endpoint that receives problem data and returns optimized routes
 * 
 * Expected Request Body:
 * {
 *   n: number of houses,
 *   points: array of {c: x-coord, r: y-coord, qty: quantity},
 *   capacity: truck capacity,
 *   useGridDist: boolean (true for Manhattan, false for Euclidean)
 * }
 */
app.post("/solve", (req, res) => {
    const { n, points, capacity, useGridDist } = req.body;
    
    // Validate input
    if (!points || !Array.isArray(points) || points.length !== n + 2) {
        return res.status(400).json({
            success: false,
            error: "Bad request",
            message: `Points array length must be ${n + 2} (1 truck + ${n} houses + 1 dump)`
        });
    }
    
    // Path to the compiled C solver executable
    // On Windows: solver.exe, on Unix/Linux/Mac: solver
    const solverPath = path.join(__dirname, "..",'src', process.platform === "win32" ? "solver.exe" : "solver");
    
    // Spawn the C solver as a child process
    const solver = spawn(solverPath);
    
    let stdout = "";  // Collect standard output (JSON result)
    let stderr = "";  // Collect error messages
    
    // Collect output data
    solver.stdout.on("data", (chunk) => {
        stdout += chunk.toString();
    });
    
    // Collect error data
    solver.stderr.on("data", (chunk) => {
        stderr += chunk.toString();
    });
    
    // Handle solver process completion
    solver.on("close", (code) => {
        // Log any errors from the solver
        if (stderr) {
            console.error("C Solver stderr:", stderr);
        }
        
        // Check exit code
        if (code !== 0) {
            console.error(`Solver exited with code ${code}`);
        }
        
        try {
            // Parse the JSON output from C solver
            const parsedResult = JSON.parse(stdout);
            
            // Check if solver reported success
            if (!parsedResult.success) {
                return res.status(500).json({
                    success: false,
                    error: "Solver error",
                    details: parsedResult.error || "Unknown error"
                });
            }
            
            // Send successful result to frontend
            res.json({
                success: true,
                routes: parsedResult.routes || [],
                totalDistance: parsedResult.totalDistance || 0,
                numRoutes: parsedResult.numRoutes || 0
            });
            
        } catch (parseError) {
            // JSON parsing failed - output format error
            console.error("C Solver output parse error:", parseError.message);
            console.error("Raw C output:", stdout);
            
            res.status(500).json({
                success: false,
                error: "Solver output format error",
                message: "Failed to parse solver output as JSON",
                details: stderr || `Exit code: ${code}`,
                rawOutput: stdout.substring(0, 500)  // First 500 chars for debugging
            });
        }
    });
    
    // Handle spawn errors (e.g., solver executable not found)
    solver.on("error", (err) => {
        console.error("Failed to spawn solver:", err);
        res.status(500).json({
            success: false,
            error: "Failed to execute solver",
            message: err.message,
            details: "Make sure the solver executable is compiled and in the correct location"
        });
    });
    
    // ========== SEND INPUT DATA TO C SOLVER ==========
    
    try {
        // Line 1: Number of houses
        solver.stdin.write(String(n) + "\n");
        
        // Line 2: Capacity and distance type
        solver.stdin.write(`${capacity} ${useGridDist ? 1 : 0}\n`);
        
        // Next n+2 lines: x y quantity for each point
        points.forEach(point => {
            const x = point.c !== undefined ? point.c : 0;
            const y = point.r !== undefined ? point.r : 0;
            const quantity = point.qty || 0;
            solver.stdin.write(`${x} ${y} ${quantity}\n`);
        });
        
        // Close stdin to signal end of input
        solver.stdin.end();
        
    } catch (writeError) {
        console.error("Error writing to solver stdin:", writeError);
        solver.kill();
        res.status(500).json({
            success: false,
            error: "Failed to send data to solver",
            message: writeError.message
        });
    }
});

/**
 * Start the server
 */
app.listen(PORT, () => {
    console.log("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    console.log("  🚀 CVRP Solver Server");
    console.log("  Team Medium: Lohith, Bharath, Jagadesh");
    console.log("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    console.log(`✅ Server running on http://localhost:${PORT}`);
    console.log("🔧 Make sure C solver is compiled:");
    console.log("   Run 'make' in parent directory");
    console.log("   Executable should be at ../src/solver");
    console.log("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    console.log("📱 Open http://localhost:3000 in your browser");
    console.log("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
});