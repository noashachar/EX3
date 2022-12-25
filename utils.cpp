//
// Created by noash on 14/12/2022.
//

#include "utils.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "distances.h"


using namespace std;

DistanceCalculator* createDistCalc(string& name) {
    if (name == "AUC") {
        return new EuclideanDistanceCalculator;
    }
    if (name == "MAN") {
        return new ManhattanDistanceCalculator;
    }
    if (name == "CHB") {
        return new ChebyshevDistanceCalculator;
    }
    if (name == "CAN") {
        return new CanberraDistanceCalculator;
    }
    if (name == "MIN") {
        return new MinkowskiDistanceCalculator(3); // I chose this pow_num.
    }
    return nullptr;
}


//read classified file return two vectors one is a vector of vectors of the vales and one for the tags
pair<vector<vector<double>>, vector<string>> readFileToVectors(string& path)
{
    fstream file(path, ios::in);
    vector<vector<double>> content;
    vector<double> row;
    vector<string> tag;
    string line, word;

    if (file.is_open())
    {
        while (getline(file, line))
        {
            row.clear();
            stringstream str(line);
            while (getline(str, word, ','))
            {
                try
                {
                    row.push_back(stof(word));
                }
                catch (std::exception& e)
                {
                    tag.push_back(word);
                }
            }
            content.push_back(row);
        }
        file.close();
    }
    else {
        cout << "Could not open the file" << endl;
    }
    return make_pair(content, tag);
}

bool illegal(vector<vector<double>> f, vector<string> s) {
    if (f.size() != s.size()) {
        std::cout << "illegal file" << endl;
        return false;
    }
    if (f.size() < 1) {
        std::cout << "illegal file" << endl;
        return false;
    }
    unsigned int size = f[0].size();
    for (auto& i : f) {
        if (i.size() != size) {
            std::cout << "illegal file" << endl;
            return false;
        }
    }
    return true;
}


//split sperates the string text put it in a vector
//get text and some seperator- one space in our case
vector<double> split(const string& text, char sep) {
    try {
        vector<double> tokens;
        size_t start = 0, end;
        while ((end = text.find(sep, start)) != string::npos) {
            double var = stod(text.substr(start, end - start));
            tokens.push_back(var);
            start = end + 1;
        }
        double var = stod(text.substr(start));
        tokens.push_back(var);
        return tokens;
    }

    catch (std::exception& e) {
        vector<double> tokens;
        return tokens;
    }
}