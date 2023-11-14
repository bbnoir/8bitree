#include "DataLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

int8 findMin(vector<int> freqMap);

int8 findMax(vector<int> freqMap);

DataLoader::DataLoader() : filePath(""), numBytes(0), numLines(0), intPerLine(128), numInts(256), Max(0), Min(0)
{
    dataAry = vector<vector<int8>>(numLines, vector<int8>(intPerLine, 0));
    freqMap = vector<int>(256, 0);
}

DataLoader::DataLoader(string filePath) : filePath(""), numLines(0), intPerLine(128), numInts(0), Max(0), Min(0)
{
    this->filePath = filePath;
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
    int8 num = 0;
    int N = 0;
    while (getline(inFile, line))
    {
        lineAry.clear();
        numLines++;
        stringstream ss(line);
        while (ss >> N)
        {
            num = int8(N);
            lineAry.push_back(num);
            freqMap[num + 128]++;
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
    intPerLine = dataAry[0].size();
    numBytes = numLines * intPerLine;
}

DataLoader::DataLoader(int bin, string filePath) : filePath(""), numLines(0), intPerLine(128), numInts(0), Max(0), Min(0)
{
    this->filePath = filePath;
    freqMap = vector<int>(256, 0);
    if (bin == 0)
    {
        ifstream inFile(filePath, std::ios::binary);
        if (!inFile)
        {
            cout << "Error: can't open file" << endl;
            exit(1);
        }
        inFile.read((char *)&intPerLine, sizeof(int));
        inFile.read((char *)&numLines, sizeof(int));
        dataAry = vector<vector<int8>>(numLines, vector<int8>(intPerLine, 0));
        int8 num = 0;
        for (int i = 0; i < numLines; i++)
            for (int j = 0; j < intPerLine; j++)
            {
                inFile.read((char *)&num, sizeof(int8));
                dataAry[i][j] = num;
                freqMap[num + 128]++;
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
        numBytes = numLines * intPerLine;
    }
    else
    {
        // read data using mmap
        int fd = open(filePath.c_str(), O_RDONLY);
        if (fd == -1)
        {
            cout << "Error: can't open file" << endl;
            exit(1);
        }
        struct stat sb;
        if (fstat(fd, &sb) == -1)
        {
            cout << "Error: can't get file size" << endl;
            exit(1);
        }
        size_t length = sb.st_size;
        int8 *addr = (int8 *)mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0);
        if (addr == MAP_FAILED)
        {
            cout << "Error: mmap failed" << endl;
            exit(1);
        }
        intPerLine = *(int *)addr;
        numLines = *((int *)addr + 1);
        int8 *cur = addr + 2;
        dataAry = vector<vector<int8>>(numLines, vector<int8>(intPerLine, 0));
        int8 num = 0;
        for (int i = 0; i < numLines; i++)
        {
            for (int j = 0; j < intPerLine; j++)
            {
                num = *(cur++);
                dataAry[i][j] = num;
                freqMap[num + 128]++;
            }
        }
        munmap(addr, length);
        close(fd);
        for (int i = 0; i < 256; i++)
        {
            if (freqMap[i] > 0)
            {
                numInts++;
            }
        }
        Max = findMax(freqMap);
        Min = findMin(freqMap);
        numBytes = numLines * intPerLine;
    }
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

int DataLoader::getNumLines()
{
    return numLines;
}

int DataLoader::getIntPerLine()
{
    return intPerLine;
}

const vector<vector<int8>> &DataLoader::getDataAry()
{
    return dataAry;
}

vector<int> DataLoader::getFreqMap()
{
    return freqMap;
}

ostream &operator<<(ostream &os, const DataLoader &dl)
{
    os << "filePath: " << dl.filePath << endl;
    os << "numBytes: " << dl.numBytes << endl;
    os << "numLines: " << dl.numLines << endl;
    os << "intPerLine: " << dl.intPerLine << endl;
    os << "numInts: " << dl.numInts << endl;
    os << "Max: " << int(dl.Max) << endl;
    os << "Min: " << int(dl.Min) << endl;
    os << "freqMap: " << endl;
    for (int i = 0; i < 256; i++)
    {
        if (dl.freqMap[i] > 0)
        {
            os << i - 128 << ": " << dl.freqMap[i] << endl;
        }
    }
    return os;
}