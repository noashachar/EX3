#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "Server.h"
#include <vector>
#include "utils.h"
#include "distances.h"
#include "knn.h"

using namespace std;
//"127.0.0.1"
/*
    constructor
*/
Server::Server(const int p)
{
    sock = -1;
    port_no = p;
}

/*
    Connect to a host on a certain port number
*/
bool Server::conn()
{
    // create socket if it is not already created
    if (sock == -1)
    {
        //Create socket
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0)
        {
            perror("error creating socket");
        }
        memset(&client_sin, 0, sizeof(client_sin));
        client_sin.sin_family = AF_INET;
        client_sin.sin_addr.s_addr = INADDR_ANY;
        client_sin.sin_port = htons(port_no);
        if (bind(sock, (struct sockaddr*)&client_sin, sizeof(client_sin)) < 0)
        {
            //error
            //perror("error binding socket");
        }
    }
    return true;
}

bool Server::listenToClient(int n = 5)
{
    if (listen(sock, n) < 0)
    {
        //error
        //perror("error listening to a socket");
        return false;
    }
    return true;
}


/*
    Send data to the connected host
*/
bool Server::send_data(string data)
{

    // Send some data
    if (send(client_sock, data.c_str(), strlen(data.c_str()), 0) < 0)
    {
        perror("Send failed : ");
        return false;
    }

    return true;
}

bool Server::acceptClient()
{
    unsigned int addr_len = sizeof(client_sin);
    client_sock = accept(sock, (struct sockaddr*)&client_sin, &addr_len);
    if (client_sock < 0)
    {
        //error
        perror("error accepting client");
        return 0;
    }
    return client_sock;
}

/*
    Receive data from the connected host
*/
string Server::receive(int size = 4096)
{
    char buffer[size];
    string reply;
    int read_bytes= recv(client_sock, buffer, sizeof(buffer), 0);
    reply = buffer;
    memset(buffer, 0,sizeof(buffer));
    //Receive a reply from the server
    if ( read_bytes< 0)
    {
        return "0";
    }
    if (read_bytes == 0)
    {
        // connection is closed
        return "-1";
    }
    response_data = reply;
    return reply;
}




void Server::closeConn() {
    if (sock != -1) {
        close(sock);
    }
}

int main(int argc, char* argv[]) {
    //read file
    string path = argv[1];
    std::pair<vector<vector<double>>, vector<string>> zug;
    zug = readFileToVectors(path);
    auto X = zug.first;
    auto y = zug.second;
    if (!illegal(X, y)) {
        return -1;
    }
    knn prediction_x(X,y);
    //get port
    const int server_port = stoi(argv[2]);
    Server s(server_port);
    s.conn();
    bool flag;
    flag = s.listenToClient();
    string ans, prediction;
    while (true)
    {
        flag = s.acceptClient();
        if (!flag) {
            prediction = "invalid input";
        }

        while (true) {
            flag = true;
            ans = s.receive();
            if (ans == "-1"){
                break;
            }
            vector<double> vec1;
			string dist;
			int k;
			try {
                //get place of last
                auto pk = ans.find_last_of(" ");
                //get the last str
                k = stod(ans.substr(pk));
                //get pace pf one before last
                auto pd = ans.substr(0, pk).find_last_of(" ");
                //get one before last str
                dist = ans.substr(1, pk - 1).substr(pd);
                vec1 = split(ans.substr(0, pd), ' ');

            }
            catch (std::exception& e) {
                flag = false;
            }
            if (vec1.size() != X[0].size())
            {
                flag = false;
            }   
            DistanceCalculator* dc = createDistCalc(dist);
            if (dc == nullptr) {
                free(dc);
                flag = false;
            }
            if (flag)
            {
                prediction_x.prepareKnn(k, dc, vec1);
                vector<double> dis = prediction_x.getDistances();
                vector<string> k_tags = prediction_x.neighborsLabels(dis);
                prediction = prediction_x.getBetterLbels(k_tags);
            }
            if (!flag) {
                prediction = "invalid input";
            }
            s.send_data(prediction);
        } 
    }

    s.closeConn();
}