
# KNN Algorithm with Different Distance Metrics, in Client-Server Architecture

This project contains an implementation of the k-nearest neighbors (KNN) algorithm using different distance metrics. The project contains an abstract base class called `DistanceCalculator` with a virtual function called `calculateDistance`. Five different classes inherit from this base class and implement the `calculateDistance` function using different distance metrics, such as Manhattan distance and Euclidean distance.

The `Knn` class has a field of type pointer to an object of one of the classes that inherit from `DistanceCalculator`. This allows the `Knn` class to use the selected distance metric to compute distances between data points.

The User-Interface is a little program called "client" which awaits one-line inputs in the cmd, and passes them to the backend server (which calculates the result using KNN algorithm);
The server them replies to the client with the result, and the client simply outputs it, and goes back to listening for one-line inputs from the user.

All communication between the client and the server is textual and is done simple TCP.

## Usage

To run the project, compile with `make`.

You'll then need to run the server followed by the client:

`./server.out <path/to/classified-examples.csv> <port>`   
`./client.out <ip> <port>`  
Then, in the client, write as many lines as you'd like in the following format:
`...<vector-entries> <knn-metric> <knn-k-value>`
for example:
`1.2 3.4 5.6 7.8 MAN 7`

The following values for `knn-metric` are supported:
-   `MAN` for Manhattan distance
-   `AUC` for Euclidean distance
-   `CHB` for Chebyshev distance
-   `MIN` for Minkowski distance
-   `CAN` for Canberra distance

## Limitations

The input data must be in CSV format, with the class label as the last column of each row.

Right now the server only supports one client at a time.

The server and the client only run on UNIX based systems.