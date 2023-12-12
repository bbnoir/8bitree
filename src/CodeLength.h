#pragma once
#include "Constants.h"
#include <iostream>
#include <vector>
#include <set>

#define CL_MAX 32

using namespace std;

// enum class initMode
// {
//     BALANCE,
//     RANDOM,
//     HUFFMAN,
//     HALF
// };

class CodeLength
{
private:
    vector<int> codeLength;
    vector<int> prevCodeLength;
    int symNum;
    vector<int> symList;
    vector<vector<int>> lenSet;

    void init();

public:
    CodeLength();
    CodeLength(vector<int> cl);
    CodeLength(const CodeLength &cl);
    friend ostream &operator<<(ostream &os, const CodeLength &cl);

    // for modify
    bool checkKraft();
    void modify(int times);
    void recover();

    // get
    vector<int> getCodeLength();
    vector<int> &getCodeLengthRef();
};