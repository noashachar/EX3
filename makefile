all: server client

client: TcpClient.cpp TcpClient.h
	 g++ -std=c++11 TcpClient.cpp -o client.out

client.o: TcpClient.cpp
	 g++ -std=c++11 -c TcpClient.cpp

server: server.o distances.o utils.o knn.o
	 g++ -std=c++11 server.o distances.o utils.o knn.o -o server.out

server.o: server.cpp server.h
	 g++ -std=c++11 -c server.cpp

distances.o: distances.cpp distances.h
	 g++ -std=c++11 -c distances.cpp

utils.o: utils.cpp utils.h
	 g++ -std=c++11 -c utils.cpp

knn.o: knn.cpp knn.h
	 g++ -std=c++11 -c knn.cpp

clean:
	 rm -f *.o
