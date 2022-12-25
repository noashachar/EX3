a.out: main.o distances.o utils.o knn.o
	g++ -std=c++11 main.o distances.o utils.o knn.o -o a.out

main.o: main.cpp
	 g++ -std=c++11 -c main.cpp

distances.o: distances.cpp distances.h
	 g++ -std=c++11 -c distances.cpp

utils.o: utils.cpp utils.h
	 g++ -std=c++11 -c utils.cpp

knn.o: knn.cpp knn.h
	 g++ -std=c++11 -c knn.cpp

clean:
	 rm *.o a.out
