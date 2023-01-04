#ifndef EX2_DISTANCES_H
#define EX2_DISTANCES_H

#include <string>

void powVector(std::vector<double> &p, double pow_num);

double sumVector(std::vector<double> &vec2);

std::vector<double> makeDiffrencesVector(std::vector<double> &p, std::vector<double> &q);

// --------

class DistanceCalculator {
public:
    virtual double calculateDistance(std::vector<double> &v1, std::vector<double> &v2) = 0;

    virtual ~DistanceCalculator() = default;
};

class MinkowskiDistanceCalculator : public DistanceCalculator {
public:
    int pow_num;
    double calculateDistance(std::vector<double> &v1, std::vector<double> &v2) override;
    explicit MinkowskiDistanceCalculator(int pow_num) { this->pow_num = pow_num; }
};

class CanberraDistanceCalculator : public DistanceCalculator {
public:
    double calculateDistance(std::vector<double> &v1, std::vector<double> &v2) override;
};

class ChebyshevDistanceCalculator : public DistanceCalculator {
public:
    double calculateDistance(std::vector<double> &v1, std::vector<double> &v2) override;
};

class EuclideanDistanceCalculator : public DistanceCalculator {
public:
    double calculateDistance(std::vector<double> &v1, std::vector<double> &v2) override;
};


class ManhattanDistanceCalculator : public DistanceCalculator {
public:
    double calculateDistance(std::vector<double> &v1, std::vector<double> &v2) override;
};

#endif // EX2_DISTANCES_H
