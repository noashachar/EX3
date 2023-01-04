#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

class Server
{
private:
    int sock;
    int port_no;
    struct sockaddr_in client_sin;
    int client_sock;

public:
    Server(const int p);
    bool conn();
    bool listenToClient(int);
    bool acceptClient();
    bool sendData(std::string data);
    std::string receive(int);
    void closeConn();
};