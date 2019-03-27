//
// Created by dean on 16-3-19.
//

#ifndef CHAT_SERVER_CLIENT_H
#define CHAT_SERVER_CLIENT_H

#include <string.h>
#include <stdio.h>

#include "Server.h"
#include "helperFunctions.h"

class Server;

class Client {
private:
    Server* server;
    std::thread* thread;

    int socketId;
    std::string username;

    bool tick();

public:
    Client(int socketId) {
        this->socketId = socketId;
    }

    void setup(Server* server) {
        this->server = server;
    }

    bool login();

    void logOut();

    void loop() {
        bool running = true;
        while(running) {
            running = tick();
        }
        this->logOut();
    }

    int getSocket(){
        return this->socketId;
    }

    std::string getUsername() {
        return this->username;
    }


    inline void setThread(std::thread* thread) {
        this->thread = thread;
    }
};


#endif //CHAT_SERVER_CLIENT_H
