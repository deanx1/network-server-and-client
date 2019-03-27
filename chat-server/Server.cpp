//
// Created by dean on 12-3-19.
//

#include "Server.h"

void Server::createSocket() {
    memset (&hints, 0, sizeof (hints));
    hints.sin_family = AF_INET;
    hints.sin_addr.s_addr = htonl(INADDR_ANY);
    hints.sin_port = htons(PORT);

    // Create socket
    this->sock = socket(hints.sin_family, SOCK_STREAM, 0);
    if (this->sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    } else {
        printf("socket creation successful\n");
    }

    if ((bind(this->sock, (struct sockaddr *) &hints, sizeof(hints)) < 0)) {
        printf("socket bind failed\n");
        exit(0);
    } else {
        printf("socket bind successful\n");
    }

    if (listen(this->sock, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

void Server::spawnClient() {
    int new_socket;

    if ((new_socket = accept(this->sock, (struct sockaddr*) &hints, (socklen_t*) &hints)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    } else {
        printf("socket successfully accepted\n");
    }

    //Creates a new client with the new socket.
    Client *newClient = new Client(new_socket);
    newClient->setup(this);
    if(newClient->login()) {
        //If the user is loged in it's added to the list with logged in user.
        clients.push_back(newClient);
        //Bind a new thread to the client.
        newClient->setThread(new std::thread(&Client::loop, newClient));
    }
}

std::string Server::getUsers() {
    std::string string = "";
    for(int i = 0; i < this->clients.size(); i++) {
        string += this->clients[i]->getUsername();
        if(i < this->clients.size() -1) {
            string += ", ";
        }
    }
    return  string;
}

int Server::getClientSocketId(std::string username) {
    for(int i = 0; i < clients.size(); i++) {
        if(clients[i]->getUsername() == username) {
            return clients[i]->getSocket();
        }
    }
}

std::string Server::getClientFromSocket(int socketId) {
    for(int i = 0; i < clients.size(); i++) {
        if(clients[i]->getSocket() == socketId) {
            return clients[i]->getUsername();
        }
    }
}

void Server::removeClientBySock(int sockId) {
    for(int i = 0; i < clients.size(); i++) {
        if(clients[i]->getSocket() == sockId) {
            clients.erase(clients.begin() + i);
        }
    }
}

 bool Server::loggedIn(std::string username) {
    for(int i = 0; i < clients.size(); i++) {
        if(clients[i]->getUsername() == username) {
            return true;
        }
    }
    return false;
}