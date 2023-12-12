#pragma once
#include "Constants.h"
#include "DataLoader.h"
#include "TreeArray.h"
#include "CodeLength.h"
#include <map>
#include <unordered_map>
#include <vector>

using namespace std;

class Encoder2
{
private:
    map<int, string> canonCode;
    int bestWidth;
    vector<int> &codeLength;
    DataLoader *dl;

public:
    Encoder2(DataLoader *dl, vector<int> &codeLength);
    int getMaxWidth();
    map<int, string> genCanonCode();
    void encode(string outputFileName);
};