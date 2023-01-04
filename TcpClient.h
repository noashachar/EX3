#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

class TcpClient
{
    private:
        const char* ip_address;
        int sock;
        int port_no;
        struct sockaddr_in sin;

    public:
        TcpClient(const char* addr, const int p);
        bool conn();
        bool sendData(std::string data);
        std::string receive(int);
        void closeConn();
};
