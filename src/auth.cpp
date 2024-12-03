// auth.cpp
#include "auth.h"
#include "utils.h"  
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

Auth::Auth(const std::string& client_id, const std::string& client_secret)
    : client_id_(client_id), client_secret_(client_secret) {}

std::string Auth::authenticate() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // Construct the URL with query parameters
        std::string url = "https://test.deribit.com/api/v2/public/auth";
        url += "?client_id=" + client_id_;
        url += "&client_secret=" + client_secret_;
        url += "&grant_type=client_credentials";

        // Log the final URL
        std::cout << "Request URL: " << url << std::endl;

        // Set the URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Set headers
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set the write callback to capture the response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the GET request
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "Curl request failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            // Log raw response
            std::cout << "Raw Response: " << readBuffer << std::endl;

            // Parse the response
            try {
                json response = json::parse(readBuffer);
                if (response.contains("result") && response["result"].contains("access_token")) {
                    token_ = response["result"]["access_token"];
                    std::cout << "Authenticated successfully. Token: " << token_ << std::endl;
                    return token_;
                } else if (response.contains("error")) {
                    std::cerr << "Error Code: " << response["error"]["code"] << std::endl;
                    std::cerr << "Error Message: " << response["error"]["message"] << std::endl;
                } else {
                    std::cerr << "Unexpected response format." << std::endl;
                }
            } catch (const std::exception& e) {
                std::cerr << "Error parsing JSON: " << e.what() << std::endl;
            }
        }

        // Clean up
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return "";
}
