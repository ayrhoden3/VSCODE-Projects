#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <limits>

struct NetworkLink {
    std::string destinationExchange;
    double latencyMs;
    std::string connectionName;

    NetworkLink(std::string dest, double lat, std::string name);
};

struct ExchangeNode {
    std::string id;
    std::vector<NetworkLink> outgoingConnections;

    ExchangeNode();
    ExchangeNode(std::string name);
};

struct RoutingState {
    std::string currentExchange;
    double cumulativeLatency;

    RoutingState(std::string exchange, double latency);
    bool operator>(const RoutingState& other) const;
};

class ArbitrageNetwork {
public:
    std::unordered_map<std::string, ExchangeNode> topology;

    void addExchange(const std::string& id);
    void addConnection(const std::string& from, const std::string& to, double latency, const std::string& name);
    void updateConnectionLatency(const std::string& from, const std::string& to, double newLatency);
    
    void routeArbitrageSignal(const std::string& startExchange, const std::string& targetExchange);
};

#endif