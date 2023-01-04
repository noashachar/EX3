//
// Created by noash on 10/12/2022.
//


#ifndef EX2_KNN_H
#define EX2_KNN_H

#include <string>
#include "distances.h"

class Knn {
    protected:
        int knnK;
        std::vector<std::vector<double>> knnX;
        std::vector<std::string> knnY;
        DistanceCalculator *dis;
        std::vector<double> t;
    public:
    Knn(int k, std::vector<std::vector<double>> &X,std::vector<std::string> &y, DistanceCalculator *distance, std::vector<double> input_vec);
    Knn(std::vector<std::vector<double>> &X,std::vector<std::string> &y);
    void prepareKnn(int k, DistanceCalculator *distance, std::vector<double> input_vec);
    std::vector<double> getDistances();
    std::vector<std::string> neighborsLabels(std::vector<double> distances);
    std::string getBetterLbels(std::vector<std::string> neighborsLabels);
};

#endif //EX2_KNN_H