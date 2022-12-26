#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <vector>
#include "utils.h"
#include "distances.h"
#include "knn.h"
using namespace std;


int main(int argc, char* argv[])
{
	const int server_port = stoi(argv[2]);
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		perror("error creating socket");
	}
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(server_port);
	if (bind(sock, (struct sockaddr*)&sin, sizeof(sin)) < 0)
	{
		perror("error binding socket");
	}
	if (listen(sock, 5) < 0)
	{
		perror("error listening to a socket");
	}
	struct sockaddr_in client_sin;
	unsigned int addr_len = sizeof(client_sin);
	int client_sock = accept(sock, (struct sockaddr*)&client_sin, &addr_len);
	if (client_sock < 0)
	{
		perror("error accepting client");
	}
	char buffer[4096];
	int expected_data_len = sizeof(buffer);
	int read_bytes = recv(client_sock, buffer, expected_data_len, 0);
	if (read_bytes == 0)
	{
		// connection is closed
	}
	else if (read_bytes < 0)
	{
		// error 
	}
	else
	{
		string s = buffer;
		//get place of last
		auto pk = s.find_last_of(" ");
		//get the last str
		int k = stod(s.substr(pk));
		//get pace pf one before last
		auto pd = s.substr(0, pk).find_last_of(" ");
		//get one before last str
		string dist = s.substr(1, pk-1).substr(pd);

		vector<double> vec1;
		vec1 = split(s.substr(0, pd-1), ' ');
		string path = argv[1];
		std::pair<vector<vector<double>>, vector<string>> zug;
		zug = readFileToVectors(path);
    	auto X = zug.first;
    	auto y = zug.second;
		DistanceCalculator* dc = createDistCalc(dist);
		knn prediction_x(k, X, y, dc, vec1);
        vector<double> dis = prediction_x.getDistances();
        vector<string> k_tags = prediction_x.neighborsLabels(dis);
        string prediction = prediction_x.getBetterLbels(k_tags);
        std::cout << prediction << endl;
	}
	int sent_bytes = send(client_sock, buffer, read_bytes, 0);
	if (sent_bytes < 0)
	{
		perror("error sending to client");
	}
	close(sock);
	return 0;
}