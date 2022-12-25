//
// Created by noash on 14/12/2022.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "distances.h"
#include "knn.h"
#include "utils.h"

using namespace std;

int main(int argc, char* argv[]) {
    int k = stoi(argv[1]);
    string path = argv[2];
    string dist = argv[3];
    pair<vector<vector<double>>, vector<string>> zug;

    DistanceCalculator* dc = createDistCalc(dist);
    if (dc == nullptr) {
        std::cout << "unknown distance function" << endl;
        return 1;
    }

    zug = readFileToVectors(path);
    auto X = zug.first;
    auto y = zug.second;
    vector<double> vec1;
    string text;

    if (!illegal(X, y)) {
        return -1;
    }

    //dint free dc
    while (true) {
        do {
            getline(cin, text);
            vec1 = split(text, ' ');
        } while (vec1.size() != X[0].size());
        knn prediction_x(k, X, y, dc, vec1);
        vector<double> dis = prediction_x.getDistances();
        vector<string> k_tags = prediction_x.neighborsLabels(dis);
        string prediction = prediction_x.getBetterLbels(k_tags);
        std::cout << prediction << endl;
    }
}        