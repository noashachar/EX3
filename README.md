# KNN Algorithm with Different Distance Metrics

This project contains an implementation of the k-nearest neighbors (KNN) algorithm using different distance metrics. The project contains an abstract base class called `DistanceCalculator` with a virtual function called `calculateDistance`. Five different classes inherit from this base class and implement the `calculateDistance` function using different distance metrics, such as Manhattan distance and Euclidean distance.

The `Knn` class has a field of type pointer to an object of one of the classes that inherit from `DistanceCalculator`. This allows the `Knn` class to use the selected distance metric to compute distances between data points.

## Usage

To run the project, compile with `make` and specify three parameters:

Copy code

`./a.out k <samples.csv> <metric>` 

where `samples.csv` is the path to a classified data set to learn from, and `metric` is a string that specifies the distance metric to use. The following values for `metric` are supported:

-   `MAN` for Manhattan distance
-   `AUC` for Euclidean distance
-   `CHB` for Chebyshev distance
-   `MIN` for Minkowski distance
-   `CAN` for Canberra distance

For example, the following command will run the KNN algorithm with k=5 and use the Euclidean distance metric on the data in `samples.csv`:

Copy code

`./a.out 5 samples.csv AUC` 

## Limitations

The input data must be in CSV format, with the class label as the last column of each row.
