# Arbitrage Latency Routing Engine 

This project was developed for my Data Structures and Analytics class to demonstrate an applied use case of **Dijkstra’s Shortest Path Algorithm**. 

I was given free range to display the Dijkstra Algorithm however I wanted. So instead of a standard grid maze, I made this program that models a high-frequency trading network where data centers (nodes) are connected by various data links like microwave towers and fiber optic cables (edges). The algorithm calculates the absolute fastest path to route an arbitrage signal between financial exchanges. 

To demonstrate dynamic graph traversal, the simulation injects a "network shock" (simulating weather interference on a microwave tower) mid-execution, forcing the priority queue to re-evaluate edge weights and reroute the signal through a secondary fiber connection.

##  Data Structures & Architecture

*   **Directed Weighted Graph:** The network topology is represented as an adjacency list.
*   **Hash Maps (`std::unordered_map`):** Utilized for $O(1)$ average-time complexity lookups of execution venues via string identifiers (e.g., `"CME"`, `"LME"`).
*   **Min-Heap Priority Queue (`std::priority_queue`):** A custom `RoutingState` struct overloads the `>` operator to maintain the min-heap property. This ensures the routing frontier strictly evaluates the lowest cumulative latency paths first.
*   **Lazy Deletion:** Rather than implementing an expensive $O(N)$ decrease-key operation when a faster path to an explored node is found, the algorithm pushes the new state and gracefully ignores stale, higher-latency states when they are popped.

##  The Simulation

The program runs in two distinct phases to showcase algorithmic responsiveness:

1.  **Phase 1 (Baseline Routing):** The algorithm successfully finds the fastest route using a low-latency microwave connection.
2.  **Phase 2 (Dynamic Rerouting):** A simulated weather event spikes the latency on the primary microwave link. The edge weight is updated in the adjacency list, and the algorithm is re-run. The min-heap naturally bypasses the newly congested route, dynamically failing over to a slightly longer but now-optimal fiber optic path.

##  Compilation & Execution

This project was written in standard C++ and requires no external libraries. 

Compile the project using standard g++:
```bash
g++ main.cpp Network.cpp -o router
