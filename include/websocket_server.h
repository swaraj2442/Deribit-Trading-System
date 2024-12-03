#ifndef WEBSOCKET_SERVER_H
#define WEBSOCKET_SERVER_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <thread>

class WebSocketServer {
public:
    // Default constructor
    WebSocketServer();

    // Constructor with custom address and port
    WebSocketServer(const std::string &address, unsigned short port);

    // Destructor
    ~WebSocketServer();

    // Start the WebSocket server
    void run();

    // Connect to Deribit WebSocket API
    void connectToDeribit();

private:
    // Handle an incoming connection
    void handleConnection(boost::asio::ip::tcp::socket socket);

    // Process WebSocket messages
    void handleMessage(
        boost::beast::websocket::stream<boost::asio::ip::tcp::socket> &ws,
        const std::string &message);

    // Add and remove subscriptions
    void addSubscription(const std::string &symbol, void *client);
    void removeSubscription(const std::string &symbol, void *client);

    // Broadcast updates to subscribers
    void broadcastUpdates(const std::string &symbol, const std::string &message);

    // Helper function to mimic std::string::starts_with
    bool startsWith(const std::string &str, const std::string &prefix);

    // ASIO context and thread management
    boost::asio::io_context ioContext;
    boost::asio::ip::tcp::acceptor acceptor;
    std::thread serverThread;

    // Deribit WebSocket connection management
    void deribitListener();

    boost::beast::websocket::stream<boost::asio::ip::tcp::socket> *deribitWebSocket = nullptr;

    // Subscriptions map: symbol -> set of clients
    std::unordered_map<std::string, std::unordered_set<void *>> subscriptions;
    std::mutex subscriptionsMutex;
};

#endif // WEBSOCKET_SERVER_H
