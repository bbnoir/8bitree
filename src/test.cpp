#include "encoder.h"
#include <random>
#include <iostream>

using namespace std;

int main()
{
    vector<int> codeLength(256, 0);
    for (int i = 0; i < 256; i++)
        codeLength[i] = 8;
    map<int, string> canonCode = genCanonCode(codeLength);
    for (auto i : canonCode)
        cout << i.first << " " << i.second << endl;
    return 0;
}