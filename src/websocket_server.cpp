#include "websocket_server.h"
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <iostream>
#include <thread>

using tcp = boost::asio::ip::tcp;
namespace websocket = boost::beast::websocket;

// Default constructor with default address and port
WebSocketServer::WebSocketServer()
    : WebSocketServer("0.0.0.0", 8080) {}

// Parameterized constructor
WebSocketServer::WebSocketServer(const std::string &address, unsigned short port)
    : acceptor(ioContext, tcp::endpoint(boost::asio::ip::make_address(address), port)) {}

// Destructor
WebSocketServer::~WebSocketServer() {
    ioContext.stop();
    if (serverThread.joinable()) {
        serverThread.join();
    }
}

// Start the WebSocket server
void WebSocketServer::run() {
    serverThread = std::thread([this]() {
        while (true) {
            try {
                auto socket = tcp::socket(ioContext);
                acceptor.accept(socket);
                std::thread(&WebSocketServer::handleConnection, this, std::move(socket)).detach();
            } catch (std::exception &e) {
                std::cerr << "Error accepting connection: " << e.what() << std::endl;
            }
        }
    });

    connectToDeribit();
}

// Handle an incoming connection
void WebSocketServer::handleConnection(tcp::socket socket) {
    try {
        websocket::stream<tcp::socket> ws(std::move(socket));
        ws.accept();

        while (true) {
            boost::beast::flat_buffer buffer;
            ws.read(buffer);

            std::string message = boost::beast::buffers_to_string(buffer.data());
            handleMessage(ws, message);
        }
    } catch (std::exception &e) {
        std::cerr << "Connection error: " << e.what() << std::endl;
    }
}

// Process WebSocket messages
void WebSocketServer::handleMessage(
    websocket::stream<tcp::socket> &ws, const std::string &message) {
    if (startsWith(message, "subscribe:")) {
        std::string symbol = message.substr(10);
        addSubscription(symbol, &ws);
        ws.text(true);
        ws.write(boost::asio::buffer("Subscribed to " + symbol));
    } else if (startsWith(message, "unsubscribe:")) {
        std::string symbol = message.substr(12);
        removeSubscription(symbol, &ws);
        ws.text(true);
        ws.write(boost::asio::buffer("Unsubscribed from " + symbol));
    } else {
        ws.text(true);
        ws.write(boost::asio::buffer("Unknown command"));
    }
}

// Add a subscription
void WebSocketServer::addSubscription(const std::string &symbol, void *client) {
    std::lock_guard<std::mutex> lock(subscriptionsMutex);
    subscriptions[symbol].insert(client);
}

// Remove a subscription
void WebSocketServer::removeSubscription(const std::string &symbol, void *client) {
    std::lock_guard<std::mutex> lock(subscriptionsMutex);
    auto it = subscriptions.find(symbol);
    if (it != subscriptions.end()) {
        it->second.erase(client);
        if (it->second.empty()) {
            subscriptions.erase(it);
        }
    }
}

// Broadcast updates to subscribers
void WebSocketServer::broadcastUpdates(const std::string &symbol, const std::string &message) {
    std::lock_guard<std::mutex> lock(subscriptionsMutex);
    auto it = subscriptions.find(symbol);
    if (it != subscriptions.end()) {
        for (void *client : it->second) {
            auto *ws = static_cast<websocket::stream<tcp::socket> *>(client);
            ws->text(true);
            ws->write(boost::asio::buffer(message));
        }
    }
}

// Connect to the Deribit WebSocket API
void WebSocketServer::connectToDeribit() {
    try {
        tcp::resolver resolver(ioContext);
        auto const results = resolver.resolve("test.deribit.com", "443");

        auto deribitSocket = std::make_unique<tcp::socket>(ioContext);
        boost::asio::connect(*deribitSocket, results);

        deribitWebSocket = new websocket::stream<tcp::socket>(std::move(*deribitSocket));
        deribitWebSocket->handshake("test.deribit.com", "/ws/api/v2");

        std::thread(&WebSocketServer::deribitListener, this).detach();

        std::cout << "Connected to Deribit WebSocket API" << std::endl;
    } catch (std::exception &e) {
        std::cerr << "Failed to connect to Deribit: " << e.what() << std::endl;
    }
}

// Deribit WebSocket listener
void WebSocketServer::deribitListener() {
    try {
        while (true) {
            boost::beast::flat_buffer buffer;
            deribitWebSocket->read(buffer);

            std::string response = boost::beast::buffers_to_string(buffer.data());
            for (const auto &[symbol, clients] : subscriptions) {
                broadcastUpdates(symbol, response);
            }
        }
    } catch (std::exception &e) {
        std::cerr << "Error in Deribit listener: " << e.what() << std::endl;
    }
}

// Helper to mimic std::string::starts_with
bool WebSocketServer::startsWith(const std::string &str, const std::string &prefix) {
    return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
}
