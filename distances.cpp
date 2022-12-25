#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include "distances.h"

using namespace std;



//move on vector values and updte the value to be in pow that gived as parameter
void powVector(vector<double> &p, double pow_num){
    for(int i=0 ; i<p.size(); ++i){
        p[i] = (pow(p[i],pow_num));
    }
}

//sum the value of the vactor return double num 
double sumVector(vector<double> &vec2) {
    double sum = 0;
    for (unsigned i = 0; i < vec2.size(); ++i) {
        sum += vec2[i];
    }
    return sum;
}

//conculate the abs Diffrences between two vector
vector<double> makeDiffrencesVector(vector<double> &p, vector<double> &q) {
    std::vector<double> vect;
    if (p.size() != q.size()) {
        return vect;
    }
    for (int i = 0; i < p.size(); ++i) {
        vect.push_back(abs(p[i] - q[i]));
    }
    return vect;
}

//return double Minkowski Distance between two vectors
double MinkowskiDistanceCalculator::calculateDistance(vector<double> &v1, vector<double> &v2){
    vector<double>diffrences_vec = makeDiffrencesVector(v1, v2);
    powVector(diffrences_vec,pow_num);
    double sum = sumVector(diffrences_vec);
    return pow(sum,(1/pow_num));
}

//return double Canberra Distance between two vectors
double CanberraDistanceCalculator::calculateDistance(vector<double> &p, vector<double> &q){
    double sum=0;
    vector<double>diffrences_vec = makeDiffrencesVector(p,q);
    for (int i=0 ; i<diffrences_vec.size(); ++i){
        double div = abs(p[i])+abs(q[i]);
        //if div < 0 ignore this iteration
        if (div > 0){
            sum += diffrences_vec[i]/div;
        }
    }
    return sum;
}

//return double Chebyshev Distance between two vectors
double ChebyshevDistanceCalculator::calculateDistance(vector<double> &p, vector<double> &q){
    double max=0;
    vector<double>diffrences_vec = makeDiffrencesVector(p,q);
    for (double i : diffrences_vec){
        if (i > max){
        max = i;
        }
    }
    return max;
}

//return double euclidan Distance between two vectors
double EuclideanDistanceCalculator::calculateDistance(vector<double> &viki1, vector<double> &viki2) {
    vector<double> diffrences = makeDiffrencesVector(viki1, viki2);
    powVector(diffrences, 2);
    return sqrt(sumVector(diffrences));
}

//return double manhatan Distance between two vectors
double ManhattanDistanceCalculator::calculateDistance(vector<double> &viki1, vector<double> &viki2) {
    vector<double> diffrences = makeDiffrencesVector(viki1, viki2);
    return sumVector(diffrences);

}
