#include <iostream>
#include "Encoder.h"
#include "Decoder.h"
#include "DataLoader.h"
#include "Constants.h"
#include "Utils.h"
#include <random>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

bool verify(DataLoader *dl, string decodedFileName)
{
    ifstream decodedFile(decodedFileName);
    string decodedLine;
    int curline = 0;
    int intPerLine = dl->getIntPerLine();
    int numLines = dl->getNumLines();
    bool diff = false;
    while (getline(decodedFile, decodedLine))
    {
        progressBar("Verify", curline++, numLines - 1);
        stringstream ss(decodedLine);
        int n;
        for (int i = 0; i < intPerLine; i++)
        {
            ss >> n;
            if (n != dl->getDataAry()[curline - 1][i])
            {
                diff = true;
                break;
            }
        }
        if (diff)
        {
            cout << endl;
            cout << "\033[31m"
                 << "     === Verification failed at line " << curline << " ==="
                 << "\033[0m" << endl;
            cout << "Data line: ";
            for (int i = 0; i < intPerLine; i++)
                cout << dl->getDataAry()[curline - 1][i] << " ";
            cout << endl;
            cout << "Decoded line: " << decodedLine << endl;
            cout << endl;
            return false;
        }
    }
    if (decodedFile.peek() != EOF)
    {
        cout << endl;
        cout << "\033[31m"
             << "     === Verification failed: Decoded file has more lines than data file ==="
             << "\033[0m" << endl;
        cout << endl;
        return false;
    }
    if (curline != numLines)
    {
        cout << endl;
        cout << "\033[31m"
             << "     === Verification failed: Data file has more lines than decoded file ==="
             << "\033[0m" << endl;
        cout << endl;
        return false;
    }
    cout << endl;
    cout << "\033[32m"
         << "     === Verification passed ==="
         << "\033[0m" << endl;
    cout << endl;
    return true;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Usage: ./test <dataFileName>" << endl;
        return EXIT_FAILURE;
    }
    string dataFileName = argv[1];
    string encodedFileName = dataFileName.substr(0, dataFileName.find_last_of('.')) + "_encoded.txt";
    string decodedFileName = dataFileName.substr(0, dataFileName.find_last_of('.')) + "_decoded.txt";

    bool clean = true;
    if (argc == 3 && string(argv[2]) == "noclean")
        clean = false;

    cout << "\033[1m";
    cout << "Start verification with data file: " << dataFileName << endl;
    cout << "\033[0m";

    // load data
    DataLoader *dl = new DataLoader(dataFileName);

    // setup tree array based on data
    cout << "Generating tree array..." << endl;
    // TreeArray *tree = TreeArray::genHuffmanArray(dl);
    cout << "numInts: " << dl->getNumInts() << endl;
    TreeArray *tree = new TreeArray(dl->getNumInts());
    srand(time(NULL));
    // tree->modify(rand() % 30000);
    cout << "Tree array: " << *tree << endl;

    // encode data based on tree array
    Encoder encoder(dl, tree);
    encoder.encode(encodedFileName);

    // decode data
    Decoder decoder(encodedFileName, decodedFileName);
    decoder.decode(dl->getNumLines());

    // verify
    bool success = verify(dl, decodedFileName);

    // clean up
    if (clean)
    {
        remove(encodedFileName.c_str());
        remove(decodedFileName.c_str());
    }

    return EXIT_SUCCESS;
}