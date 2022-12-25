//
// Created by noash on 14/12/2022.
//

#ifndef EX2_UTILS_H
#define EX2_UTILS_H

#include <string>
#include <vector>
#include "distances.h"

DistanceCalculator* createDistCalc(std::string &name) ;
bool illegal(std::vector<std::vector<double>> f, std::vector<std::string> s);
std::vector<double> split(const std::string &text, char sep);
std::pair<std::vector<std::vector<double>>, std::vector<std::string>> readFileToVectors(std::string &path);



#endif //EX2_UTILS_H
