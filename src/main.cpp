#include <iostream>
#include "auth.h"
#include "order_manager.h"
#include "websocket_server.h"
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

int main() {
    // Read credentials from config.json
    std::ifstream configFile("/Users/swaraj/Documents/c++/project/DeribitTradingSystem/src/config.json");
    if (!configFile.is_open()) {
        std::cerr << "Failed to open config.json" << std::endl;
        return 1;
    }

    json config;
    configFile >> config;
    
    std::string client_id = config["client_id"];
    std::string client_secret = config["client_secret"];

    // Create Auth object with client_id and client_secret from config.json
    Auth auth(client_id, client_secret);
    std::string token = auth.authenticate();

    if (token.empty()) {
        std::cerr << "Authentication failed. Exiting..." << std::endl;
        return 1;
    }

    // Create OrderManager object with the token
    OrderManager orderManager(token);

    // Place an order
    std::cout << "Placing order..." << std::endl;
    orderManager.placeOrder("BTC-PERPETUAL", "buy", 10.0, 30000.0);

    // Cancel an order (you can replace "order_id_to_cancel" with an actual order ID)
    std::cout << "Canceling order..." << std::endl;
    orderManager.cancelOrder("order_id_to_cancel");

    // Get open orders for a specific instrument
    std::cout << "Fetching open orders..." << std::endl;
    orderManager.getOpenOrders("BTC-PERPETUAL");

    // Get account summary for a specific currency
    std::cout << "Fetching account summary..." << std::endl;
    orderManager.getAccountSummary("BTC");

    // Instantiate the WebSocket server
    WebSocketServer websocketServer; // Default constructor

    // Run the WebSocket server
    websocketServer.run();

    std::cout << "WebSocket Server running on ws://0.0.0.0:8084" << std::endl;

    // Keep the server running indefinitely
    while (true) {
        std::this_thread::sleep_for(std::chrono::hours(1));
    }

    return 0;
}
