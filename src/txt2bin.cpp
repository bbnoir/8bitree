#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "DataLoader.h"

using namespace std;

typedef int8_t int8;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Usage: ./txt2bin <inputFileName> <outputFileName>" << endl;
        return EXIT_FAILURE;
    }

    cout << "Loading data..." << endl;

    string inputFileName = argv[1];
    string outputFileName = argv[2];

    DataLoader *dl = new DataLoader(inputFileName);
    ofstream out(outputFileName, std::ios::binary);

    int intPerLine = dl->getIntPerLine();
    int numLines = dl->getNumLines();
    cout << "intPerLine: " << intPerLine << endl;
    cout << "numLines: " << numLines << endl;

    cout << "Writing to file..." << endl;
    out.write((char *)&intPerLine, sizeof(int));
    out.write((char *)&numLines, sizeof(int));

    for (int i = 0; i < numLines; i++)
        for (int j = 0; j < intPerLine; j++)
            out.write((char *)&dl->getDataAry()[i][j], sizeof(int8));
    cout << "Done" << endl;
    out.close();

    cout << "Start reading binary file..." << endl;
    ifstream in(outputFileName, std::ios::binary);
    int intPerLine2 = 0;
    int numLines2 = 0;
    in.read((char *)&intPerLine2, sizeof(int));
    in.read((char *)&numLines2, sizeof(int));
    cout << "intPerLine2: " << intPerLine2 << endl;
    cout << "numLines2: " << numLines2 << endl;
    if (intPerLine2 != intPerLine || numLines2 != numLines)
    {
        cout << "Error: intPerLine or numLines not match" << endl;
        return EXIT_FAILURE;
    }
    vector<vector<int8>> dataAry2(numLines2, vector<int8>(intPerLine2, 0));
    for (int i = 0; i < numLines2; i++)
        for (int j = 0; j < intPerLine2; j++)
            in.read((char *)&dataAry2[i][j], sizeof(int8));
    // check if the two data arrays are the same
    bool same = true;
    for (int i = 0; i < numLines2; i++)
        for (int j = 0; j < intPerLine2; j++)
            if (dl->getDataAry()[i][j] != dataAry2[i][j])
            {
                same = false;
                break;
            }
    if (same)
        cout << "The two data arrays are the same" << endl;
    else
        cout << "The two data arrays are different" << endl;

    return EXIT_SUCCESS;
}
