#ifndef ORDER_MANAGER_H
#define ORDER_MANAGER_H

#include <string>
#include <nlohmann/json.hpp>

class OrderManager {
public:
    OrderManager(const std::string& token);
    void placeOrder(const std::string& instrument, const std::string& side, double amount, double price);
    void cancelOrder(const std::string& orderId);
    void getOpenOrders(const std::string& instrument);
    void getAccountSummary(const std::string& currency);
    void sendRequest(const std::string& endpoint, const nlohmann::json& body, const std::string& requestType); // Updated to accept 3 arguments

private:
    std::string token_;
};

#endif
