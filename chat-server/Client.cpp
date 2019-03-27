//
// Created by dean on 16-3-19.
//

#include "Client.h"


bool Client::login() {
    char* buffer = new char[1024];
    read(this->socketId, buffer, 1024);
    std::string message(buffer);
    char result[1024];

    if(help::compareInput(message.c_str(), "HELLO-FROM")) {
        // Strips the \n from the username.
        std::string stripedMessage;
        std::remove_copy(message.begin(), message.end(), std::back_inserter(stripedMessage), '\n');
        std::string username = server->getUsername(stripedMessage);

        if(!server->loggedIn(username)) {
            // Checks if there is place on the server for the client.
            if(server->isSpaceAvailable()) {
                this->username = username;
                struct sockaddr lSocketAddress;
                socklen_t lSocketAddresslen = sizeof(lSocketAddress);
                accept(this->socketId, &lSocketAddress, &lSocketAddresslen);

                sprintf(result, "HELLO %s\n", username.c_str());
                send(socketId, result, strlen(result), 0);
                return true;
            } else {
                // chat room is full
                send(socketId, "BUSY\n", strlen(result), 0);
                return false;
            }
        } else {
            // Username is already taken
            sprintf(result, "IN-USE\n");
            // close socket
            send(socketId, result, strlen(result), 0);
            close(socketId);
            return false;
        }
    }
    // The request contains a bad header or a command which is, not yet, supported.
    sprintf(result, "BAD-RQST-HDR\n");
    send(socketId, result, strlen(result), 0);
    close(socketId);

    return false;
}

void Client::logOut() {
    this->server->removeClientBySock(this->socketId);
    close(socketId);
}

bool Client::tick() {
    char* buffer = new char[1024];
    if(read(socketId, buffer, 1024) > 0) {
        std::string message(buffer);
        char result[1024];

        if (help::compareInput(message.c_str(), "WHO\n")) {
            // Strips the \n from the username.
            std::string stripedMessage;
            std::remove_copy(message.begin(), message.end(), std::back_inserter(stripedMessage), '\n');
            std::string username = server->getUsername(stripedMessage);

            sprintf(result, "WHO-OK %s\n", this->server->getUsers().c_str());
            send(socketId, result, strlen(result), 0);
            return true;
        } else if (help::compareInput(message.c_str(), "SEND")) {
            printf("SEND message received\n");

            std::string stripedMessage;
            std::remove_copy(message.begin(), message.end(), std::back_inserter(stripedMessage), '\n');
            message = stripedMessage;

            // Get receiver username
            std::size_t space1 = message.find(" ");
            message = message.substr(space1 + 1);
            std::size_t space2 = message.find(" ");
            std::string recUsername = message.substr(0, space2);

            int senderSocket = socketId;

            // Reveiver might not be online
            if (!server->loggedIn(recUsername)) {
                sprintf(result, "UNKNOWN\n");
                send(senderSocket, result, strlen(result), 0);

            } else {
                // Get sender username
                std::string sendUsername = server->getClientFromSocket(socketId);

                // Get message
                message = message.substr(space2 + 2, size(message) - 3);

                int receiverSocket = server->getClientSocketId(recUsername);

                // Forward the message to the receiver
                sprintf(result, "DELIVERY %s %s\n", sendUsername.c_str(), message.c_str());
                send(receiverSocket, result, strlen(result), 0);

                // Else send OK message to sender
                sprintf(result, "SEND-OK\n");
                send(senderSocket, result, strlen(result), 0);
                return true;
            }

        }
        // The request contains a bad header or a command which is, not yet, supported.
        sprintf(result, "BAD-RQST-HDR\n");
        send(socketId, result, strlen(result), 0);
        close(socketId);
    }

    return false;
}