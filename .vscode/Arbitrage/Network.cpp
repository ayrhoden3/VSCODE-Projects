#include "Network.h"
#include <algorithm>

NetworkLink::NetworkLink(std::string dest, double lat, std::string name)
    : destinationExchange(dest), latencyMs(lat), connectionName(name) {}

ExchangeNode::ExchangeNode() {}
ExchangeNode::ExchangeNode(std::string name) : id(name) {}

RoutingState::RoutingState(std::string exchange, double latency)
    : currentExchange(exchange), cumulativeLatency(latency) {}

bool RoutingState::operator>(const RoutingState& other) const {
    return cumulativeLatency > other.cumulativeLatency;
}

void ArbitrageNetwork::addExchange(const std::string& id) {
    topology[id] = ExchangeNode(id);
}

void ArbitrageNetwork::addConnection(const std::string& from, const std::string& to, double latency, const std::string& name) {
    topology[from].outgoingConnections.emplace_back(to, latency, name);
}

void ArbitrageNetwork::updateConnectionLatency(const std::string& from, const std::string& to, double newLatency) {
    for (auto& link : topology[from].outgoingConnections) {
        if (link.destinationExchange == to) {
            link.latencyMs = newLatency;
            break; 
        }
    }
}

void ArbitrageNetwork::routeArbitrageSignal(const std::string& startExchange, const std::string& targetExchange) {
    std::unordered_map<std::string, double> minLatencies;
    std::unordered_map<std::string, std::string> previousNode; 
    
    for (const auto& pair : topology) {
        minLatencies[pair.first] = std::numeric_limits<double>::infinity();
    }
    minLatencies[startExchange] = 0.0;

    std::priority_queue<RoutingState, std::vector<RoutingState>, std::greater<RoutingState>> pq;
    
    std::cout << "[INIT] Arbitrage signal detected at " << startExchange 
              << ". Target: " << targetExchange << "." << std::endl;
              
    pq.push(RoutingState(startExchange, 0.0));

    while (!pq.empty()) {
        RoutingState current = pq.top();
        pq.pop();

        std::string currId = current.currentExchange;
        
        if (current.cumulativeLatency > minLatencies[currId]) {
            continue;
        }

        std::cout << "[POP]  Evaluating routing through " << currId << "..." << std::endl;

        if (currId == targetExchange) {
            break; 
        }

        for (const NetworkLink& link : topology[currId].outgoingConnections) {
            double newLatency = current.cumulativeLatency + link.latencyMs;
            std::string neighborId = link.destinationExchange;

            if (newLatency < minLatencies[neighborId]) {
                minLatencies[neighborId] = newLatency;
                previousNode[neighborId] = currId; 
                
                std::cout << "[PUSH] Discovered route to " << neighborId 
                          << " via " << link.connectionName 
                          << " (Cumulative Cost: " << newLatency << "ms)" << std::endl;
                          
                pq.push(RoutingState(neighborId, newLatency));
            }
        }
    }

    if (minLatencies[targetExchange] == std::numeric_limits<double>::infinity()) {
        std::cout << "[FAIL] No viable network path found to " << targetExchange << "." << std::endl;
        return;
    }

    std::vector<std::string> path;
    for (std::string at = targetExchange; at != ""; at = previousNode[at]) {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());

    std::cout << "[SUCCESS] Execution path found: ";
    for (size_t i = 0; i < path.size(); ++i) {
        std::cout << path[i] << (i < path.size() - 1 ? " -> " : "");
    }
    std::cout << std::endl << "[RESULT] Total Latency: " << minLatencies[targetExchange] << "ms" << std::endl << std::endl;
}