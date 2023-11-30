#include "DataLoader.h"
#include "Constants.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int getMaxIndex(const vector<int> &freqMap)
{
    int maxIndex = 0;
    for (int i = 0; i < freqMap.size(); i++)
        if (freqMap[i] > freqMap[maxIndex])
            maxIndex = i;
    return maxIndex;
}

int getMinIndex(const vector<int> &freqMap)
{
    int minIndex = 0;
    for (int i = 0; i < freqMap.size(); i++)
        if (freqMap[i] < freqMap[minIndex])
            minIndex = i;
    return minIndex;
}

DataLoader::DataLoader(string filePath) : filePath(""), numLines(0), elementPerLine(128), numInts(0)
{
    this->filePath = filePath;
    freqMap = vector<freq_t>(SYM_NUM, 0);
    occurenceMap = vector<pairInt>(SYM_NUM, pairInt(0, 0));
    extremumMap = vector<pairInt>(SYM_NUM, pairInt(0, 0));
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
    elementPerLine = *(int *)addr;
    numLines = *((int *)addr + 1);
    int8 *cur = (int8 *)(addr + 2 * sizeof(int));
    dataAry = vector<vector<int8>>(numLines, vector<int8>(elementPerLine, 0));
    int8 num = 0;
    for (int i = 0; i < numLines; i++)
    {
        // progressBar("Loading data", i, numLines - 1);
        vector<int> tmpFreqMap(SYM_NUM, 0);
        for (int j = 0; j < elementPerLine; j++)
        {
            num = *(cur++);
            dataAry[i][j] = num;
            if (freqMap[num + 128] != FREQ_MAX)
                freqMap[num + 128]++;
            tmpFreqMap[num + 128]++;
        }
        int maxIndex = getMaxIndex(tmpFreqMap);
        int minIndex = getMinIndex(tmpFreqMap);
        occurenceMap[maxIndex].first++;
        occurenceMap[minIndex].second++;
        for (int i = 0; i < extremumMap.size(); i++)
        {
            if (tmpFreqMap[i] > extremumMap[i].first)
                extremumMap[i].first = tmpFreqMap[i];
            if (tmpFreqMap[i] < extremumMap[i].second)
                extremumMap[i].second = tmpFreqMap[i];
        }
    }
    munmap(addr, length);
    close(fd);
    for (int i = 0; i < SYM_NUM; i++)
    {
        if (freqMap[i] != 0)
            numInts++;
        else
        {
            occurenceMap[i].first = INT32_MIN;
            occurenceMap[i].second = INT32_MAX;
        }
    }
    numBytes = numLines * elementPerLine;
}

int DataLoader::getNumInts()
{
    return numInts;
}

int DataLoader::getNumLines()
{
    return numLines;
}

int DataLoader::getElementPerLine()
{
    return elementPerLine;
}

int DataLoader::getNumBytes()
{
    return numBytes;
}

const vector<vector<int8>> &DataLoader::getDataAry()
{
    return dataAry;
}

vector<unsigned int> DataLoader::getFreqMap()
{
    return freqMap;
}

vector<pairInt> DataLoader::getOccurenceMap()
{
    return occurenceMap;
}

vector<pairInt> DataLoader::getExtremumMap()
{
    return extremumMap;
}

ostream &operator<<(ostream &os, const DataLoader &dl)
{
    os << "filePath: " << dl.filePath << endl;
    os << "numBytes: " << dl.numBytes << endl;
    os << "numLines: " << dl.numLines << endl;
    os << "intPerLine: " << dl.elementPerLine << endl;
    os << "numInts: " << dl.numInts << endl;
    os << "freqMap: " << endl;
    for (int i = 0; i <= SYM_NUM; i++)
        if (dl.freqMap[i] > 0)
            os << i + SYM_MIN << ": " << dl.freqMap[i] << endl;
    return os;
}