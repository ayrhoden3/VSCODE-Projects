#include "Network.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    ArbitrageNetwork router;

    router.addExchange("CME");
    router.addExchange("NY_Fiber");
    router.addExchange("NY_Microwave");
    router.addExchange("LME");

    router.addConnection("CME", "NY_Microwave", 4.1, "Microwave_Tower_A");
    router.addConnection("CME", "NY_Fiber", 4.8, "Fiber_Trunk_1");
    router.addConnection("NY_Microwave", "LME", 25.0, "Transatlantic_Radio");
    router.addConnection("NY_Fiber", "LME", 29.4, "Transatlantic_Cable");

    std::cout << "--- PHASE 1: Baseline Routing ---" << std::endl;
    router.routeArbitrageSignal("CME", "LME");

    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "----------------------------------------------------" << std::endl;
    std::cout << "[EVENT] Heavy rain system moving across Pennsylvania." << std::endl;
    std::cout << "[EVENT] Microwave_Tower_A latency spiking to 12.5ms!" << std::endl;
    std::cout << "----------------------------------------------------" << std::endl << std::endl;

    router.updateConnectionLatency("CME", "NY_Microwave", 12.5);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "--- PHASE 2: Dynamic Rerouting ---" << std::endl;
    router.routeArbitrageSignal("CME", "LME");

    return 0;
}