#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

class DataLoader
{
private:
    vector<vector<int>> dataAry;
    int numLines;
    int numElements; // number of elements per line
    unordered_map<int, int> freqMap;

public:
    DataLoader();
    DataLoader(string filePath);
};
