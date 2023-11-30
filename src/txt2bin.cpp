#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "DataLoader.h"

using namespace std;

typedef int8_t int8;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Usage: ./txt2bin <inputFileName>" << endl;
        return EXIT_FAILURE;
    }

    string inputFileName = argv[1];
    string outputFileName = inputFileName.substr(0, inputFileName.find_last_of('.')) + ".bin";

    cout << "inputFileName: " << inputFileName << endl;
    cout << "outputFileName: " << outputFileName << endl;
    cout << "Loading data..." << endl;
    DataLoader *dl = new DataLoader(inputFileName);
    ofstream out(outputFileName, std::ios::binary);

    int intPerLine = dl->getElementPerLine();
    int numLines = dl->getNumLines();

    cout << "Writing to file..." << endl;
    out.write((char *)&intPerLine, sizeof(int));
    out.write((char *)&numLines, sizeof(int));

    for (int i = 0; i < numLines; i++)
        for (int j = 0; j < intPerLine; j++)
            out.write((char *)&dl->getDataAry()[i][j], sizeof(int8));
    cout << "Done" << endl;
    out.close();
}
