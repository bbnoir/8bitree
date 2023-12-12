#pragma once
#include "Constants.h"
#include "DataLoader.h"
#include "TreeArray.h"
#include <map>
#include <unordered_map>
#include <vector>

using namespace std;

enum way
{
    FQ,
    EX,
    OC
};

class Encoder
{
private:
    map<int, string> canonCode;
    int bestWidth;
    int bestWay;

    vector<int> &codeLength;
    vector<int> cl_FQ;
    vector<int> cl_EX;
    vector<int> cl_OC;

public:
    DataLoader *dl;
    TreeArray *tree;
    Encoder(DataLoader *dl, TreeArray *tree);
    int getMaxWidth(vector<int> lenghtTable);
    int genCodeLength();
    int genCodeLength_FQ();
    int genCodeLength_EX();
    int genCodeLength_OC();
    map<int, string> genCanonCode();
    void encode(string outputFileName);
    int getBestWidth();
    int getBestWay();
    vector<int> getCodeLength();
};