#pragma once
#include "Constants.h"
#include "DataLoader.h"
#include "TreeArray.h"
#include <map>
#include <unordered_map>
#include <vector>

using namespace std;

class Encoder
{
private:
    vector<int> codeLength;
    map<int, string> canonCode;

public:
    DataLoader *DL;
    TreeArray *tree;
    Encoder(DataLoader *dl, TreeArray *tree);
    vector<int> genCodeLength();
    map<int, string> genCanonCode();
    static map<int, string> genCanonCode(vector<int> codeLength);
    void encode(string inputFileName, string outputFileName);
    void encode(string inputFileName, string outputFileName, int numLines);
};