#ifndef DL_H
#define DL_H
#include "tree.h"
#include <iostream>
#include <vector>
#include <unordered_map>

typedef char int8;

using namespace std;

class DataLoader
{
private:
    vector<vector<int8>> dataAry;
    int numLines;
    int numElements; // number of elements per line
    int numInts;     // number of integers occured in the data
    int8 Max;
    int8 Min;
    vector<int> freqMap;

public:
    DataLoader();
    DataLoader(string filePath);
    int getNumInts();
    int getNumLines();
    int getNumElements();
};

#endif