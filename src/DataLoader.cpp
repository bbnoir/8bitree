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

using namespace std;

DataLoader::DataLoader(string filePath) : filePath(""), numLines(0), intPerLine(128), numInts(0)
{
    this->filePath = filePath;
    freqMap = vector<int>(SYM_NUM, 0);
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
    cout << "intPerLine: " << intPerLine << endl;
    cout << "numLines: " << numLines << endl;
    int8 *cur = (int8 *)(addr + 2 * sizeof(int));
    dataAry = vector<vector<int8>>(numLines, vector<int8>(intPerLine, 0));
    int8 num = 0;
    for (int i = 0; i < numLines; i++)
    {
        progressBar("Loading data", i, numLines - 1);
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
    numBytes = numLines * intPerLine;
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
    os << "freqMap: " << endl;
    for (int i = 0; i <= SYM_NUM; i++)
        if (dl.freqMap[i] > 0)
            os << i + SYM_MIN << ": " << dl.freqMap[i] << endl;
    return os;
}