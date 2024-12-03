Performance Highlights

1. Efficient HTTP Requests (Using libcurl)
The code utilizes libcurl, a highly efficient and well-optimized library for handling HTTP requests. By using this library, the code ensures fast and reliable communication with the Deribit REST API. The performance of making asynchronous HTTP requests is enhanced through careful management of connection pooling and request handling.

2. Real-time Data with WebSocket Server
The WebSocketServer class leverages Boost.Asio and Boost.Beast for managing WebSocket communication. This setup allows for low-latency, real-time updates from the Deribit WebSocket API. Subscriptions and message broadcasting are handled efficiently using unordered maps and unordered sets to ensure fast lookups and quick message dispatch to connected clients.

3. Thread Management
The use of multi-threading for handling incoming WebSocket connections ensures non-blocking operations and improves overall system responsiveness. Each client connection is handled in its own thread, allowing the server to scale well under heavy traffic while avoiding bottlenecks.

4. Optimized Subscription Management
Subscriptions are stored in a mutex-protected unordered map, which ensures thread-safe access. The map allows for constant-time lookups and updates, making subscription management both efficient and safe in a multi-threaded environment.

5. Real-time Error Logging
The system logs key operations and errors in real-time, providing immediate feedback in the console. This enables quicker debugging and identification of issues during both API interactions and WebSocket communication.