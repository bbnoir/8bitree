#include "dl.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

short findMin(unordered_map<short, int> freqMap);

short findMax(unordered_map<short, int> freqMap);

DataLoader::DataLoader() : numLines(0), numElements(128), numInts(256), Max(0), Min(0)
{
    dataAry = vector<vector<short>>(numLines, vector<short>(numElements, 0));
}

DataLoader::DataLoader(string filePath) : numLines(0), numElements(128), numInts(0), Max(0), Min(0)
{
    ifstream inFile;
    inFile.open(filePath.c_str());
    if (!inFile)
    {
        cout << "Error: file not found" << endl;
        exit(1);
    }
    string line;
    vector<short> lineAry;
    short num;
    while (getline(inFile, line))
    {
        lineAry.clear();
        numLines++;
        stringstream ss(line);
        while (ss >> num)
        {
            lineAry.push_back(num);
            freqMap[num]++;
        }
        dataAry.push_back(lineAry);
    }
    inFile.close();
    for (short i = -128; i <= 127; i++)
    {
        if (freqMap[i] > 0)
        {
            numInts++;
        }
    }
    Max = findMax(freqMap);
    Min = findMin(freqMap);
    cout << "numInts: " << numInts << endl;
}

short findMin(unordered_map<short, int> freqMap)
{
    for (short i = -128; i <= 127; i++)
    {
        if (freqMap[i] > 0)
        {
            return i;
        }
    }
    return 127;
}

short findMax(unordered_map<short, int> freqMap)
{
    for (short i = 127; i >= -128; i--)
    {
        if (freqMap[i] > 0)
        {
            return i;
        }
    }
    return -128;
}

int DataLoader::getNumInts()
{
    return numInts;
}