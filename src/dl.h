#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

class DataLoader
{
private:
    vector<vector<short>> dataAry;
    int numLines;
    int numElements; // number of elements per line
    int numInts;     // number of integers occured in the data
    short Max;
    short Min;
    unordered_map<short, int> freqMap;

public:
    DataLoader();
    DataLoader(string filePath);
    int getNumInts();
};
