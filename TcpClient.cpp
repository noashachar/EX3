#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "TcpClient.h"
#include "utils.h"


bool is_valid_ipv4(const std::string& ip_address) {
    // Make sure the IP address is not empty
    if (ip_address.empty()) {
        return false;
    }

    // Check if the IP address consists of four octets separated by dots
    int num_dots = 0;
    for (char c : ip_address) {
        if (c == '.') {
            num_dots++;
        } else if (!isdigit(c)) {
            return false;
        }
    }
    if (num_dots != 3) {
        return false;
    }

    // Split the IP address into its octets
    int octet1, octet2, octet3, octet4;
    int num_octets = sscanf(ip_address.c_str(), "%d.%d.%d.%d", &octet1, &octet2, &octet3, &octet4);

    // Make sure we parsed exactly four octets
    if (num_octets != 4) {
        return false;
    }

    // Make sure each octet is in the correct range (0-255)
    return octet1 >= 0 && octet1 <= 255 && octet2 >= 0 && octet2 <= 255 && octet3 >= 0 && octet3 <= 255 && octet4 >= 0 && octet4 <= 255;
}



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
        cout << "error: wrong number of args" << endl;
        return 1;
    }
    int port;
    try {
        port = stoi(argv[2]);
        if (port <= 0 || port >= 65536) {
            cout << "error: port out of range" << endl;
            return 2;
        }
    }
    catch (exception &) {
        cout << "error: invalid port" << endl;
        return 35;
    }
    string ip = argv[1];
    if (!is_valid_ipv4(ip)) {
        cout << "error: <" << ip << "> is not a valid ipv4 address" << endl;
        return 93;
    }
    TcpClient c(ip.c_str(), port);
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