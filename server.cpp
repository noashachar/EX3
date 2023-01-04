#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "server.h"
#include <vector>
#include "utils.h"
#include "distances.h"
#include "knn.h"

using namespace std;
//"127.0.0.1"
/*
    constructor
*/
Server::Server(const int p) {
    sock = -1;
    port_no = p;
}

/*
    Connect to a host on a certain port number
*/
bool Server::conn() {
    // create socket if it is not already created
    if (sock == -1) {
        //Create socket
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            perror("error creating socket");
            return false;
        }
        memset(&client_sin, 0, sizeof(client_sin));
        client_sin.sin_family = AF_INET;
        client_sin.sin_addr.s_addr = INADDR_ANY;
        client_sin.sin_port = htons(port_no);
        if (bind(sock, (struct sockaddr *) &client_sin, sizeof(client_sin)) < 0) {
            perror("error binding socket");
            return false;
        }
    }
    return true;
}

bool Server::listenToClient(int n = 5) {
    if (listen(sock, n) < 0) {
        perror("error listening to a socket");
        return false;
    }
    return true;
}


/*
    Send data to the connected host
*/
bool Server::sendData(string data) {
    // Send some data
    if (send(client_sock, data.c_str(), data.length(), 0) < 0) {
        perror("Send failed :( ");
        return false;
    }

    return true;
}

bool Server::acceptClient() {
    unsigned int addr_len = sizeof(client_sin);
    client_sock = accept(sock, (struct sockaddr *) &client_sin, &addr_len);
    if (client_sock < 0) {
        perror("error accepting client");
        return false;
    }
    return true;
}

/*
    Receive data from the connected host
*/
string Server::receive(int size = 4096) {
    char buffer[size];
    string reply;
    int read_bytes = recv(client_sock, buffer, sizeof(buffer), 0);
    reply = buffer;
    memset(buffer, 0, sizeof(buffer));

    if (read_bytes <= 0) {
        // either client sent empty line or cwe could not read
        return "-1";
    }

    return reply;
}


void Server::closeConn() {
    int result = close(client_sock);
    if (result < 0) {
        perror("could not close socket with client");
    }
}

bool parseUserInput(
        vector<double> *userVec,
        string *distFuncName,
        int *k,
        string &ans,
        vector<vector<double>> &X
) {
    try {
        //get place of last
        auto pk = ans.find_last_of(' ');
        //get the last str
        *k = stoi(ans.substr(pk));
        //get pace pf one before last
        auto pd = ans.substr(0, pk).find_last_of(' ');
        //get one before last str
        *distFuncName = ans.substr(1, pk - 1).substr(pd);
        *userVec = split(ans.substr(0, pd), ' ');
    }
    catch (std::exception &e) {
        return false;
    }
    if (userVec->size() != X[0].size()) {
        return false;
    }
    return true;
}

int main(int argc, char *argv[]) {
    //read file
    if (argc != 3) {
        cout << "wrong number of args" << endl;
        return 3;
    }
    string path = argv[1];
    std::pair<vector<vector<double>>, vector<string>> zug;

    zug = readFileToVectors(path);

    auto X = zug.first;
    auto y = zug.second;
    if (!illegal(X, y)) {
        perror("file invalid");
        return -1;
    }
    Knn prediction_x(X, y);
    //get port
    int server_port;
    try {
        server_port = stoi(argv[2]);
        if (server_port <= 0 || server_port >= 65536) {
            perror("port out of range");
            return 7;
        }
    }
    catch (exception &) {
        perror("port invalid");
        return -2;
    }
    Server s(server_port);
    if (!s.conn()) {
        perror("server could not connect");
        return 1;
    }
    if (!s.listenToClient()) {
        perror("could not listen for clients");
        return 2;
    }
    string ans, prediction;

    // this loop for each client
    while (true) {
        if (!s.acceptClient()) {
            perror("could not accept client");
            continue;
        }

        // this loop for each line from our client
        while (true) {
            ans = s.receive();
            if (ans == "-1") {
                break; // out of the "for each line" loop
            }

            vector<double> userVec;
            string distFuncName;
            int k;

            bool isOk = parseUserInput(&userVec, &distFuncName, &k, ans, X);

            DistanceCalculator *dc = createDistCalc(distFuncName);
            if (dc == nullptr) {
                isOk = false;
            }

            if (isOk) {
                prediction_x.prepareKnn(k, dc, userVec);
                vector<double> dis = prediction_x.getDistances();
                vector<string> k_tags = prediction_x.neighborsLabels(dis);
                prediction = prediction_x.getBetterLbels(k_tags);
                delete dc;
            } else {
                prediction = "invalid input";
            }


            if (!s.sendData(prediction)) {
                perror("could not send data to client");
                break; // closeConn with client
            }
        }

        s.closeConn();
    }

    return 0;
}