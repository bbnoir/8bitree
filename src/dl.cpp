#include "dl.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

DataLoader::DataLoader()
{
    numLines = 0;
    numElements = 128;
}

DataLoader::DataLoader(string filePath)
{
    numLines = 0;
    numElements = 0;
    ifstream inFile;
    inFile.open(filePath.c_str());
    if (!inFile)
    {
        cout << "Error: file not found" << endl;
        return;
    }
    string line;
    int MAX = 0;
    int MIN = 0;
    while (getline(inFile, line))
    {
        numLines++;
        vector<int> lineAry;
        stringstream ss(line);
        int num;
        while (ss >> num)
        {
            lineAry.push_back(num);
            freqMap[num]++;
            numElements++;
            MAX = max(MAX, num);
            MIN = min(MIN, num);
        }
        dataAry.push_back(lineAry);
    }
    numElements /= numLines;
    inFile.close();
    for (int i = -128; i < 128; i++)
    {
        cout << i << " " << freqMap[i] << endl;
    }
    cout << "MAX: " << MAX << endl;
    cout << "MIN: " << MIN << endl;
}