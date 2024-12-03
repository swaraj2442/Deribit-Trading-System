#include "order_manager.h"
#include "utils.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <curl/curl.h>

using json = nlohmann::json;

OrderManager::OrderManager(const std::string& token) : token_(token) {}

void OrderManager::placeOrder(const std::string& instrument, const std::string& side, double amount, double price) {
    json body = {
        {"amount", amount},
        {"instrument_name", instrument},
        {"price", price},
        {"type", "market"}  // Example: type = "market"
    };
    sendRequest("private/" + side, body, "GET");  // Use GET for the correct method type
}

void OrderManager::cancelOrder(const std::string& orderId) {
    json body = {{"order_id", orderId}};
    sendRequest("private/cancel", body, "GET");
}

void OrderManager::getOpenOrders(const std::string& instrument) {
    json body = {{"instrument_name", instrument}};
    sendRequest("private/get_open_orders", body, "GET");
}

void OrderManager::getAccountSummary(const std::string& currency) {
    json body = {{"currency", currency}};
    sendRequest("private/get_account_summary", body, "GET");
}

void OrderManager::sendRequest(const std::string& endpoint, const json& body, const std::string& requestType) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    std::string url = "https://test.deribit.com/api/v2/" + endpoint;

    // If there is a body, convert it to query parameters
    if (!body.empty()) {
        std::stringstream queryParams;
        bool first = true;

        // Loop through the JSON body and append each key-value pair as query parameters
        for (auto& el : body.items()) {
            if (!first) {
                queryParams << "&";
            }
            queryParams << el.key() << "=" << el.value();
            first = false;
        }

        // Append the query parameters to the URL
        url += "?" + queryParams.str();
    }

    // Log the final URL to ensure correctness
    std::cout << "Request URL: " << url << std::endl;

    // Log the Bearer token before adding it to the header
    std::cout << "Bearer Token: " << token_ << std::endl;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Set HTTP request type (GET or POST)
        if (requestType == "GET") {
            curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        } else if (requestType == "POST") {
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
        }

        // Set Authorization header with Bearer token
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        // Construct the Authorization header
        std::string authHeader = "Authorization: Bearer " + token_;
        headers = curl_slist_append(headers, authHeader.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set callback function to handle the response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            std::cerr << "Curl request failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            // Log the response
            std::cout << "Response: " << readBuffer << std::endl;

            // Parse the response
            try {
                json response = json::parse(readBuffer);
                if (response.contains("error")) {
                    std::cerr << "Error: " << response["error"]["message"] << std::endl;
                } else {
                    std::cout << "Result: " << response["result"].dump() << std::endl;
                }
            } catch (const std::exception& e) {
                std::cerr << "Error parsing JSON: " << e.what() << std::endl;
            }
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}
