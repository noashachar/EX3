#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "TcpClient.h"

using namespace std;
//"127.0.0.1"
/*
    constructor
*/
TcpClient::TcpClient(const char *addr, const int p) {
    sock = -1;
    port_no = p;
    ip_address = addr;
}

/*
    Connect to a host on a certain port number
*/
bool TcpClient::conn() {
    // create socket if it is not already created
    if (sock == -1) {
        //Create socket
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            perror("error creating socket");
            return false;
        }

        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = inet_addr(ip_address);
        sin.sin_port = htons(port_no);
        if (connect(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
            perror("error connecting to server");
            return false;
        }
    }

    return true;
}

string readLineFromUser() {
    string text;
    getline(cin, text);
    return text;
}

/*
    Send data to the connected host
*/
bool TcpClient::sendData(string data) {
    // Send some data
    if (send(sock, data.c_str(), strlen(data.c_str()), 0) < 0) {
        perror("Send failed");
        return false;
    }
    return true;
}

/*
    Receive data from the connected host
*/
string TcpClient::receive(int size = 4096) {
    char buffer[size];
    string reply;

    //Receive a reply from the server
    if (recv(sock, buffer, sizeof(buffer), 0) < 0) {
        puts("recv failed");
        throw exception();
    }

    reply = buffer;
    return reply;
}

void TcpClient::closeConn() {
    if (sock != -1) {
        close(sock);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        perror("wrong number of args");
        return 1;
    }
    int port;
    try {
        port = stoi(argv[2]);
        if (port <= 0 || port >= 65536) {
            perror("port out of range");
            return 2;
        }
    }
    catch (exception &) {
        perror("invalid port");
        return 35;
    }
    TcpClient c(argv[1], port);
    if (!c.conn()) {
        perror("could not connect to server");
        return 44;
    }
    std::string userInput = readLineFromUser();
    while (userInput != "-1") {
        if (!c.sendData(userInput)) {
            perror("could no send data to server");
            return 7;
        }
        try {
            cout << c.receive() << endl;
        }
        catch (exception &) {
            perror("could not read data from server");
        }
        userInput = readLineFromUser();
    }
    c.closeConn();
}