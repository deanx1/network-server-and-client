
#include "Client.h"

#define IP "localhost" //52.58.97.202
#define PORT "5386"

int Client::tick() {

    if(stdinBuffer.hasLine()) {
        std::string input = stdinBuffer.readLine();

        if (compareInput(input.c_str(), "!quit")) {
            return -1;
        }

        if (compareInput(input.c_str(), "!who")) {
            char sendCommand[] = "WHO\n";
            if (send(sock, sendCommand, strlen(sendCommand), 0) < 0) {
                perror("send() failed");
            }
        }

        if (compareInput(input.c_str(), "@")) {

            char sendCommand[1024];
            std::size_t space = input.find(" ");
            std::string username = input.substr(1, space);
            std::string message = input.substr(space + 1, input.length());

            sprintf(sendCommand, "SEND %s %s\n", username.c_str(), message.c_str());

            if (send(sock, sendCommand, strlen(sendCommand), 0) < 0) {
                perror("send() failed");
            }
        }
    }

    if (socketBuffer.hasLine()) {
        printf("%s\n", socketBuffer.readLine().c_str());
    }

    return 0;
}

int Client::compareInput(const char input[], const char expectedResult[]) {
    int resultLength = strlen(expectedResult);
    return strlen(input) >= resultLength && strncmp(expectedResult, input, resultLength) == 0;
}

int Client::readFromStdin() {
    std::string input;
    getline(std::cin, input);

    input = input + '\n';
    stdinBuffer.writeChars(input.c_str(), input.size());

    if (compareInput(input.c_str(), "!quit"))
        return -1;

    return input.size();
}

int Client::readFromSocket() {
    char* buffer = new char[socketBuffer.freeSpace()];
    size_t numbytes = recv(sock, buffer, socketBuffer.freeSpace(), 0);
    if(!socketBuffer.writeChars(buffer, numbytes)) {
        printf("Response is too big\n");
    }
    return numbytes;
}

void Client::createSocketAndLogIn() {
    struct addrinfo hints;
    struct addrinfo *res;

    int errcode;

    memset (&hints, 0, sizeof (hints));
    hints.ai_family = AF_INET; //IPv4
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(IP, PORT, &hints, &res)) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(errcode));
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    std::string username;

    // The server closes the socket when a login was unsuccessful, so a new socket needs to be created.
    do {
        this->sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sock == -1) {
            perror("socket");
            exit(EXIT_FAILURE);
        }

        if (connect(sock, res->ai_addr, res->ai_addrlen) < 0) {
            perror("connect() failed");
            exit(EXIT_FAILURE);
        }
        printf ("Connected to Host: %s:%s\n", IP, PORT);

        printf("Enter a user name: ");
        getline(std::cin, username);

        // Puts the username in the command buffer.
        sprintf(buffer, "HELLO-FROM %s\n", username.c_str());

        if (send(sock, buffer, strlen(buffer), 0) < 0) {
            perror("send() failed");
        }

        if (recv(sock, buffer, 1024, 0) > 0) {
            if (compareInput(buffer, "HELLO")) {
                printf("You\'r now logged in as %s\n", username.c_str());

                // The user logged in succesfully and the program can stop asking for a username.
                this->loggedIn = true;
            } else if (compareInput(buffer, "IN-USE")) {
                printf("Username %s is already in use, please use another username.\n", username.c_str());
            }
        }
    } while(!loggedIn);
}

void Client::closeSocket() {
    sock_close(sock);
}