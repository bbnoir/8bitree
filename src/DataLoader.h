#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>

typedef int8_t int8;

using namespace std;

class DataLoader
{
private:
    vector<vector<int8>> dataAry;
    string filePath;
    unsigned int numBytes;
    int numLines;
    int intPerLine; // number of elements per line
    int numInts;    // number of integers occured in the data
    int8 Max;
    int8 Min;
    vector<int> freqMap;

public:
    DataLoader();
    DataLoader(string filePath); // read data from file
    int getNumInts();
    int getNumLines();
    int getNumElements();
    vector<int> getFreqMap();
    friend ostream &operator<<(ostream &os, const DataLoader &dl);
};