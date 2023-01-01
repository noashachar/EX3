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
        std::string response_data = "";
        int port_no;
        struct sockaddr_in client_sin;
        int client_sock;

    public:
        std::string getDataFromUser();
        Server(const int p);
        bool conn();
        bool listenToClient(int);
        bool acceptClient();
        bool send_data(std::string data);
        std::string receive(int);
        void closeConn();
};