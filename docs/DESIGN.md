# Deribit API Client

This repository provides a set of classes for interacting with the Deribit API, supporting authentication, order management, and WebSocket communication.

## Overview

- **Authentication:** Allows obtaining an OAuth2 access token.
- **Order Management:** Places, cancels, and queries orders on the Deribit platform.
- **WebSocket Server:** A real-time server that listens for updates from the Deribit WebSocket API and handles client subscriptions.

## Components

### `auth.h / auth.cpp`

- **Class:** `Auth`
  - **Purpose:** Authenticates using OAuth2 credentials and retrieves an access token.
  - **Key Methods:**
    - `authenticate()`: Obtains an access token from Deribit.
    - `getToken()`: Returns the current access token.

### `order_manager.h / order_manager.cpp`

- **Class:** `OrderManager`
  - **Purpose:** Manages orders and retrieves account details.
  - **Key Methods:**
    - `placeOrder()`: Places a new order.
    - `cancelOrder()`: Cancels an existing order.
    - `getOpenOrders()`: Retrieves open orders.
    - `getAccountSummary()`: Gets account summary.

### `utils.h / utils.cpp`

- **Purpose:** Contains helper functions, such as handling API response data using `libcurl`.

### `websocket_server.h / websocket_server.cpp`

- **Class:** `WebSocketServer`
  - **Purpose:** Implements a WebSocket server to listen to real-time updates from Deribit.
  - **Key Methods:**
    - `run()`: Starts the WebSocket server.
    - `connectToDeribit()`: Establishes a connection to Deribit’s WebSocket API.
    - `handleMessage()`: Handles client messages, including subscription/unsubscription.

## Dependencies

- **libcurl:** For HTTP requests to the REST API.
- **nlohmann/json:** For JSON parsing.
- **Boost.Asio & Boost.Beast:** For WebSocket communication.

## Error Handling

- **Authentication errors**, **API request errors**, and **WebSocket errors** are logged, ensuring robust error tracking.