#include "dl.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int8 findMin(vector<int> freqMap);

int8 findMax(vector<int> freqMap);

DataLoader::DataLoader() : numLines(0), numElements(128), numInts(256), Max(0), Min(0)
{
    dataAry = vector<vector<int8>>(numLines, vector<int8>(numElements, 0));
    freqMap = vector<int>(256, 0);
}

DataLoader::DataLoader(string filePath) : numLines(0), numElements(128), numInts(0), Max(0), Min(0)
{
    freqMap = vector<int>(256, 0);
    ifstream inFile;
    inFile.open(filePath.c_str());
    if (!inFile)
    {
        cout << "Error: file not found" << endl;
        exit(1);
    }
    string line;
    vector<int8> lineAry;
    int num;
    while (getline(inFile, line))
    {
        lineAry.clear();
        numLines++;
        stringstream ss(line);
        while (ss >> num)
        {
            int8 upper4bits = (num >> 4) & 0x0F;
            int8 lower4bits = num & 0x0F;
            lineAry.push_back(upper4bits);
            lineAry.push_back(lower4bits);
            freqMap[upper4bits]++;
            freqMap[lower4bits]++;
        }
        dataAry.push_back(lineAry);
    }
    inFile.close();
    for (int i = 0; i < 256; i++)
    {
        if (freqMap[i] > 0)
        {
            numInts++;
        }
    }
    Max = findMax(freqMap);
    Min = findMin(freqMap);
    // cout << "numInts: " << numInts << endl;
}

int8 findMin(vector<int> freqMap)
{
    for (int i = 0; i < 256; i++)
    {
        if (freqMap[i] > 0)
        {
            return i - 128;
        }
    }
    return 127;
}

int8 findMax(vector<int> freqMap)
{
    for (int i = 255; i >= 0; i--)
    {
        if (freqMap[i] > 0)
        {
            return i - 128;
        }
    }
    return -128;
}

int DataLoader::getNumInts()
{
    return numInts;
}