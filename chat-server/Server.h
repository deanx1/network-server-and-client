//
// Created by vu-student on 12-3-19.
//

#ifndef CHAT_SERVER_SERVER_H
#define CHAT_SERVER_SERVER_H

#include <thread>
#include <vector>
#include <algorithm>
#include <string.h>
#include <pthread.h>

#include "Client.h"
#include "vusocket.h"
#include "CircularLineBuffer.h"

#define PORT 5386
#define MAX_CLIENTS 5

class Client;

class Server {
private:

    std::vector<Client*> clients;
    struct sockaddr_in hints;
    SOCKET sock;

    // Creates new clients when they connect.
    void spawnClient();

    inline void closeSocket() {
        sock_close(this->sock);
    }

public:

    bool stop = false;

    inline ~Server() {
        closeSocket();
    }

    inline void setup() {
        createSocket();
    }

    inline void tick() {
        spawnClient();
    }

    // Creates a socket and binds it to the PORT.
    void createSocket();

    // Gets the username from the message, everything after the first space.
    std::string getUsername(std::string message) {
        std::size_t space = message.find(" ");
        return message.substr(space + 1, message.length());
    }

    // Checks if there is place in the room.
    bool isSpaceAvailable() {
        return this->clients.size() < MAX_CLIENTS;
    }

    // Get all users in a string seperated by a comma.
    std::string getUsers();

    // Removes a client from the logged in list by a given socket id.
    int getClientSocketId(std::string username);

    // Gets a client name from their socket
    std::string getClientFromSocket(int socketId);

    // Removes a client from the logged in list by a given socket id.
    void removeClientBySock(int sockId);

    // Checks if there is already a user with the current username online.
    bool loggedIn(std::string username);
};

#endif //CHAT_SERVER_SERVER_H
