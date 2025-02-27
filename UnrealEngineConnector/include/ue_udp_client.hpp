#pragma once
#include <iostream>
#include <string>
#include <stdexcept>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <nlohmann/json.hpp>

#pragma comment(lib, "ws2_32.lib") // Link against Winsock library

using json = nlohmann::json;


class UdpSocket {
private:
    SOCKET sockfd;
    sockaddr_in server_addr;

public:
    // Constructor to initialize the UDP socket
    UdpSocket(const std::string& ipAddress, int port) {
        // Initialize Winsock
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            throw std::runtime_error("WSAStartup failed");
        }

        // Create a UDP socket
        sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sockfd == INVALID_SOCKET) {
            WSACleanup();
            throw std::runtime_error("Failed to create socket");
        }

        // Configure server address
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);

        // Convert IP address from string to binary form
        if (inet_pton(AF_INET, ipAddress.c_str(), &server_addr.sin_addr) <= 0) {
            closesocket(sockfd);
            WSACleanup();
            throw std::runtime_error("Invalid IP address");
        }
    }

    // Destructor to close the socket and clean up Winsock
    ~UdpSocket() {
        closesocket(sockfd);
        WSACleanup();
    }

    // Method to send JSON data
    void sendJson(json jsonData) {
        // Convert JSON to string
        std::string jsonString = jsonData.dump();
        retry:
        long retryCount = 0;
        // Send the JSON string over the UDP socket
        int bytesSent = sendto(sockfd, jsonString.c_str(), static_cast<int>(jsonString.size()), 0,
            (struct sockaddr*)&server_addr, sizeof(server_addr));

        if (bytesSent == SOCKET_ERROR) {
            retryCount += 1;
            goto retry;
        }

        //std::cout << "Sent " << bytesSent << " bytes of JSON data." << std::endl;
    }
};