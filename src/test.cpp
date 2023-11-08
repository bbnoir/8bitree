#include "Encoder.h"
#include "Decoder.h"
#include "Constants.h"
#include <random>
#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    // given code length
    vector<int> codeLength(INT_NUM, 0);
    for (int i = 0; i < INT_NUM; i++)
        codeLength[i] = 8;
    // generate canonical code
    map<int, string> canonCode = genCanonCode(codeLength);
    for (auto i : canonCode)
        cout << i.first << " " << i.second << endl;

    // given encoded data, output decoded data
    ifstream in("encoded_data.txt");
    ofstream out("decoded_data.txt");
    Decoder decoder(in);
    while (decoder.notEOF())
    {
        out << decoder.readline() << endl;
    }

    return EXIT_SUCCESS;
}