#include <iostream>
#include <string>
#include <vector>
#include <algorithm>    // sort
#include <unordered_map>
#include "knn.h"
using namespace std;

//init func of knn, inherited from DistanceVector withe the input vector
Knn::Knn(int k, vector<vector<double>> &X,vector<string> &y, DistanceCalculator *distance, vector<double> input_vec){
    knnK = k;
    knnX = X;
    knnY = y;
    dis = distance;
    t = input_vec;
}

Knn::Knn(vector<vector<double>> &X,vector<string> &y){
    knnX = X;
    knnY = y;
}

void Knn::prepareKnn(int k, DistanceCalculator *distance, vector<double> input_vec){
    knnK = k;
    dis = distance;
    t = input_vec;
}

//return Distances between self vector to data vectors as a arr
//by the name of the distance func that required from the terminal
vector<double> Knn::getDistances(){
    vector<double> distances;
    for(vector<double>x: knnX ){
        double dist = dis->calculateDistance(x, t);
        distances.push_back(dist);
    }
     return distances;
}

//return vector of the k tags that nearest to self.vec  
vector<string> Knn::neighborsLabels(vector<double>  distances){
     vector<int> indexes;
     for (int i=0;i<knnY.size();i++){
         indexes.push_back(i);
     }
    //inner func to sort by comparing the distances
     auto compare = [&](int i, int j){
        if(distances[i]<distances[j]){
            return true;
        }
         return false;
     };
     sort(indexes.begin(), indexes.end(), compare);
     
     //push to vector the k tags that nearest to self.vec 
     vector<string> neighborsLabels;

     if (knnK > indexes.size()) {
         puts("warning: k was too big; reduced to size of input");
         knnK = indexes.size();
     }

     for(int i=0;i<knnK;i++){
         neighborsLabels.push_back(knnY[indexes[i]]);
     }
     return neighborsLabels;
}

//return the tag that appeared the most times
string Knn::getBetterLbels(vector<string> neighborsLabels){
     unordered_map<string, int> umap;
    for(string& label:neighborsLabels){
        // If key not found in map iterator
        if (umap.find(label) == umap.end()){
            umap[label]=1;
        }
        //if key found add +1
        else{
            umap[label]+=1;
        }
    }

     // find the maxValue value
     int maxValue=0;
     for (auto &kv : umap){
         if (maxValue < kv.second){
             maxValue=kv.second;
         }
     }
     
     for(auto &kk :umap){
         if (kk.second==maxValue){
             return kk.first;
         }
     }
    
     return "-1"; // will not happen
}
